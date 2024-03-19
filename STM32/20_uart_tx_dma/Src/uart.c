/*
 * uart.c
 *
 */


#include "uart.h"

#define GPIOAEN		(1U<<0)
#define UART2EN		(1U<<17)

#define CR1_TE		(1U<<3)
#define CR1_RE		(1U<<2)
#define CR1_UE		(1U<<13)
#define SR_TXE		(1U<<7)

#define RCC_DMA1EN	(1U<<24)
#define DMA_Cx_EN	(1U<<0)



#define CR1_RXNEIE	(1U<<5)

#define SYS_FREQ	16000000
#define APB1_CLK	SYS_FREQ

#define UART_BAUDRATE	115200

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t baudRate);
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t baudRate);

void uart2_write(int ch);


int __io_putchar(int ch)
{
	uart2_write(ch);
	return(ch);
}

void dma1_stream7_init(uint32_t src, uint32_t dst, uint32_t len)
{
	/* Enable clock access to DMA */
	RCC->AHBENR |= RCC_DMA1EN;

	/* Disable DMA1 Channel 7*/
	DMA1_Channel7->CCR &= ~(DMA_Cx_EN);
	while(DMA1_Channel7->CCR & DMA_Cx_EN){}
	/* Clear interrupt flags for stream 7 */
	DMA1->IFCR |= CH7_TERR_ICLR;
	DMA1->IFCR |= CH7_TH_ICLR;
	DMA1->IFCR |= CH7_TDONE_ICLR;


	/* Set destination buffer */
	DMA1_Channel7->CPAR = dst;

	/* Set source buffer */
	DMA1_Channel7->CMAR = src;

	/* Set length */
	DMA1_Channel7->CNDTR = len;

	/* Enable memory increment */
	DMA1_Channel7->CCR |= (1U<<7);

	/* Configure transfer direction (memory to peripheral) */
	DMA1_Channel7->CCR |= (1U<<4);

	/* Enable circular mode */
	DMA1_Channel7->CCR |= (1U<<5); //Already cleared

	/* Enable DMA interrupts */
	DMA1_Channel7->CCR |= (DMA_TDONE_IEN); //|| DMA_THALF_IEN || DMA_TERR_IEN);

	/* Enable DMA1 Stream 7 */
	DMA1_Channel7->CCR |= DMA_Cx_EN;

	/* Enable UART2 transmitter DMA */
	USART2->CR3 |= (1<<7);

	/* Enable DMA interrupt in NVIC */
	NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

void uart2_rx_interrupt_init()
{
	/********* Configure uart GPIO tx pin *********/
	/* Enable clock access to GPIOA */
	RCC->AHBENR |= GPIOAEN;

	/* Set PA2 mode to alternate function mode */
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	/* Set PA3 to alternate function mode */
	GPIOA->MODER &= ~(1U<<6);
	GPIOA->MODER |= (1U<<7);

	/* Set PA2 alternate function type to UART_TX (AF07) */
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	/* Set PA3 alternate function type to UART_RX */
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);

	/********* Configure uart module *********/
	/* Enable clock access to UART2 */
	RCC->APB1ENR |= UART2EN;

	/* Configure UART baud rate */
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/* Configure transfer direction */
	USART2->CR1 = CR1_TE | CR1_RE;

	/* Enable RX interrupt */
	USART2->CR1 |= CR1_RXNEIE;

	/* Enable UART2 in NVIC */
	NVIC_EnableIRQ(USART2_IRQn);

	/* Enable uart module */
	USART2->CR1 |= CR1_UE;
}

void uart2_rxtx_init()
{
	/********* Configure uart GPIO tx pin *********/
	/* Enable clock access to GPIOA */
	RCC->AHBENR |= GPIOAEN;

	/* Set PA2 mode to alternate function mode */
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	/* Set PA3 to alternate function mode */
	GPIOA->MODER &= ~(1U<<6);
	GPIOA->MODER |= (1U<<7);

	/* Set PA2 alternate function type to UART_TX (AF07) */
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	/* Set PA3 alternate function type to UART_RX */
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);

	/********* Configure uart module *********/
	/* Enable clock access to UART2 */
	RCC->APB1ENR |= UART2EN;

	/* Configure UART baud rate */
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/* Configure transfer direction */
	USART2->CR1 = CR1_TE | CR1_RE;

	/* Enable uart module */
	USART2->CR1 |= CR1_UE;
}

void uart2_tx_init()
{
	/********* Configure uart GPIO tx pin *********/
	/* Enable clock access to GPIOA */
	RCC->AHBENR |= GPIOAEN;

	/* Set PA2 mode to alternate function mode */
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	/* Set PA2 alternate function type to UART_TX (AF07) */
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	/********* Configure uart module *********/
	/* Enable clock access to UART2 */
	RCC->APB1ENR |= UART2EN;

	/* Configure UART baud rate */
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/* Configure transfer direction */
	USART2->CR1 = CR1_TE;

	/* Enable uart module */
	USART2->CR1 |= CR1_UE;
}

char uart2_read(void)
{
	while(!(USART2->SR & SR_RXNE)){}

	return USART2->DR;
}

void uart2_write(int ch)
{
	/* Make sure TX data register is empty */
	while(!(USART2->SR & SR_TXE)){}
	/* Write to TX data register */
	USART2->DR	= (ch & 0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t baudRate)
{
	USARTx->BRR = compute_uart_bd(PeriphClk, baudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t baudRate)
{
	return PeriphClk / baudRate;
}

