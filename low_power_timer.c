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
    PORTB->PCR[21] |= (1 << 8);	// red led enable
    GPIOB->PDDR |= (1 << 21);
    GPIOB->PSOR |= (1 << 21);

    NVIC->IP[58] = (1 << 5) | (1 << 4); // Preemptive Priority

    SIM->SCGC5 |= (1 << 0);

    LPTMR0->PSR = (1 << 2) | (1 << 0);
    LPTMR0->CMR = 90000000;
    LPTMR0->CSR = (1 << 0) | (1 << 6); // Timer enable and interrupt enable

    NVIC->ISER[1] |= (1 << 26) ;
}

void LPTMR0_IRQHandler(void)
{
	GPIOB->PCOR |= (1 << 21); // Blue Led ON

	for (int i = 0; i < 1000000; i++);

	GPIOB->PSOR |= (1 << 21); // Blue Led OFF

	LPTMR0->CSR |= (1 << 7);
}
