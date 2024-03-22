/*
 * myclock.c
 *
 *  Created on: Mar 3, 2024
 *      Author: lars
 */

#include "myclock.h"

void clock_step_up_to_4M();


void clock_step_up_to_4M()
{
	RCC->ICSCR |= (1U<<15);
	RCC->ICSCR |= (1U<<14);
	RCC->ICSCR &= ~(1U<<13);
}

void clock_step_up_to_16M()
{
	clock_step_up_to_4M();
	for(int i = 0; i<25; i++){}

	RCC->CR |= (1U<<0);
	while(!(RCC->CR & (1U<<1))){}

	RCC->CFGR &= ~(1U<<1);
	RCC->CFGR |= (1U<<0);
}
