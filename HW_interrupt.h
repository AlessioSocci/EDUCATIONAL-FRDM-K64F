/*
 * HW_interrupt.h
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#ifndef HW_INTERRUPT_H_
#define HW_INTERRUPT_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "core_cm4.h"

#include "time.h"

void HW_interrupt_init(void);
void PORTB_IRQHandler(void);

#endif /* HW_INTERRUPT_H_ */
