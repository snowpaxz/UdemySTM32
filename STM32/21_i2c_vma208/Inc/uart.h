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

void dma1_stream7_init(uint32_t src, uint32_t dst, uint32_t len);

#define CH7_GLOBAL_ICLR (1U<<24)
#define CH7_TERR_ICLR	(1U<<27)
#define CH7_TH_ICLR		(1U<<26)
#define CH7_TDONE_ICLR	(1U<<25)

#define DMA_TDONE_IEN	(1U<<1)
#define DMA_THALF_IEN	(1U<<2)
#define DMA_TERR_IEN	(1U<<3)

#endif /* UART_H_ */
