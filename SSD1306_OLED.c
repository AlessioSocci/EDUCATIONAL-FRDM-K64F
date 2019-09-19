/*
 * SSD1306_OLED.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#include <SSD1306_OLED.h>
#include "picture.h"

#define WAITING_TIME 	150

// OLED 128 x 64 Driver SSD1306
uint8_t SSD1306_buffer [SSD1306_bufferLenght];
uint8_t SSD1306_data = 0;

uint8_t SSD1306_xPositionStart = 0;
uint8_t SSD1306_xPositionStop = 0;
uint8_t SSD1306_yPositionStart = 0;
uint8_t SSD1306_yPositionStop = 0;

uint32_t currentTicks = 0;

static uint16_t icrTab[] = {20, 22, 24, 26, 28, 30, 34, 40, 28, 32, 36, 40, 44, 48, 56, 68, 48, 56, 64, 72, 80, 88, 104, 128, 80, 96, 112, 128,
	144, 160, 192, 240, 160, 192, 224, 256, 288, 320, 384, 480, 320, 384, 448, 512, 576, 640, 768, 960, 640, 768, 896, 1024, 1152, 1280, 1536,
	1920, 1280, 1536,  1792, 2048, 2304, 2560, 3072, 3840};

typedef enum _SSD1306_state
{
	init,
	display_on,
	display_off,
	set_xPosition_forFlushPart,
	set_yPosition_forFlushPart,
	flushPart,
}SSD1306_state;

SSD1306_state SSD1306state;


void I2C0_IRQHandler(void)
{
	if (SSD1306state == init)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_DISPLAYOFF; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETDISPLAYCLK; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0x80; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETMUXRATIO; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0x3F; // Send Instruction (display high - 1)
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETDISPLAYOFFSET; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0x00; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete



			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETDISPLAYSTARTLINE; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_CHARGEPUMP; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0x14; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETADDRESSINGMODE; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_HORIZONTALADDRESSINGMODE; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


//			time_delay(5);
//			currentTicks = ticks;
//			I2C0->D = SSD1306_sendMoreThanOneCommand;
//			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete
//
//			time_delay(5);
//			currentTicks = ticks;
//			I2C0->D = SSD1306_SEGMENTREMAP | 0x01; // Send Instruction
//			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


//			time_delay(5);
//			currentTicks = ticks;
//			I2C0->D = SSD1306_sendMoreThanOneCommand;
//			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete
//
//			time_delay(5);
//			currentTicks = ticks;
//			I2C0->D = SSD1306_COMSCANDIRECTIONDOWN; // Send Instruction
//			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_COMPINS; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0x12; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETCONTRAST; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0xFF; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETPRECHARGE; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0xF1; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_SETDESELECTLEVEL; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = 0x40; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_DISPLAYALLONRESUME; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete


			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_DISPLAYNORMAL; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag
		}
		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}

	if (SSD1306state == display_on)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_DISPLAYON; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag
		}

		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}

	if (SSD1306state == display_off)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_sendOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			currentTicks = ticks;
			I2C0->D = SSD1306_DISPLAYOFF; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			time_delay(5);
			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag
		}

		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}

	if (SSD1306state == set_xPosition_forFlushPart)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			currentTicks = ticks;
			I2C0->D = SSD1306_SETCOLUMNADDRESS; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			currentTicks = ticks;
			I2C0->D = SSD1306_xPositionStart; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			currentTicks = ticks;
			I2C0->D = SSD1306_sendOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			currentTicks = ticks;
			I2C0->D = SSD1306_xPositionStop; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag
		}
		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}

	if (SSD1306state == set_yPosition_forFlushPart)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			currentTicks = ticks;
			I2C0->D =  SSD1306_SETPAGEADDRESS ; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			currentTicks = ticks;
			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			currentTicks = ticks;
			I2C0->D = SSD1306_yPositionStart / 8; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			currentTicks = ticks;
			I2C0	->D = SSD1306_sendOneCommand;
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			currentTicks = ticks;
			I2C0->D = SSD1306_yPositionStop / 8; // Send Instruction
			while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag
		}
		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}

	}


	if (SSD1306state == flushPart)
	{
		uint16_t pos = 0;

		time_delay(5);

		uint8_t m = SSD1306_xPositionStart;

		time_delay(5);

		uint8_t n = SSD1306_yPositionStart;

		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			for(n = (SSD1306_yPositionStart / 8); n <= (SSD1306_yPositionStop / 8); n++)
			{
				for(m = SSD1306_xPositionStart; m <= SSD1306_xPositionStop; m++)
				{
					pos = m + n * 128;

					currentTicks = ticks;
					I2C0->D = SSD1306_sendMoreThanOneData;
					while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete

					currentTicks = ticks;
					I2C0->D = SSD1306_buffer[pos]; // Send Instruction
					while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete
				}
			}

			I2C0->C1 &= ~((1 << 5) | (1 << 4)); // Stop
			I2C0->S |= (1 << 1); // Clear the interrupt flag

		}
		else
		{
			I2C0->S |= (1 << 1); // Clear the interrupt flag
			return;
		}
	}

}

void I2C_sendStart(SSD1306_state state)
{
	SSD1306state = state;

	time_delay(5);

	I2C0->C1 |= (1 << 5) | (1 << 4); // Start
	I2C0->D = 0x3C << 1; // Send Slave Address 0x3C << 1

	currentTicks = ticks;
	while((I2C0->S & (1 << 7)) == 0 && ((ticks - currentTicks) < WAITING_TIME)); // wait transfer complete
}

void SSD1306_clearAllPixel(void)
{
	time_delay(50);

	for(uint16_t i = 0; i < SSD1306_bufferLenght; ++i)
	{
		SSD1306_buffer[i] = 0;
	}

	SSD1306_flushPart(0, 127, 0, 63);
}

void SSD1306_clearPixel(uint8_t xPos, uint8_t yPos)
{
	uint16_t pos = xPos + ((yPos / 8) * 128);

	SSD1306_buffer[pos] = 0;
}

void SSD1306_drawPixel(uint8_t xPos, uint8_t yPos)
{
	uint16_t pos = xPos + ((yPos / 8) * 128);

	SSD1306_buffer[pos] |= (1 << (yPos % 8));
}


void SSD1306_flushPart(uint8_t xPosStart, uint8_t xPosStop, uint8_t yPosStart, uint8_t yPosStop)
{
	time_delay(50);

	SSD1306_xPositionStart = xPosStart;

	time_delay(50);

	SSD1306_xPositionStop = xPosStop;

	time_delay(50);

	SSD1306_yPositionStart = yPosStart;

	time_delay(50);

	SSD1306_yPositionStop = yPosStop;

	time_delay(50);

	I2C_sendStart(set_yPosition_forFlushPart);

	time_delay(50);

	I2C_sendStart(set_xPosition_forFlushPart);

	time_delay(50);

	I2C_sendStart(flushPart);
}


void SSD1306_drawPicture(uint8_t bitmap[SSD1306_bufferLenght])
{
	for (uint16_t i = 0; i <= SSD1306_bufferLenght; ++i)
	{
		SSD1306_buffer[i] = bitmap[i];
	}
}

void SSD1306_drawChar(uint16_t xPos, uint16_t yPos, uint8_t c)
{
	uint16_t pos = xPos + ((yPos / 8) * 128);

	for (uint8_t i = 0; i < 6; i++)
	{
		SSD1306_buffer[pos + i] = numberFont[c][i];
	}
}

void SSD1306_turnOn(void)
{
	I2C_sendStart(display_on);

}
void SSD1306_turnOff(void)
{
	I2C_sendStart(display_off);
}

void SSD1306_init(uint32_t inputBaudRate)
{
	// I2C (OLED 128x64 driver SSD1306)

	time_delay(200);

	PORTB->PCR[2] |= (1 << 9); // I2C SCL (alt2 mode)
	PORTB->PCR[3] |= (1 << 9); // I2C SDA (alt2 mode)

	NVIC->IP[24] = (1 << 5); // Preemptive Priority (2), for "init" state, one less than PIT to allow usage of delay into ISR and permit base time of
							 // task scheduling

	SIM->SCGC4 |= (1 << 6); // Clock on I2C0 Module

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
			// wrong inputBaudRate, fix this blocking state !
		}
	}

	uint8_t regValue = resultIcr & 0b00111111;

	I2C0->F = regValue; // Set Baud Rate

	I2C0->C1 |= (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4); // I2C0 Enable, Interrupt Enable, Master Mode, TX Mode

	NVIC->ISER[0] |= (1 << 24);

	time_delay (100);

	I2C_sendStart(init);

	time_delay (100);

	SSD1306_clearAllPixel();

	time_delay (100);

	SSD1306_flushPart(0, 127, 0, 63);
}
