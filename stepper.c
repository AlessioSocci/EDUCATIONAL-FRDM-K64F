/*
 * stepper.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#include "stepper.h"

void stepper_init(void)
{
//	 H BRIDGE & STEPPER
	PORTB->PCR[23] |= (1 << 8);	//  IN1
	GPIOB->PDDR |= (1 << 23);
	GPIOB->PSOR |= (1 << 23);
	GPIOB->PCOR |= (1 << 23);

	PORTB->PCR[9] |= (1 << 8);	//  IN2
	GPIOB->PDDR |= (1 << 9);
	GPIOB->PSOR |= (1 << 9);
	GPIOB->PCOR |= (1 << 9);

	PORTC->PCR[2] |= (1 << 8);	//  IN3
	GPIOC->PDDR |= (1 << 2);
	GPIOC->PSOR |= (1 << 2);
	GPIOC->PCOR |= (1 << 2);

	PORTC->PCR[3] |= (1 << 8);	//  IN4
	GPIOC->PDDR |= (1 << 3);
	GPIOC->PSOR |= (1 << 3);
	GPIOC->PCOR |= (1 << 3);
}
void runStepper (uint8_t rotation, uint8_t verse) // 1 anti-clockwise, 2 clockwise
{
	for (int i=0; i<(rotation * 12); i++)
	{
		switch (verse)
		{
			case  1:
			// step 1
			GPIOB->PCOR |= (1 << 23); // IN1
			GPIOB->PSOR |= (1 << 9); // IN2 *
			GPIOC->PSOR |= (1 << 2); // IN3 *
			GPIOC->PCOR |= (1 << 3); // IN4
			delay(20);
			// step 2
			GPIOB->PCOR |= (1 << 23); // IN1
			GPIOB->PSOR |= (1 << 9); // IN2 *
			GPIOC->PCOR |= (1 << 2); // IN3
			GPIOC->PSOR |= (1 << 3); // IN4 *
			delay(20);
			// step 3
			GPIOB->PSOR |= (1 << 23); // IN1 *
			GPIOB->PCOR |= (1 << 9); // IN2
			GPIOC->PCOR |= (1 << 2); // IN3
			GPIOC->PSOR |= (1 << 3); // IN4 *
			delay(20);
			// step 4
			GPIOB->PSOR |= (1 << 23); // IN1 *
			GPIOB->PCOR |= (1 << 9); // IN2
			GPIOC->PSOR |= (1 << 2); // IN3 *
			GPIOC->PCOR |= (1 << 3); // IN4
			delay(20);
			break;

			case 2:
			// step 4
			GPIOB->PSOR |= (1 << 23); // IN1 *
			GPIOB->PCOR |= (1 << 9); // IN2
			GPIOC->PSOR |= (1 << 2); // IN3 *
			GPIOC->PCOR |= (1 << 3); // IN4
			delay(20);
			// step 3
			GPIOB->PSOR |= (1 << 23); // IN1 *
			GPIOB->PCOR |= (1 << 9); // IN2
			GPIOC->PCOR |= (1 << 2); // IN3
			GPIOC->PSOR |= (1 << 3); // IN4 *
			delay(20);
			// step 2
			GPIOB->PCOR |= (1 << 23); // IN1
			GPIOB->PSOR |= (1 << 9); // IN2 *
			GPIOC->PCOR |= (1 << 2); // IN3
			GPIOC->PSOR |= (1 << 3); // IN4 *
			delay(20);
			// step 1
			GPIOB->PCOR |= (1 << 23); // IN1
			GPIOB->PSOR |= (1 << 9); // IN2 *
			GPIOC->PSOR |= (1 << 2); // IN3 *
			GPIOC->PCOR |= (1 << 3); // IN4
			delay(20);
			break ;
		}
	}
}
