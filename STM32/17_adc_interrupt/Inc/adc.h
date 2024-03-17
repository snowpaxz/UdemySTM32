/*
 * adc.h
 *
 *  Created on: Mar 3, 2024
 *      Author: lars
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32l1xx.h"
#include <stdint.h>

void pa1_adc_init();
void start_conversion(void);
void pa1_adc_interrupt_init(void);
uint32_t adc_read(void);

#define SR_EOC			(1U<<1)

#endif /* ADC_H_ */
