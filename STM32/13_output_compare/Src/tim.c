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

#define OC_TOGGLE	(1U<<4) | (1U<<5)
#define CCER_CC1E	(1U<<0)

#define GPIOAEN		(1U<<0)
#define	PIN5		(1U<<5)
#define LED_PIN		PIN5

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

void tim2_PA5_output_compare(void)
{
	// Setup GPIO
	RCC->AHBENR |= GPIOAEN;

	// Set Mode to Alternate Function
	GPIOA->MODER |= (1U<<11);
	GPIOA->MODER &= ~(1U<<10);

	// Set Alternate Function to mode AF1
	GPIOA->AFR[0] |= (1U<<20);

	/* Enable clock to timer2 */
	RCC->APB1ENR |= TIM2EN;

	/* Set prescaler value */
	TIM2->PSC = 1600 - 1;

	/* Set autoreload value */
	TIM2->ARR = 10000 - 1;

	/* Set output compare toggle mode */
	TIM2->CCMR1 |= OC_TOGGLE;

	/* Enable TIM2 CH1 */
	TIM2->CCER |= CCER_CC1E;


	/* Set timer as counter */
	TIM2->CNT = 0;

	/* Enable timer */
	TIM2->CR1 |= CR1_CEN;

}

