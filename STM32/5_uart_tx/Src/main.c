#include "stm32l1xx.h"
#include <stdint.h>

#define GPIOAEN		(1U<<0)
#define UART2EN		(1U<<17)

#define	PIN5		(1U<<5)
#define LED_PIN		PIN5

#define CR1_TE		(1U<<3)
#define CR1_UE		(1U<<13)
#define SR_TXE		(1U<<7)

#define SYS_FREQ	16000000
#define APB1_CLK	SYS_FREQ

#define UART_BAUDRATE	115200


static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t baudRate);
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t baudRate);

void clock_step_up_to_16M();
void clock_step_up_to_4M();
void uart2_tx_init();
void uart2_write(int ch);

int main(void)
{
	clock_step_up_to_16M();

	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	uart2_tx_init();
	while(1)
	{
		uart2_write('y');
		GPIOA->ODR ^= LED_PIN;
		for(int i = 0; i < 100000; i++){
			i = i;
		}
	}
}


void clock_step_up_to_4M()
{
	RCC->ICSCR |= (1U<<15);
	RCC->ICSCR |= (1U<<14);
	RCC->ICSCR &= ~(1U<<13);
}

void clock_step_up_to_16M()
{
	clock_step_up_to_4M();
	for(int i = 0; i<10000; i++){}

	RCC->CR |= (1U<<0);
	while(!(RCC->CR & (1U<<1))){}

	RCC->CFGR &= ~(1U<<1);
	RCC->CFGR |= (1U<<0);
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
