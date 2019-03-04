/*
 * dac.h
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#ifndef DAC_H_
#define DAC_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "core_cm4.h"

#include "time.h"

void dac_init(void);
void dacOut (uint16_t value);

#endif /* DAC_H_ */
