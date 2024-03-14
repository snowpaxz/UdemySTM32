#include "stm32l1xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "myclock.h"
#include "systick.h"
#include "tim.h"

int timestamp = 0;

/* Set up: Connect a jumper from PA5 to PA6 */
int main(void)
{
	// STM32L152RE restarts to approximately 2MHz
	// Need to step this up to 16MHz
	clock_step_up_to_16M();

	tim2_PA5_output_compare();
	tim3_pa6_input_capture();

	while(1)
	{
		/* Wait until capture interrupt */
		while(!(TIM3->SR & SR_CC1IF)){}

		/* Read value */
		timestamp = TIM3->CCR1;
	}

}



