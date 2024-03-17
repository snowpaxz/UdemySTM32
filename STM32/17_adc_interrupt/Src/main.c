#include "stm32l1xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "myclock.h"
#include "adc.h"

static void ADC_callback(void);
uint32_t sensor_value;
int main(void)
{
	// STM32L152RE restarts to approximately 2MHz
	// Need to step this up to 16MHz
	clock_step_up_to_16M();

	uart2_tx_init();

	pa1_adc_interrupt_init();
	start_conversion();

	while(1)
	{

	}

}

static void ADC_callback(void)
{
	sensor_value = ADC1->DR;
	printf("Sensor value %d\r\n", (int)sensor_value);
}

void ADC1_IRQHandler(void)
{
	if(ADC1->SR & SR_EOC)
	{
		/* EOC bit cleared by reading from ADC DR */
		ADC1->SR &= ~SR_EOC;

		// Do something...
		ADC_callback();
	}
}



