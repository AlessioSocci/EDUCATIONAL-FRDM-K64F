/*
 * adc.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */
#include "adc.h"

void adc_init(void)
{
	  // ADC
	    PORTB->PCR[10] |= (1 << 1); // alt0 mode

	    SIM->SCGC3 |= (1 << 27); // Clock on adc1

	    ADC1->CFG1 |= (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6); // input clock is bus clock / 2 , 16 bit single-end mode, clock divide is 8
}

uint32_t adc_read (void)
{
	uint32_t dataADC = 0;
	ADC1->SC1[0] = (1 << 3) | (1 << 2) |  (1 << 1); // adc1 channel select, for B10 is ch14

	while((ADC1->SC1[0] & (1<<7)) == 0)
	{

	}
	dataADC = ADC1->R[0];
	dataADC = (dataADC * 3300) / 65535;
	return dataADC;
}
