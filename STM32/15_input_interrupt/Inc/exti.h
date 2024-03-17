/*
 * exti.h
 *
 *  Created on: Mar 14, 2024
 *      Author: lars
 */

#ifndef EXTI_H_
#define EXTI_H_

#include "stm32l1xx.h"

#define 	LINE13		(1U<<13)

void pc13_exti_init(void);

#endif /* EXTI_H_ */
