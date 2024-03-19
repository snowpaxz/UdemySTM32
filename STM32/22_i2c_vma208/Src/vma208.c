/*
 * vma208.c
 *
 *  Created on: Mar 19, 2024
 *      Author: lars
 */

#include "vma208.h"


char data;
uint8_t data_rec[6];

void vma208_read_address(uint8_t reg)
{
	I2C1_byteRead(SADDR, reg, &data);
}

void vma208_write(uint8_t reg, char value)
{
	char data[1];
	data[0] = value;

	I2C1_burstWrite(SADDR, reg, 1, data);
}

void vma208_read_values(uint8_t reg)
{
	I2C1_burstRead(SADDR, reg, 6, (char *)data_rec);
}

void vma208_init(void)
{
	/* Enable I2C */
	I2C1_init();

	/* Read DEVID, should return 0x1C */
	vma208_read_address(DEVID_R);

	/* Set data range to 4G+/- */
	vma208_write(DATAFORMAT_R, FOUR_G);

	/* Deactivate */
	vma208_write(POWERCTRL_R, 0);

	/* Configure control bits */
	vma208_write(POWERCTRL_R, (SET_MEASURE_B | 1U));
}
