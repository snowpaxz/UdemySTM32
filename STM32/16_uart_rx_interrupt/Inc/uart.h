/*
 * uart.h
 *
 *  Created on: Mar 3, 2024
 *      Author: lars
 */

#ifndef UART_H_
#define UART_H_

#include "stm32l1xx.h"

#include <stdint.h>

void uart2_rxtx_init();
void uart2_tx_init();
char uart2_read();
void uart2_rx_interrupt_init();

#define SR_RXNE		(1U<<5)

#endif /* UART_H_ */
