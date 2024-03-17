#include "stm32l1xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "myclock.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"

#define GPIOAEN (1U<<0)
#define PIN5	(1U<<5)
#define LEDPIN	PIN5


static void exti_callback(void);

int main(void)
{
	clock_step_up_to_16M();

	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	pc13_exti_init();
	uart2_tx_init();

	while(1)
	{

	}

}

static void exti_callback(void)
{
	printf("Btn pressed \n\r");

	GPIOA->ODR ^= LEDPIN;

}

void EXTI15_10_IRQHandler(){
	if(EXTI->PR & LINE13){
		/* Clear PR flag */
		EXTI->PR |= LINE13;

		// Do desired function now that interrupt occurred
		exti_callback();
	}
}

