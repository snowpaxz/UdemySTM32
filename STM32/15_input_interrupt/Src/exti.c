/*
 * exti.c
 *
 *  Created on: Mar 14, 2024
 *      Author: lars
 */

#include "exti.h"

#define GPIOCEN			(1U<<2)
#define SYSCFGEN		(1U<<0)



void pc13_exti_init(void)
{
	/* Disable global interrupt */
	__disable_irq();

	/* Enable clock access for GPIOC */
	RCC->AHBENR |= GPIOCEN;

	/* Enable clock access to SYSCONFIG */
	RCC->APB2ENR |= SYSCFGEN;

	/* Set PORTC for EXTI13 */
	GPIOC->MODER &=	~(1U<<26);
	GPIOC->MODER &=	~(1U<<27);

	SYSCFG->EXTICR[3] &= ~(1U<<7);
	SYSCFG->EXTICR[3] &= ~(1U<<6);
	SYSCFG->EXTICR[3] |= (1U<<5);
	SYSCFG->EXTICR[3] &= ~(1U<<4);

	/* Unmask EXTI13 */
	EXTI->IMR |= (1U<<13);

	/* Set trigger to falling edge */
	EXTI->FTSR |= (1U<<13);

	/* Enable EXTI13 in NVIC */
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* Enable global interrupt */
	__enable_irq();

}
