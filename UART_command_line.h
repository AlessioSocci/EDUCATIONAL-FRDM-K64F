/*
 * uart.h
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#ifndef UART_COMMAND_LINE_H_
#define UART_COMMAND_LINE_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "core_cm4.h"

#include "time.h"

extern uint8_t UARTdataIn;

void UART_init (void);
void UART_send (uint8_t dataOut);
void UART_get (uint8_t* dataIn);

#endif /* UART_COMMAND_LINE_H_ */
