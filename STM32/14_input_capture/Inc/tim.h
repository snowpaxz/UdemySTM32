/*
 * tim.h
 *
 *  Created on: Mar 10, 2024
 *      Author: lars
 */

#ifndef TIM_H_
#define TIM_H_


#define SR_UIF   (1U<<0)

void tim2_1hz_init(void);
void tim2_PA5_output_compare(void);
void tim3_pa6_input_capture(void);

#define SR_UIF 		(1U<<0)
#define SR_CC1IF 	(1U<<1)

#endif /* TIM_H_ */
