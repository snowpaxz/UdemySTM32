/*
 * tim.c
 *
 *  Created on: Mar 10, 2024
 *      Author: lars
 */

#include "stm32l1xx.h"
#include "tim.h"

#define TIM2EN		(1U<<0)
#define CR1_CEN		(1U<<0)

#define DIER_UIE	(1U<<0)

void tim2_1hz_interrupt_init(void)
{
	/* Enable clock to timer2 */
	RCC->APB1ENR |= TIM2EN;

	/* Set prescaler value */
	TIM2->PSC = 1600 - 1;

	/* Set autoreload value */
	TIM2->ARR = 10000 - 1;

	/* Set timer as counter */
	TIM2->CNT = 0;

	/* Enable timer */
	TIM2->CR1 |= CR1_CEN;

	/* Enable TIM2 Interrupt */
	TIM2->DIER |= DIER_UIE;

	/* Add TIM2 to NVIC Interrupt */
	NVIC_EnableIRQ(TIM2_IRQn);


}

/* Set timer 2 to generate a 1Hz timeout */
void tim2_1hz_init(void)
{
	/* Enable clock to timer2 */
	RCC->APB1ENR |= TIM2EN;

	/* Set prescaler value */
	TIM2->PSC = 1600 - 1;

	/* Set autoreload value */
	TIM2->ARR = 10000 - 1;

	/* Set timer as counter */
	TIM2->CNT = 0;

	/* Enable timer */
	TIM2->CR1 |= CR1_CEN;

}

