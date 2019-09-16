/*
 * pwm.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */
#include "pwm.h"

void pwm_init(void)
{
	SIM->SCGC3 |= (1 << 25); // FTM3 Clock Gate Enable

	PORTC->PCR[11] |= (1 << 9) | (1 << 8); // PTC11 in Alt 3 Mode

	FTM3->SC |= (1 << 3) | (1 << 0) | (1 << 1) | (1 << 3); // FTM0 CLock Source is System Clock, Prescale Factor is 128
	FTM3->CONTROLS[7].CnSC |= (1 << 5) | (1 << 3); // PWM Mode Select, Center Allinie
	FTM3->CONTROLS[7].CnV = (1 << 15); // Set duty cycle


}
