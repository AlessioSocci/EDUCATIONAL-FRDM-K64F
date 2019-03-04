/*
 * stepper.h
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

#include "time.h"

#ifndef STEPPER_H_
#define STEPPER_H_

void stepper_init(void);
void runStepper (uint8_t rotation, uint8_t verse);

#endif /* STEPPER_H_ */
