#include "stm32l1xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "myclock.h"

#include "systick.h"


#define GPIOAEN		(1U<<0)
#define	PIN5		(1U<<5)
#define LED_PIN		PIN5



int count = 0;
int main(void)
{
	// STM32L152RE restarts to approximately 2MHz
	// Need to step this up to 16MHz
	clock_step_up_to_16M();

	// Setup UART
	uart2_tx_init();

	// Setup GPIO
	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	systick_1hz_interrupt();

	while(1)
	{

	}

}

void SysTick_callback(void)
{
	GPIOA->ODR ^= LED_PIN;

	printf("Count value %d\r\n", count++);

}

void SysTick_Handler (void)
{
	SysTick_callback();
}

