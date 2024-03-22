/*
 * i2c.c
 *
 *  Created on: Mar 19, 2024
 *      Author: lars
 */

#include "stm32l1xx.h"

#include "i2c.h"

#define GPIOBEN	(1U<<1)
#define I2C1EN	(1U<<21)

#define CR1_PE	(1U<<0)

#define I2C_100KHZ	80
#define SD_MODE_MAX_RISE_TIME	17

#define SR2_BUSY (1U<<1)
#define CR1_START (1U<<8)
#define SR1_START (1U<<0)
#define SR1_ADDR (1U<<1)
#define SR1_TXE	(1U<<7)
#define CR1_ACK	(1U<<10)
#define CR1_STOP (1U<<9)

#define SR1_RxNE (1U<<6)
#define SR1_BTF (1U<<2)

/* Use PB8 as i2c SCL, PB9 as SDA */
void I2C1_init()
{
	/* Enable clock access to GPIOB */
	RCC->AHBENR |= GPIOBEN;

	/* Set PB8 and PB9 to AFIO 4 */
	// Set PIN8 to 10
	GPIOB->MODER |= (1U<<17);
	GPIOB->MODER &= ~(1U<<16);

	// Set PIN9 to 10
	GPIOB->MODER |= (1U<<19);
	GPIOB->MODER &= ~(1U<<18);

	// Set Pin8 to AF4
	GPIOB->AFR[1] &= ~(1U<<3);
	GPIOB->AFR[1] |= (1U<<2);
	GPIOB->AFR[1] &= ~(1U<<1);
	GPIOB->AFR[1] &= ~(1U<<0);

	// Set Pin9 to AF4
	GPIOB->AFR[1] &= ~(1U<<7);
	GPIOB->AFR[1] |= (1U<<6);
	GPIOB->AFR[1] &= ~(1U<<5);
	GPIOB->AFR[1] &= ~(1U<<4);

	/* Set output mode to open drain */
	GPIOB->OTYPER |= (1U<<8);
	GPIOB->OTYPER |= (1U<<9);

	/* Enable pullups on PB8 & PB9 */
	GPIOB->PUPDR &= ~(1U<<17);
	GPIOB->PUPDR |= (1U<<16);

	GPIOB->PUPDR &= ~(1U<<19);
	GPIOB->PUPDR |= (1U<<18);

	/* Enable clock access to I2C1 */
	RCC->APB1ENR |= I2C1EN;

	/* Enter reset mode */
	I2C1->CR1 |= (1U<<15);

	/* Exit reset mode */
	I2C1->CR1 &= ~(1U<<15);

	/* Set frequency to 16Mhz */
	I2C2->CR2 &= ~(1U<<5);
	I2C2->CR2 |= (1U<<4);
	I2C2->CR2 &= ~(1U<<3);
	I2C2->CR2 &= ~(1U<<2);
	I2C2->CR2 &= ~(1U<<1);
	I2C2->CR2 &= ~(1U<<0);

	/* Set I2C to Standard Mode */
	I2C1->CCR = I2C_100KHZ;

	/* Set I2C Rise Time */
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	/* Enable I2C */
	I2C1->CR1 |= CR1_PE;
}

void I2C1_byteRead(char saddr, char maddr, char* data)
{
	volatile int temp;
	/* Check if bus is busy */
	while(I2C1->SR2 & SR2_BUSY){}

	/* Set Start Bit */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_START)){}

	/* Write periph address to DR with write flag
	 * I2C read is 1, write is 0, by shifting, bit 0 is left as 0
	 * This is the standard for calling to a peripheral to request data */
	I2C1->DR = saddr << 1;

	/* Wait until address flag is set */
	while(!(I2C1->SR1 & SR1_ADDR)){}

	/* Clear addr flag */
	temp = I2C1->SR2;

	/* Wait until TX is empty */
	while(!(I2C1->SR1 & SR1_TXE)){}

	/* Send memory address */
	I2C1->DR = maddr;

	/* Wait until TX is empty */
	while(!(I2C1->SR1 & SR1_TXE)){}

	/* Create restart condition */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_START)){}

	/* Transmit periph address + read */
	I2C1->DR = saddr << 1 | 1;

	/* Wait until address flag is set */
	while(!(I2C1->SR1 & SR1_ADDR)){}

	/* Disable ACK */
	I2C1->CR1 &= ~CR1_ACK;

	/* Clear addr flag */
	temp = I2C1->SR2;

	/* Generate stop after data received */
	I2C1->CR1 |= CR1_STOP;

	/* Wait until RXNE flag set */
	while(!(I2C1->SR1 & SR1_RxNE)){}

	/* Read data from DR */
	*data++ = I2C1->DR;
}

void I2C1_burstRead(char saddr, char maddr, int n, char* data)
{
	volatile int temp;

	/* Wait until bus not busy */
	while(I2C1->SR2 & SR2_BUSY){}

	/* Set Start Bit */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_START)){}

	/* Write Periph address + Write */
	I2C1->DR = saddr << 1;

	/* Wait until address flag is set */
	while(!(I2C1->SR1 & SR1_ADDR)){}

	/* Clear addr flag */
	temp = I2C1->SR2;

	/* Wait until TX is empty */
	while(!(I2C1->SR1 & SR1_TXE)){}

	/* Send memory address */
	I2C1->DR = maddr;

	/* Wait until TX is empty */
	while(!(I2C1->SR1 & SR1_TXE)){}

	/* Create restart condition */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_START)){}

	/* Transmit periph address + read */
	I2C1->DR = saddr << 1 | 1;

	/* Wait until address flag is set */
	while(!(I2C1->SR1 & SR1_ADDR)){}

	/* Clear addr flag */
	temp = I2C1->SR2;

	/* Enable ACK */
	I2C1->CR1 |= CR1_ACK;

	while(n > 0U)
	{
		// Check for final byte (since n is decrementing)
		if(n == 1)
		{
			/* Disable ACK */
			I2C1->CR1 &= ~CR1_ACK;

			/* Generate stop after data received */
			I2C1->CR1 |= CR1_STOP;

			/* Wait until RXNE flag set */
			while(!(I2C1->SR1 & SR1_RxNE)){}

			/* Read data from DR */
			*data++ = I2C1->DR;

			break;
		} else {
			/* Wait until RXNE flag set */
			while(!(I2C1->SR1 & SR1_RxNE)){}

			/* Read data from DR */
			*data++ = I2C1->DR;

			n--;
		}
	}
}

void I2C1_burstWrite(char saddr, char maddr, int n, char* data)
{
	volatile int temp;
	/* Check if bus is busy */
	while(I2C1->SR2 & SR2_BUSY){}

	/* Set Start Bit */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_START)){}

	/* Transmit periph address with write */
	I2C1->DR = saddr << 1;

	/* Wait for addr bit to be set */
	while(!(I2C1->SR1 & SR1_ADDR)){}

	/* Clear addr flag */
	temp = I2C1->SR2;

	/* Wait until TX is empty */
	while(!(I2C1->SR1 & SR1_TXE)){}

	/* Send memory address with write */
	I2C1->DR = maddr;

	for(int i = 0; i < n; i++)
	{
		/* Wait until data register is empty */
		while(!(I2C1->SR1 & SR1_TXE)){}

		I2C1->DR = *data++;

	}
	/* Wait until tx is finished */
	while(!(I2C1->SR1 & SR1_BTF)){}

	/* Generate stop */
	I2C1->CR1 |= CR1_STOP;
}
