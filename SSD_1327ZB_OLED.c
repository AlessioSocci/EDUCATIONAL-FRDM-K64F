/*
 * 1327ZB.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#include <SSD_1327ZB_OLED.h>

// OLED 128 x 128 Driver SSD1327ZB
uint8_t SSD1327ZB_buffer [SSD1327ZB_bufferLenght];

void SSD1327ZB_sendCommand (uint8_t value)
{
	 GPIOE->PSOR |= (1 << 26); // RS
	 GPIOE->PCOR |= (1 << 24); // CS
	 GPIOC->PSOR |= (1 << 10); // WR
	 GPIOE->PCOR |= (1 << 25); // DC
	 GPIOC->PCOR |= (1 << 10); // WR

     ((value & 0x01) > 0) ? (GPIOC->PSOR |= (1 << 5)) : (GPIOC->PCOR |= (1 << 5)); // D0
     ((value & 0x02) > 0) ? (GPIOC->PSOR |= (1 << 7)) : (GPIOC->PCOR |= (1 << 7)); // D1
     ((value & 0x04) > 0) ? (GPIOC->PSOR |= (1 << 0)) : (GPIOC->PCOR |= (1 << 0)); // D2
     ((value & 0x08) > 0) ? (GPIOC->PSOR |= (1 << 9)) : (GPIOC->PCOR |= (1 << 9)); // D3
     ((value & 0x10) > 0) ? (GPIOC->PSOR |= (1 << 8)) : (GPIOC->PCOR |= (1 << 8)); // D4
     ((value & 0x20) > 0) ? (GPIOC->PSOR |= (1 << 1)) : (GPIOC->PCOR |= (1 << 1)); // D5
     ((value & 0x40) > 0) ? (GPIOB->PSOR |= (1 << 19)) : (GPIOB->PCOR |= (1 << 19)); //D6
     ((value & 0x80) > 0) ? (GPIOB->PSOR |= (1 << 18)) : (GPIOB->PCOR |= (1 << 18));  //D7

     GPIOC->PSOR |= (1 << 10);  // WR
     GPIOE->PSOR |= (1 << 24); // CS
}
void SSD1327ZB_flush (void)
{
	SSD1327ZB_sendCommand(SSD1327ZB_CMD_SETCOLUMNADDR);
	SSD1327ZB_sendCommand(0);
	SSD1327ZB_sendCommand(63);

	SSD1327ZB_sendCommand(SSD1327ZB_CMD_SETROWADDR);
	SSD1327ZB_sendCommand(0);
	SSD1327ZB_sendCommand(127);

	for (uint16_t i = 0; i < SSD1327ZB_bufferLenght; i++)
	{
		GPIOE->PSOR |= (1 << 26); // RS
		GPIOE->PCOR |= (1 << 24); // CS
		GPIOC->PSOR |= (1 << 10); // WR
		GPIOE->PSOR |= (1 << 25); // DC
		GPIOC->PCOR |= (1 << 10); // WR

		((SSD1327ZB_buffer[i] & 0x01) > 0) ? (GPIOC->PSOR |= (1 << 5)) : (GPIOC->PCOR |= (1 << 5)); // D0
		((SSD1327ZB_buffer[i] & 0x02) > 0) ? (GPIOC->PSOR |= (1 << 7)) : (GPIOC->PCOR |= (1 << 7)); // D1
		((SSD1327ZB_buffer[i] & 0x04) > 0) ? (GPIOC->PSOR |= (1 << 0)) : (GPIOC->PCOR |= (1 << 0)); // D2
		((SSD1327ZB_buffer[i] & 0x08) > 0) ? (GPIOC->PSOR |= (1 << 9)) : (GPIOC->PCOR |= (1 << 9)); // D3
		((SSD1327ZB_buffer[i] & 0x10) > 0) ? (GPIOC->PSOR |= (1 << 8)) : (GPIOC->PCOR |= (1 << 8)); // D4
		((SSD1327ZB_buffer[i] & 0x20) > 0) ? (GPIOC->PSOR |= (1 << 1)) : (GPIOC->PCOR |= (1 << 1)); // D5
		((SSD1327ZB_buffer[i] & 0x40) > 0) ? (GPIOB->PSOR |= (1 << 19)) : (GPIOB->PCOR |= (1 << 19)); //D6
		((SSD1327ZB_buffer[i] & 0x80) > 0) ? (GPIOB->PSOR |= (1 << 18)) : (GPIOB->PCOR |= (1 << 18));  //D7

		GPIOC->PSOR |= (1 << 10);  // WR
		GPIOE->PSOR |= (1 << 24); // CS
	}
}

void SSD1327ZB_clear (void)
{
	for (uint16_t i = 0; i < SSD1327ZB_bufferLenght; i++)
	{
		SSD1327ZB_buffer[i]=0;
	}
}

void SSD1327ZB_drawPixel (uint8_t xPos, uint8_t yPos, uint8_t tone)
{
	uint16_t pos = ((uint16_t) xPos/2) + ((uint16_t) yPos * 128);

	if (xPos%2)
	{
		SSD1327ZB_buffer[pos] = (((tone << 4) & 0xF0) | (SSD1327ZB_buffer[pos] & 0x0F));
	}
	else
	{
		SSD1327ZB_buffer[pos] = ((tone & 0x0F) | (SSD1327ZB_buffer[pos] & 0xF0));
	}
}
void SSD1327ZB_init(void)
{
	// PARALLEL 8080 (OLED 128x128 driver SSD1327ZB)
	PORTC->PCR[10] = (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, Write input
	GPIOC->PDDR = (1 << 10);
	GPIOC->PCOR = (1 << 10);

	PORTB->PCR[19] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, D6
	GPIOB->PDDR |= (1 << 19);
	GPIOB->PCOR |= (1 << 19);

	PORTB->PCR[18] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, D7
	GPIOB->PDDR |= (1 << 18);
	GPIOB->PCOR |= (1 << 18);

	PORTC->PCR[9] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, D3
	GPIOC->PDDR |= (1 << 9);
	GPIOC->PCOR |= (1 << 9);

	PORTC->PCR[8] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, D4
	GPIOC->PDDR |= (1 << 8);
	GPIOC->PCOR |= (1 << 8);

	PORTC->PCR[7] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, D1
	GPIOC->PDDR |= (1 << 7);
	GPIOC->PCOR |= (1 << 7);

	PORTC->PCR[5] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, D0
	GPIOC->PDDR |= (1 << 5);
	GPIOC->PCOR |= (1 << 5);

	PORTC->PCR[4] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode,, Read input
	GPIOC->PDDR |= (1 << 4);
	GPIOC->PCOR |= (1 << 4);

	PORTC->PCR[1] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, D5
	GPIOC->PDDR |= (1 << 1);
	GPIOC->PCOR |= (1 << 1);

	PORTC->PCR[0] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, D2
	GPIOC->PDDR |= (1 << 0);
	GPIOC->PCOR |= (1 << 0);

	PORTE->PCR[26] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, Reset
	GPIOE->PDDR |= (1 << 26);
	GPIOE->PCOR |= (1 << 26);

	PORTE->PCR[25] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, Data - Command
	GPIOE->PDDR |= (1 << 25);
	GPIOE->PCOR |= (1 << 25);

	PORTE->PCR[24] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode,  Chip Select
	GPIOE->PDDR |= (1 << 24);
	GPIOE->PCOR |= (1 << 24);

	GPIOE->PSOR |= (1 << 25); // DC
	GPIOC->PSOR |= (1 << 10); // RD
	GPIOC->PSOR |= (1 << 4); // WR
	GPIOE->PSOR |= (1 << 24); // CS
	GPIOE->PSOR |= (1 << 26); // RS

	SSD1327ZB_sendCommand(SSD1327ZB_CMD_DISPLAYON);
	SSD1327ZB_sendCommand(SSD1327ZB_CMD_DISPLAYNORMAL);
	SSD1327ZB_sendCommand(SSD1327ZB_CMD_SEGMENTREMAP);
	SSD1327ZB_sendCommand(SSD1327ZB_REMAP_ODDEVEN_COM);
	SSD1327ZB_sendCommand(SSD1327ZB_CMD_STARTLINE);
	SSD1327ZB_sendCommand(0);
	SSD1327ZB_sendCommand(SSD1327ZB_CMD_DISPLAYALLON);

//	SSD1327ZB_clear ();
}
