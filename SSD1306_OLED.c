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


void I2C0_IRQHandler(void)
{
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

			I2C0->D = SSD1306_sendMoreThanOneCommand;
			while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

			I2C0->D = SSD1306_DISPLAYON; // Send Instruction
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

	if (SSD1306_state == 20)
	{
		if((I2C0->S & (1 << 0)) == 0) // wait to receive ack from slave device
		{
			for (uint16_t i = 0; i < SSD1306_bufferLenght; ++i)
			{
				SSD1306_data = SSD1306_buffer[i];

				I2C0->D = SSD1306_sendMoreThanOneData;
				while((I2C0->S & (1 << 7)) == 0); // wait transfer complete

				I2C0->D = SSD1306_data; // Send Instruction
				while((I2C0->S & (1 << 7)) == 0); // wait transfer complete
			}
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
void SSD1306_sendStart(uint8_t SSD1306_stateSelect)
{
	SSD1306_state = SSD1306_stateSelect;

	delay (20);

	I2C0->C1 |= (1 << 5) | (1 << 4); // Start
	I2C0->D = 0x3C << 1; // Send Slave Address 0x3C << 1
	while ((I2C0->S & (1 << 7)) == 0); // wait transfer complete
}

void SSD1306_flush (void)
{
	delay (500);

	SSD1306_sendStart(20);
}
void SSD1306_clear(void)
{
	delay (500);

	for(uint16_t i = 0; i < SSD1306_bufferLenght; ++i)
	{
		SSD1306_buffer[i] = 0;
	}

	SSD1306_flush();
}
void SSD1306_drawPixel (uint8_t xPos, uint8_t yPos)
{
	uint16_t pos = xPos + ((yPos/8) * 128);

	SSD1306_buffer[pos] |= (1 << (yPos % 8));
}
void SSD1306_drawPicture (uint8_t bitmap[SSD1306_bufferLenght])
{
	for (uint16_t i = 0; i < SSD1306_bufferLenght; ++i)
	{
		SSD1306_buffer[i] = bitmap [i];
	}

	SSD1306_flush();
}
void SSD1306_init(void)
{
	// I2C (OLED 128x64 driver SSD1306)
	PORTB->PCR[2] |= (1 << 9); // I2C SCL (alt2 mode)
	PORTB->PCR[3] |= (1 << 9); // I2C SDA (alt2 mode)

	NVIC->IP[24] = (0 << 5); // Preemptive Priority

	SIM->SCGC4 |= (1 << 6); // CLock on I2C0 Module

	I2C0->F = 0b00110000; // Set Baud Rate

	I2C0->C1 |= (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4); // I2C0 Enable, Interrupt Enable, Master Mode, TX Mode

	NVIC->ISER[0] |= (1 << 24);

	SSD1306_sendStart(10);

}
