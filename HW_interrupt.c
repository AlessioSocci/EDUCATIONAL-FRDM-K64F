/*
 * HW_interrupt.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */
#include "HW_interrupt.h"

void PORTB_IRQHandler(void)
{
	PORTB->PCR[11] |= (1 << 24);
	GPIOB->PCOR |= (1 << 22);
	time_delay(200);
	GPIOB->PSOR |= (1 << 22);
}

void HW_interrupt_init(void)
{
    NVIC->IP[60] |= (1 << 5) | (1 << 4); // Preemptive Priority (3)

    PORTB->PCR[11] = (1 << 19) | (1 << 16) | (1 << 8) | (1 << 1) | (1 << 0) ; // 1 on 16 and 19 bits are for rising edge sense

    NVIC->ISER[1] |= (1 << 28);
}
