/*
 * systick.h
 *
 *  Created on: Mar 10, 2024
 *      Author: lars
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "stm32l1xx.h"

void systickDelayMs(int delay);
void systick_1hz_interrupt(void);

#endif /* SYSTICK_H_ */
