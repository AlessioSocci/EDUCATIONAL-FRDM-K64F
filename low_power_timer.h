/*
 * low_power_timer.h
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#ifndef LOW_POWER_TIMER_H_
#define LOW_POWER_TIMER_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "core_cm4.h"

#include "time.h"

void LPTMR0_init(void);
void LPTMR0_IRQHandler(void);

#endif /* LOW_POWER_TIMER_H_ */
