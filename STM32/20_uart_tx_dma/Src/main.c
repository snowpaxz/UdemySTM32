#include "stm32l1xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "myclock.h"

#include <stdlib.h>

#define GPIOAEN (1U<<0)
#define GPIOA_5	(1U<<5)

#define LED_PIN	GPIOA_5

void dma_callback(void);


int i = 0;
int main(void)
{
	// STM32L152RE restarts to approximately 2MHz
	// Need to step this up to 16MHz
	clock_step_up_to_16M();

	// Enable clock to GPIOA
	RCC->AHBENR |= GPIOAEN;

	// Set GPIOA5 as output
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	uart2_tx_init();

	char name[31] = "Hello from stm32 DMA transfer\r\n";
	dma1_stream7_init((uint32_t)name, (uint32_t)&USART2->DR, 31);//strlen(name));


	while(1)
	{
		i++;

		if(i > 100000){
			i = 0;
			int num = rand();
			num = (num + 32) % (126 + 1);
			if(num < 32)
			{
				num = 32;
			} else if(num > 126){
				num = 126;
			}
			int index = rand() % 29;
			name[index] = (char) num;

			NVIC_EnableIRQ(DMA1_Channel7_IRQn);
		}
	}
}


void dma_callback()
{
		GPIOA->BSRR |= LED_PIN;
		for(int e = 0; e<100000; e++){}
		GPIOA->BSRR |= (1U<<21);
		for(int e = 0; e<100000; e++){}
		NVIC_DisableIRQ(DMA1_Channel7_IRQn);
}

void DMA1_Channel7_IRQHandler()
{
	// Note: DMA sets Half flag even if interrupt is not enabled
	// This allows software polling w/o interrupts
	// To check HT flag, check that a) flag is set and b) HT interrupt is enabled
	if (DMA1->ISR & CH7_TH_ICLR && DMA1_Channel7->CCR & DMA_THALF_IEN){
		DMA1->IFCR |= CH7_TH_ICLR;

		dma_callback();
	} else if (DMA1->ISR & CH7_TDONE_ICLR && DMA1_Channel7->CCR & DMA_TDONE_IEN){
		DMA1->IFCR |= CH7_TDONE_ICLR;

		dma_callback();

	}
}
//CH7_TH_ICLR CH7_TERR_ICLR CH7_TDONE_ICLR


