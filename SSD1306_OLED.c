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

static uint16_t icrTab[] = {20, 22, 24, 26, 28, 30, 34, 40, 28, 32, 36, 40, 44, 48, 56, 68, 48, 56, 64, 72, 80, 88, 104, 128, 80, 96, 112, 128,
	144, 160, 192, 240, 160, 192, 224, 256, 288, 320, 384, 480, 320, 384, 448, 512, 576, 640, 768, 960, 640, 768, 896, 1024, 1152, 1280, 1536,
	1920, 1280, 1536,  1792, 2048, 2304, 2560, 3072, 3840};

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

	delay (10);

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
