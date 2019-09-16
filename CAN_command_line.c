/*
 * SN65HVD230.c
 *
 *  Created on: 08 apr 2019
 *      Author: Alessio
 */

#include <CAN_command_line.h>

enum cs_code_rx
{
    RX_MB_codeInactive = 0,
	RX_MB_codeFull = (1 << 25) | (1 << 24), // Full code is 0b0010
	RX_MB_codeEmpty = (1 << 26), // Empty code is 0b0100 (when a frame is received, code is update to Full)
	RX_MB_codeOverrun = (1 << 26) | (1 << 25), // Overrun code is 0b0110
	RX_MB_codeBusy = (1 << 27), // Busy code is 0b1000 (indicates that the Message Buffer is being updated)
	RX_MB_codeRanswer = (1 << 27) | (1 << 25), // Remote Answer code is 0b1010
};

enum cs_code_tx
{
	TX_MB_codeInactive = (1 << 27), // Deactivate code is 0b1000
    TX_MB_codeAbort = (1 << 27) | (1 << 24), // Abort code is 0b1001
    TX_MB_codeDataOrRemote = (1 << 27) | (1 << 26), // DataOrRemote code is 0b1100
    TX_MB_codeTanswer = (1 << 27) | (1 << 26) | (1 << 25), // T answer code is 0b1110

	TX_MB_codeExtend = (1 << 22) | (1 << 21), // code for extended frame
	TX_MB_codeRemote = (1 << 16), // code for remote frame
};

void CAN_init(void)
{
	SIM->SCGC6 |= (1 << 4); // Clock Enable on CAN module

	PORTB->PCR[18] |= (1 << 9); // In ALT 2 Mode is CAN TX
	PORTB->PCR[19] |= (1 << 9); // In ALT 2 Mode is CAN RX

	CAN0->MCR |= (1 << 31); // CAN Disable (making sure...)

	CAN0->CTRL1 &= ~(1 << 13); // Set oscillator as clock source of CAN engine

	CAN0->MCR &= !(1 << 31); // CAN Enable
	while ((CAN0->MCR & (1 << 20)) == 1); // Wait ACK of exit low power mode

	CAN0->MCR |= (1 << 25); // Soft Reset
	while ((CAN0->MCR & (1 << 25)) == 1); // Wait reset

	CAN0->MCR |= (1 << 30) | (1 << 28); // Enter in Freeze mode and Halt mode
	while ((CAN0->MCR & (1 << 24)) == 0); // Wait ACK of Freeze mode

	CAN0->MCR |= (1 << 17) | (1 << 16) | (1 << 0); // self reception disable, Individual RX mask enabled, number of last Message Buffer set as 1

//	CAN0->CTRL1 |= (1 << 5) | (1 << 4); // Enable Timer Sync, lowest number buffer first

	CAN0->CTRL1 |= (1 << 27) | (1 << 26) | (1 << 24) | (1 << 21 ) | (1 << 20) | (1 << 19) | (1 << 18) | (1 << 17) | (1 << 16); //| (1 << 2) | (1 << 1) | (1 << 0);

	// Re-sync Jump Width = RJW + 1
	// Phase Buffer Segment 1 = (PSEG1 + 1) × Time-Quanta
	// Phase Buffer Segment 2 = (PSEG2 + 1) × Time-Quanta
	// Propagation Segment Time = (PROPSEG + 1) × Time-Quanta, where Time-Quantum = one S clock period

	// Time Quanta = 1 + (PSEG1 + 1) + (PSEG2 + 1) + (PROPSEG + 1)
	// PRESCALER DIV = (Source CLock / BPS * Time Quanta) + 1

	// to select 200kbps :

    // ---> Time QUanta = 1 + (7 + 1) + (7 + 1) + (7 + 1) = 25
	// ---> PRESCALER DIV = (60000000 / 200000 * 25) + 1 = ---> 13 <---



	CAN0->MCR &= !(1 << 30); // Exit of Freeze mode and Halt mode
	while ((CAN0->MCR & (1 << 24)) == 1); // Wait ACK of Freeze mode disable
}

void CAN_readRX_MB(uint8_t MB_number, uint8_t* cs, uint32_t* id, uint32_t* word0, uint32_t* word1)
{
	*cs = CAN0->MB[MB_number].CS; // Read control status field

	uint8_t rx_code = (*cs & 0xF000000) >> 24; // extract Rx code from control status field

	if ((RX_MB_codeFull == rx_code) || (RX_MB_codeOverrun == rx_code))
	{
		*id = CAN0->MB[MB_number].ID;
		*word0 = CAN0->MB[MB_number].WORD0;
		*word1 = CAN0->MB[MB_number].WORD1;
	}
	else
	{
		uint32_t dummyReadOfFreeRunningTimer = CAN0->TIMER;
	}
}

void CAN_writeTX_MB(uint8_t MB_number, uint32_t id, uint32_t word0, uint32_t word1, TxFrame data_features)
{
	uint32_t cs_temp = 0;

	if (data_features.data_format == extended)
	{
		cs_temp |= TX_MB_codeExtend;
	}

	if (data_features.data_type == remote)
	{
		cs_temp |= TX_MB_codeRemote;
	}

	if(CAN0->MB[MB_number].CS = CAN0->MB[MB_number].CS & ~(0xF000000U) || TX_MB_codeInactive)
	{
		CAN0->MB[MB_number].ID = id;

		CAN0->MB[MB_number].WORD0 = word0;
		CAN0->MB[MB_number].WORD1 = word1;

		CAN0->MB[MB_number].CS = cs_temp | TX_MB_codeDataOrRemote | (data_features.data_length << 16);
	}

	time_delay (1000);
	CAN0->MB[MB_number].ID = 0;

	CAN0->MB[MB_number].WORD0 = 0;
	CAN0->MB[MB_number].WORD1 = 0;

	CAN0->MB[MB_number].CS = TX_MB_codeInactive;
}



