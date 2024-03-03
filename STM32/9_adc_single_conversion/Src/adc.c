/*
 * adc.c
 *
 *  Created on: Mar 3, 2024
 *      Author: lars
 */

#include "adc.h"

#define ADC1EN			(1U<<9)
#define GPIOAEN			(1U<<0)
#define ADC_CH1			(1U<<0)
#define ADC_SEQ_LEN_1	0x00
#define CR2_ADON		(1U<<0)
#define CR2_SWSTART 	(1U<<30)
#define SR_EOC			(1U<<1)

void pa1_adc_init()
{
	/*** Configure ADC pin ***/
	/* Enable clock access to GPIOA */
	RCC->AHBENR |= GPIOAEN;
	/* Set mode of PA1 to analog in mode register */
	GPIOA->MODER |= (1U<<2);
	GPIOA->MODER |= (1U<<3);

	/*** Configure ADC Peripheral ***/
	/* Enable clock access to ADC */
	RCC->APB2ENR |= ADC1EN;

	/** Configure ADC parameters **/
	/* Conversion sequence start */
	ADC1->SQR5 = ADC_CH1;

	/* Conversion sequence length */
	ADC1->SQR1 = ADC_SEQ_LEN_1;

	/* Enable ADC Module */
	ADC1->CR2 |= CR2_ADON;
}


void start_conversion(void)
{
	/* Start adc conversion */
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void)
{
	/* Wait for conversion to finish */
	while(!(ADC1->SR & SR_EOC)){}

	/* Read converted result */
	return ADC1->DR;
}
