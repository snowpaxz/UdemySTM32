// Where is the LED connected?
// Port: A
// Pin:  5

#include <stdint.h>

#define PERIPH_BASE 		(0x40000000UL)
#define GPIOA_OFFSET   		(0x00020000UL)
#define GPIOA_BASE			(PERIPH_BASE + GPIOA_OFFSET)

#define RCC_OFFSET			(0x00023800UL)
#define RCC_BASE			(PERIPH_BASE + RCC_OFFSET)

#define GPIOAEN				(1U<<0)

#define PIN5				(1U<<5)
#define LED_PIN				PIN5

#define __IO volatile

typedef struct
{
	volatile uint32_t DUMMY[7];
	volatile uint32_t AHBENR;	/*!< RCC AHB clock register					Address offset: 0x1C	*/

} RCC_Typedef;

typedef struct
{
	volatile uint32_t MODER; 	/*!< GPIO port mode register, 				Address offset: 0x00	*/
	volatile uint32_t DUMMY[4];
	volatile uint32_t ODR;		/*!< GPIO port output data register			Address offset: 0x14	*/

}GPIO_TypeDef;


#define RCC		((RCC_Typedef*) RCC_BASE)
#define GPIOA		((GPIO_TypeDef*) GPIOA_BASE)

int main(void)
{
	/* 1. Enable clock access to GPIOA */
	RCC->AHBENR |= GPIOAEN;

	/* 2. Set PA5 as output pin */
	GPIOA->MODER |=  (1U<<10);  		// Set bit 10 to 1
	GPIOA->MODER &= ~(1U<<11);   	// Set bit 11 to 0 */

	while(1)
	{
		/* Toggle PA5 */
		GPIOA->ODR ^= LED_PIN;
		for(int i = 0; i < 100000; i++)
		{}
	}
}
