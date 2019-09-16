/*
 * pwm.h
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#ifndef PWM_H_
#define PWM_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "core_cm4.h"

#include "time.h"

void pwm_init(void);

#endif /* PWM_H_ */
