/*
 * systick.c
 *
 *  Created on: Mar 10, 2024
 *      Author: lars
 */


#include "systick.h"

#define SYSTICK_LOAD_VAL	16000
#define CTRL_ENABLE			(1U<<0)
#define CTRL_CLKSRC			(1U<<2)
#define CTRL_COUNTFLAG		(1U<<16)

void systickDelayMs(int delay)
{
	/*** Configure SYSTICK ***/
	/* Load SysTick->LOAD with number of clock cycles per ms */
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	/* Clear SysTick current value register */
	SysTick->VAL = 0;

	/* Enable SysTick and select internal clock source */
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for(int i = 0; i < delay; i++)
	{
		/* Wait until COUNTFLAG is set */
		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0){}
	}
	SysTick->CTRL = 0;
}
