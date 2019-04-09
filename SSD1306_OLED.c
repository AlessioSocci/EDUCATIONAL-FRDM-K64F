/*
 * SSD1306_OLED.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#include "SSD1306_OLED.h"

// OLED 128 x 64 Driver SSD1306
uint8_t SSD1306_buffer [SSD1306_bufferLenght];
uint8_t SSD1306_state = 0;
uint8_t SSD1306_data = 0;

uint8_t SSD1306_xPosition = 0;
uint8_t SSD1306_yPosition = 0;

static uint16_t icrTab[] =
{
	/* 00 */   20, /* 01 */   22, /* 02 */   24, /* 03 */   26,
	/* 04 */   28, /* 05 */   30, /* 06 */   34, /* 07 */   40,
	/* 08 */   28, /* 09 */   32, /* 0A */   36, /* 0B */   40,
	/* 0C */   44, /* 0D */   48, /* 0E */   56, /* 0F */   68,

	/* 10 */   48, /* 11 */   56, /* 12 */   64, /* 13 */   72,
	/* 14 */   80, /* 15 */   88, /* 16 */  104, /* 17 */  128,
	/* 18 */   80, /* 19 */   96, /* 1A */  112, /* 1B */  128,
	/* 1C */  144, /* 1D */  160, /* 1E */  192, /* 1F */  240,

	/* 20 */  160, /* 21 */  192, /* 22 */  224, /* 23 */  256,
	/* 24 */  288, /* 25 */  320, /* 26 */  384, /* 27 */  480,
	/* 28 */  320, /* 29 */  384, /* 2A */  448, /* 2B */  512,
	/* 2C */  576, /* 2D */  640, /* 2E */  768, /* 2F */  960,

	/* 30 */  640, /* 31 */  768, /* 32 */  896, /* 33 */ 1024,
	/* 34 */ 1152, /* 35 */ 1280, /* 36 */ 1536, /* 37 */ 1920,
	/* 38 */ 1280, /* 39 */ 1536, /* 3A */ 1792, /* 3B */ 2048,
	/* 3C */ 2304, /* 3D */ 2560, /* 3E */ 3072, /* 3F */ 3840,
};

void I2C0_IRQHandler(void)
{
	if (SSD1306_state == 1)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_SETCOLUMNADDRESS; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_xPosition; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D =  SSD1306_SETPAGEADDRESS ; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_yPosition; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag
		}

		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}

	if (SSD1306_state == 2)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			I2C0->D = SSD1306_sendOneData;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = 0xFF; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag
		}
		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}


	if (SSD1306_state == 10)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_DISPLAYOFF; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_SETMUXRATIO; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = 0x3F; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_SETDISPLAYOFFSET; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = 0x00; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_SETDISPLAYSTARTLINE; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_SETADDRESSINGMODE; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = 0x00; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_SETCONTRAST; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = 0xFF; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_DISPLAYNORMAL; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_CHARGEPUMP; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = 0x14; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_sendOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_DISPLAYON; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

//			I2C0->D = SSD1306_sendOneCommand;
//			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete
//
//			I2C0->D = SSD1306_DISPLAYALLON; // Send Instruction
//			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag
		}
		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}

	if (SSD1306_state == 20)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			for (uint16_t i = 0; i < (SSD1306_bufferLenght - 1); ++i)
			{
				SSD1306_data = SSD1306_buffer[i];

				I2C0->D = SSD1306_sendMoreThanOneData;
				while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

				I2C0->D = SSD1306_data; // Send Instruction
				while((I2C0->S & (1 << 7)) == 0); // wait transfer complete
			}

			SSD1306_data = SSD1306_buffer[SSD1306_bufferLenght];

			I2C0->D = SSD1306_sendOneData;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_data; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag

			return;
		}
		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}
}

void I2C_sendStart(uint8_t SSD1306_stateSelect)
{
	SSD1306_state = SSD1306_stateSelect;

	delay(15);

	I2C0->C1 |= (1 << 5) | (1 << 4); // Start
	I2C0->D = 0x3C << 1; // Send Slave Address 0x3C << 1

	uint32_t timeOut = ticks + 100;
	while (timeOut > ticks && (I2C0->S & (1 << 7)) == 0 ); // wait transfer complete
}

void SSD1306_flush (void)
{
	delay(50);

	I2C_sendStart(20);
}

void SSD1306_clearAllPixel(void)
{
	delay(50);

	for(uint16_t i = 0; i < SSD1306_bufferLenght; ++i)
	{
		SSD1306_buffer[i] = 0;
	}
}

void SSD1306_drawPixel (uint8_t xPos, uint8_t yPos)
{
	uint16_t pos = xPos + ((yPos / 8) * 128);

	delay(50);

	SSD1306_buffer[pos] |= (1 << (yPos % 8));
}

void SSD1306_drawPixel2 (uint8_t xPos, uint8_t yPos)
{
	delay(50);

	SSD1306_xPosition = xPos;

	delay(50);

	SSD1306_yPosition = yPos / 8;

	delay(50);

	I2C_sendStart(1);

	delay(50);

	I2C_sendStart(2);
}


void SSD1306_drawPicture (uint8_t bitmap[SSD1306_bufferLenght])
{
	for (uint16_t i = 0; i < SSD1306_bufferLenght; ++i)
	{
		SSD1306_buffer[i] = bitmap[i];
	}

	SSD1306_flush();
}

void SSD1306_init(uint32_t inputBaudRate)
{
	// I2C (OLED 128x64 driver SSD1306)

	delay(100);

	PORTB->PCR[2] |= (1 << 9); // I2C SCL (alt2 mode)
	PORTB->PCR[3] |= (1 << 9); // I2C SDA (alt2 mode)

	NVIC->IP[24] = (1 << 4); // Preemptive Priority (1) same that PIT, in I2C transfer operation PIT ISR (every 1 ms !!) must not interfere

	SIM->SCGC4 |= (1 << 6); // CLock on I2C0 Module

	uint32_t newError;
	uint32_t oldError = 0xFFFF;
	uint8_t resultIcr = 0xFF;
	uint8_t icrTabValue = 0;
	uint32_t resultBaudRate = 0;

	for(icrTabValue = 0; icrTabValue < 64; icrTabValue++) // compute icr value
	{
		resultBaudRate = 60000000 / icrTab[icrTabValue]; // supposing mul fixed to 0

		newError = inputBaudRate - resultBaudRate;

		if(newError < oldError)
		{
			oldError = newError;
			resultIcr = icrTabValue;
		}

		else if(newError > oldError)
		{
			// wrong inputBaudRate
		}
	}

	uint8_t regValue = resultIcr & 0b00111111;

	I2C0->F = regValue; // Set Baud Rate

//	I2C0->F = 0b00110000; // Set Baud Rate

	I2C0->C1 |= (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4); // I2C0 Enable, Interrupt Enable, Master Mode, TX Mode

	NVIC->ISER[0] |= (1 << 24);

	delay (100);

	I2C_sendStart(10);

	delay (100);

	SSD1306_clearAllPixel();

	delay (100);

	SSD1306_flush();
}
