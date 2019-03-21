/*
 * time.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#include "time.h"

// Time and FSM
uint32_t ticks = 0;
uint16_t task1 = 0;
uint16_t task2 = 0;
uint16_t time1 = 1000; // threshold of counter in ms
uint16_t time2 = 1000; // threshold of counter in ms

uint8_t task1Flag = 0;
uint8_t task2Flag = 0;


void task1_incrementCounter(void)
{
   	task1++;
	if (task1 == time1)
	{
		task1 = 0;
		task1Flag = 1;
	}
}
void task2_incrementCounter(void)
{
   	task2++;
	if (task2 == time2)
	{
		task2 = 0;
		task2Flag = 1;
	}
}
void PIT0_IRQHandler(void)
{
	ticks++;
	task1_incrementCounter();
	task2_incrementCounter();
	PIT->CHANNEL->TFLG |= (1 << 0);
}

void delay (uint16_t mseconds) // Delay function, based on pit
{
    uint32_t currTicks = ticks;

    while ((ticks - currTicks) < mseconds);
}

void pit_init(void)
{
	// PIT
   SIM->SCGC6 |= (1 << 23); // Clock on PIT0

   NVIC->IP[48] = (1 << 4); ///////////////////// PREEMPTIVE PRIORITY

   PIT->MCR = 0;

   PIT->CHANNEL->LDVAL = 0x0000C49B; // 1ms

   PIT->CHANNEL->TCTRL |= (1 << 0) | (1 << 1);

   NVIC->ISER[1] |= (1<<16);
}