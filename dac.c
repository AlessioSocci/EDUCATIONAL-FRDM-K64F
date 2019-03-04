/*
 * dac.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */
#include "dac.h"

void dac_init(void)
{
	// DAC
	SIM->SCGC2 |= (1 << 12); // Clock on DAC0
    DAC0->C0 |= (1 << 7) | (1 << 6); // Enable DAC, reference is VDDA (DACREF2)
}

void dacOut (uint16_t value)
{
	uint8_t msb = 0;
	uint8_t lsb = 0;

	if(value < 4096)
	{
		msb = (uint8_t)(value >> 8);
		lsb = (uint8_t)(value);

		DAC0->DAT[0].DATH = msb;
		DAC0->DAT[0].DATL = lsb;
	}
}
