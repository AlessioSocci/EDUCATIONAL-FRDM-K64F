/*
 * time.h
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "core_cm4.h"

#ifndef TIME_H_
#define TIME_H_

// Time and FSM
extern uint32_t ticks;

extern uint8_t task1Flag;
extern uint8_t task2Flag;


void task1_incrementCounter(void);
void task2_incrementCounter(void);
void PIT0_IRQHandler(void);
void delay (uint16_t mseconds);
void pit_init(void);
#endif /* TIME_H_ */
