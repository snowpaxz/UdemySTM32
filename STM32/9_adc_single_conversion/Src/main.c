#include "stm32l1xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "myclock.h"
#include "adc.h"


uint32_t sensor_value;
int main(void)
{
	// STM32L152RE restarts to approximately 2MHz
	// Need to step this up to 16MHz
	clock_step_up_to_16M();

	uart2_tx_init();

	pa1_adc_init();

	while(1)
	{
		sensor_value = adc_read();
		start_conversion();

		printf("Sensor value %d\r\n", (int)sensor_value);
	}

}



