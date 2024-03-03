#include "stm32l1xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "myclock.h"


int main(void)
{
	// STM32L152RE restarts to approximately 2MHz
	// Need to step this up to 16MHz
	clock_step_up_to_16M();

	uart2_tx_init();
	while(1)
	{
		printf("Hello from STM32L152!......\r\n");

		for(int i = 0; i < 100000; i++){}
	}
}



