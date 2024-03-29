// Where is the LED connected?
// Port: A
// Pin:  5

#define PERIPH_BASE 		(0x40000000UL)

#define GPIOA_OFFSET   		(0x00020000UL)
#define GPIOA_BASE			(PERIPH_BASE + GPIOA_OFFSET)

#define RCC_OFFSET			(0x00023800UL)
#define RCC_BASE			(PERIPH_BASE + RCC_OFFSET)

#define AHBEN_R_OFFSET		(0x1CUL)
#define RCC_AHBEN_R 		(*(volatile unsigned int *)(RCC_BASE + AHBEN_R_OFFSET))

#define MODE_R_OFFSET		(0x00UL)
#define GPIOA_MODE_R		(*(volatile unsigned int *)(GPIOA_BASE + MODE_R_OFFSET))

#define OD_R_OFFSET			(0x14UL)
#define GPIOA_OD_R			(*(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET))

#define GPIOAEN				(1U<<0)

#define PIN5				(1U<<5)
#define LED_PIN				PIN5

/*
 *
 */

int main(void)
{
	/* 1. Enable clock access to GPIOA */
	RCC_AHBEN_R |= GPIOAEN;

	/* 2. Set PA5 as output pin */
	GPIOA_MODE_R &= ~(1U<<11);   	// Set bit 11 to 0 */
	GPIOA_MODE_R |=  (1U<<10);  		// Set bit 10 to 1

	while(1)
	{
		/* Set PA5 HIGH */
		//GPIOA_OD_R |= LED_PIN;

		/* Toggle PA5 */
		GPIOA_OD_R ^= LED_PIN;
		for(int i = 0; i < 100000; i++)
		{}
	}
}
