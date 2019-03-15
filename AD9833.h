/*
 * AD9833.h
 *
 *  Created on: 04 mar 2019
 *      Author: Alessio
 */

#ifndef AD9833_H_
#define AD9833_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "math.h"

#include "time.h"

// CONTROL WORD (bit 16 and 15 are 0)
#define AD9833_CONTROL_FREQ0_SELECT				0b0000000000000000
#define AD9833_CONTROL_FREQ1_SELECT				0b0000010000000000
#define AD9833_CONTROL_PHASE0_SELECT			0b0000000000000000
#define AD9833_CONTROL_PHASE1_SELECT			0b0000001000000000

#define AD9833_CONTROL_MSB_BOTH_LSB_FREQ_REG    0b0010000000000000 // + another write with bit13 to bit0 that means freq.; Need two consecutive writes to 28 bit resolution set freq.
#define AD9833_CONTROL_MSB_FREQ_REG         	0b0001000000000000 // + another write with bit13 to bit0 that means freq.
#define AD9833_CONTROL_LSB_FREQ_REG          	0b0000000000000000 // + another write with bit13 to bit0 that means freq.

#define AD9833_CONTROL_DAC_TO_VOUT_ON			0b0000000000000000
#define AD9833_CONTROL_DAC_TO_VOUT_OFF			0b0000000000100000

#define AD9833_CONTROL_DAC_OUT_IS_SIN			0b0000000000000000
#define AD9833_CONTROL_DAC_OUT_IS_TRI			0b0000000000000010

#define AD9833_CONTROL_DATA_MSB_DIV2_ON			0b0000000000001000
#define AD9833_CONTROL_DATA_MSB_DIV2_OFF		0b0000000000000000

#define AD9833_CONTROL_RESET_ON				  	0b0000001000000000
#define AD9833_CONTROL_RESET_OFF				0b0000000000000000

// DATA WORD
#define AD9833_WRITE_FREQ0_REG					0b0100000000000000 // where the last 14 bit are freq0 value, if 28 bit selected, first write for LSB, second for MSB, else depend by bit 13 value in control word
#define AD9833_WRITE_FREQ1_REG					0b1000000000000000 // where the last 14 bit are freq1 value, if 28 bit selected, first write for LSB, second for MSB, else depend by bit 13 value in control word
#define AD9833_WRITE_PHASE0_REG					0b1100000000000000 // Bit 13 dont'care, last 12 bit are pahase0 value.
#define AD9833_WRITE_PHASE1_REG					0b1110000000000000 // Bit 13 dont'care, last 12 bit are pahase1 value.

// SOME USEFULL MASK
#define AD9833_WRITE_PHASE_MASK 				0b0000111111111111

typedef enum
{
	SQUARE =		1,
	TRI =			2,
	SIN = 			3
} WaveForm;

typedef enum
{
	FREQ0 =			1,
	FREQ1 =			2
} Freq0_1;


void AD9833_init(void);
void AD9833_firstStart(uint8_t selectFreq0_1, uint32_t freq, uint16_t phase, WaveForm c);
void AD9833_write (uint16_t dataOut);


#endif /* AD9833_H_ */
