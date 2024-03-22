/*
 * spi.c
 *
 *  Created on: Mar 21, 2024
 *      Author: lars
 */

#include "spi.h"


#define SPI1EN	(1U<<12)
#define GPIOAEN	(1U<<0)
#define GPIOBEN	(1U<<1)


// D13 - PA5 - SPI CLK
// D12 - PA6 - SPI MISO
// D11 - PA7 - SPI MOSI
// D10 - PB6 - SPI CS

void spi_gpio_init(void)
{
	/* Enable clock access on GPIOA */
	RCC->AHBENR |= GPIOAEN;
	/* Enable clock access on GPIOB */
	RCC-> AHBENR |= GPIOBEN;

	/* Set GPIOA 5, 6, 7 to AF Mode */
	// Pin5
	GPIOA->MODER |=  (1U<<11);
	GPIOA->MODER &= ~(1U<<10);

	// Pin6
	GPIOA->MODER |=  (1U<<13);
	GPIOA->MODER &= ~(1U<<12);

	// Pin7
	GPIOA->MODER |=  (1U<<15);
	GPIOA->MODER &= ~(1U<<14);

	/* Set GPIOB Pin 6 as Output */
	GPIOB->MODER |=  (1U<<12);
	GPIOB->MODER &= ~(1U<<13);

	/* Set GPIOA 5, 6, 7 to AFIO 5 */
	// Pin5
	GPIOA->AFR[0] &= ~(1U<<23);
	GPIOA->AFR[0] |=  (1U<<22);
	GPIOA->AFR[0] &= ~(1U<<21);
	GPIOA->AFR[0] |=  (1U<<20);

	// Pin6
	GPIOA->AFR[0] &= ~(1U<<27);
	GPIOA->AFR[0] |=  (1U<<26);
	GPIOA->AFR[0] &= ~(1U<<25);
	GPIOA->AFR[0] |=  (1U<<24);

	// Pin7
	GPIOA->AFR[0] &= ~(1U<<31);
	GPIOA->AFR[0] |=  (1U<<30);
	GPIOA->AFR[0] &= ~(1U<<29);
	GPIOA->AFR[0] |=  (1U<<28);

}

void spi1_config(void)
{
	/* Enable Clock access on SPI1 */
	SPI1->CR1 &= ~(1U<<5);
	SPI1->CR1 &= ~(1U<<4);
	SPI1->CR1 |= (1U<<3);

	/* Set clock polarity and phase */
	SPI1->CR1 |= (1U<<0);
	SPI1->CR1 |= (1U<<1);

	/* Enable full duplex mode */
	SPI1->CR1 &= ~(1U<<10);

	/* Set MSB first */
	SPI1->CR1 &= ~(1U<<7);

	/* Set master mode */
	SPI1->CR1 |= (1U<<2);

	/* Set data size */
	SPI1->CR1 &= ~(1U<<11);

	/* Configure software slave management */
	SPI1->CR1 |= (1U<<9);

	/* Configure internal slave select */
	SPI1->CR1 |= (1U<<8);

	/* SPI Enable */
	SPI1->CR1 |= (1U<<6);

}
