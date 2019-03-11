/*
 * uart.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */
#include <uart_command_line.h>

uint8_t UARTdataIn = 0;

void uart_init (void)
{
	// UART
    PORTC->PCR[16] |= (1 << 9) | (1 << 8); // PTC 16 in Alt3 Mode UART3 RX
    PORTC->PCR[17] |= (1 << 9) | (1 << 8); // PTC 17 in Alt3 Mode UART3 TX

    SIM->SCGC4 |= (1 << 13); // UART3 Clock Enable

    UART3->C2 &= ~((1 << 3) | (1 << 2)); // Disable TX and RX while in setting
    UART3->C2 |= (1 << 5); // Receiver Full Interrupt Enable

    UART3->BDH |= (1 << 0); 						// Set 9600 Baud Rate SBR are the first 5 Bit in UART3->BDH Register
   	UART3->BDL |= (1 << 7) | (1 << 2) | (1 << 1); 	// Set 9600 Baud Rate SBR are the 8 Bit in UART3->BDL Register (Tot 13 Bit between BDH and BDL)
   													// SBR = clockHz / (baudrate * 16) (UART0 and UART1, clock is system clock, otherwise is bus clock, 60 MHz for K64)
   													// SBR for 9600 BR is 390, so 0000110000110, 00001 in BDH, 01101000 in BDL
   	UART3->C2 |= (1 << 3) | (1 << 2); // Enable TX and RX while in setting

   	NVIC->IP[37] = (1 << 13) | (1 << 12); // Preemptive Priority (3)
   	NVIC->ISER[1] |= (1 << 5);
}

void UART3_RX_TX_IRQHandler(void)
{
	uint8_t dummyRead = UART3->S1; // For clear interrupt flag
	UARTdataIn = UART3->D; // For clear interrupt flag
}

void UART_send (uint8_t dataOut)
{
	 while (!((UART3->S1) & (1 << 7)))
	 {

	 }
	 UART3->D = dataOut;
}

void UART_get (uint8_t* dataIn)
{
	uint32_t timeOut = ticks + 200;
	while (timeOut > ticks)
	{
		if ((UART3->S1) & (1 << 5))
		{
			while(!((UART3->S1) & (1 << 5)));
//			*dataIn = UART3->D ; // switch contest and read data into ISR.
		}
	}
}
