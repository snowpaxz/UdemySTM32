#include "stm32l1xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "myclock.h"

#define GPIOAEN (1U<<0)
#define GPIOA_5	(1U<<5)

#define LED_PIN	GPIOA_5

char key;
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

	uart2_rxtx_init();
	while(1)
	{
		key = uart2_read();
		if((key == 'y')){
			GPIOA->BSRR |= LED_PIN;
		} else {
			GPIOA->BSRR |= (1U<<21);
		}


	}

}



