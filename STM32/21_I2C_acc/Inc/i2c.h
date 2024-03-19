/*
 * i2c.h
 *
 *  Created on: Mar 19, 2024
 *      Author: lars
 */

#ifndef I2C_H_
#define I2C_H_

void I2C1_init(void);
void I2C1_byteRead(char saddr, char maddr, char* data);

#endif /* I2C_H_ */
