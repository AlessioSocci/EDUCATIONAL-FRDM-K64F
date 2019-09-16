/*
 * adc.h
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "core_cm4.h"

#include "time.h"

void adc_init(void);
uint16_t adc_read (void);

#endif /* ADC_H_ */
