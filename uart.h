/*
 * uart.h
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "core_cm4.h"

#include "time.h"

extern uint8_t UARTdataIn;

void uart_init (void);
void UART3_RX_TX_IRQHandler(void);
void UART_send (uint8_t dataOut);
void UART_get (uint8_t* dataIn);

#endif /* UART_H_ */
