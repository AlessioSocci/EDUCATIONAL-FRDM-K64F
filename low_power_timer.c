/*
 * low_power_timer.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */
#include "low_power_timer.h"

void LPTMR0_init(void)
{
  // LPTMR
    NVIC->IP[58] = (1 << 6); // Preemptive Priority (4), the lowest priority for this application

    SIM->SCGC5 |= (1 << 0);

    LPTMR0->PSR = (1 << 5) | (1 << 4) | (1 << 3); // Prescaler divides the prescaler clock by 16
    LPTMR0->CMR = 65535;
    LPTMR0->CSR = (1 << 0) | (1 << 6); // Timer enable and interrupt enable

    NVIC->ISER[1] |= (1 << 26) ;
}

void LPTMR0_IRQHandler(void)
{
	LPTMR0->CSR |= (1 << 7);
	GPIOB->PCOR |= (1 << 21);
	time_delay (1000);
	GPIOB->PSOR |= (1 << 21);
}
