#include "stm32l1xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "myclock.h"
#include "i2c.h"

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

	uart2_tx_init();
	I2C1_init();

	while(1)
	{

	}
}

