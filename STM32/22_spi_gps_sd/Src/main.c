#include "stm32l1xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "myclock.h"
#include "vma208.h"


int16_t x, y, z;
float xg, yg, zg;

extern uint8_t data_rec[6];

int main(void)
{
	// STM32L152RE restarts to approximately 2MHz
	// Need to step this up to 16MHz
	clock_step_up_to_16M();

	//uart2_tx_init();
	vma208_init();



	while(1)
	{
		vma208_read_values(DATA_START_R);
		x = ((data_rec[0] << 8)|data_rec[1]);
		y = ((data_rec[2] << 8)|data_rec[3]);
		z = ((data_rec[4] << 8)|data_rec[5]);

		xg = x * 0.0012;
		yg = y * 0.0012;
		zg = z * 0.0012;
	}
}

