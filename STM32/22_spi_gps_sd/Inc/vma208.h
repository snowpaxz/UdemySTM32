/*
 * vma208.h
 *
 *  Created on: Mar 19, 2024
 *      Author: lars
 */

#ifndef VMA208_H_
#define VMA208_H_

#include "i2c.h"
#include <stdint.h>

#define DEVID_R			0x0D
#define SADDR			0x1C

#define DATAFORMAT_R	0x0E
#define POWERCTRL_R		0x2A

#define DATA_START_R	0x01

#define FOUR_G			0x01
#define RESET			0x00
#define SET_MEASURE_B	(0b110<<3)


void vma208_init(void);
void vma208_read_values(uint8_t reg);

#endif /* VMA208_H_ */
