/*
 * 1327ZB.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#include <SSD_1327ZB_OLED.h>

// OLED 128 x 128 Driver SSD1327ZB
uint8_t SSD1327ZB_buffer [SSD1327ZB_bufferLenght];

uint8_t buffer [64][128];

void SSD1327ZB_init(void)
{
	// PARALLEL 8080 (OLED 128x128 driver SSD1327ZB)

	time_delay(100);

	PORTC->PCR[10] = (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, Write input
	GPIOC->PDDR = (1 << 10);
	GPIOC->PCOR = (1 << 10);

	PORTD->PCR[3] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, D6
	GPIOD->PDDR |= (1 << 3);
	GPIOD->PCOR |= (1 << 3);

	PORTB->PCR[20] |= (1 << 8) | (1 << 1) | (1 << 0); // in alt1 mode, D7
	GPIOB->PDDR |= (1 << 20);
	GPIOB->PCOR |= (1 << 20);

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

	time_delay(100);

	GPIOE->PSOR |= (1 << 25); // DC
	GPIOC->PSOR |= (1 << 10); // RD
	GPIOC->PSOR |= (1 << 4); // WR
	GPIOE->PSOR |= (1 << 24); // CS
	GPIOE->PSOR |= (1 << 26); // RS

	time_delay(100);

	SSD1327ZB_sendCommand(SSD1327ZB_CMD_DISPLAYOFF);

	time_delay(10);

	SSD1327ZB_sendCommand(SSD1327ZB_CMD_DISPLAYNORMAL);

	time_delay(10);

	SSD1327ZB_sendCommand(SSD1327ZB_CMD_SEGMENTREMAP);

	time_delay(10);

	SSD1327ZB_sendCommand(SSD1327ZB_REMAP_ODDEVEN_COM);

	time_delay(10);

	SSD1327ZB_sendCommand(SSD1327ZB_CMD_STARTLINE);

	time_delay(10);

	SSD1327ZB_sendCommand(0);

	time_delay(10);

	SSD1327ZB_sendCommand(SSD1327ZB_CMD_SETCONTRAST);

	time_delay(10);

	SSD1327ZB_sendCommand(0xFF);

	time_delay(10);

	SSD1327ZB_sendCommand(SSD1327ZB_CMD_DISPLAYON);

//	SSD1327ZB_sendCommand(SSD1327ZB_CMD_DISPLAYALLON);

	SSD1327ZB_clearAllPixel ();
	SSD1327ZB_flush ();
}


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
     ((value & 0x40) > 0) ? (GPIOD->PSOR |= (1 << 3)) : (GPIOD->PCOR |= (1 << 3)); //D6
     ((value & 0x80) > 0) ? (GPIOB->PSOR |= (1 << 20)) : (GPIOB->PCOR |= (1 << 20));  //D7

     GPIOC->PSOR |= (1 << 10);  // WR
     GPIOE->PSOR |= (1 << 24); // CS
}

void SSD1327ZB_sendData(uint8_t value)
{
	GPIOE->PSOR |= (1 << 26); // RS
	GPIOE->PCOR |= (1 << 24); // CS
	GPIOC->PSOR |= (1 << 10); // WR
	GPIOE->PSOR |= (1 << 25); // DC
	GPIOC->PCOR |= (1 << 10); // WR

	 ((value & 0x01) > 0) ? (GPIOB->PSOR |= (1 << 20)) : (GPIOB->PCOR |= (1 << 20)); // D7
	 ((value & 0x02) > 0) ? (GPIOD->PSOR |= (1 << 3)) : (GPIOD->PCOR |= (1 << 3)); // D6
	 ((value & 0x04) > 0) ? (GPIOC->PSOR |= (1 << 1)) : (GPIOC->PCOR |= (1 << 1)); // D5
	 ((value & 0x08) > 0) ? (GPIOC->PSOR |= (1 << 8)) : (GPIOC->PCOR |= (1 << 8)); // D4
	 ((value & 0x10) > 0) ? (GPIOC->PSOR |= (1 << 9)) : (GPIOC->PCOR |= (1 << 9)); // D3
	 ((value & 0x20) > 0) ? (GPIOC->PSOR |= (1 << 0)) : (GPIOC->PCOR |= (1 << 0)); // D2
	 ((value & 0x40) > 0) ? (GPIOC->PSOR |= (1 << 7)) : (GPIOC->PCOR |= (1 << 7)); //D1
	 ((value & 0x80) > 0) ? (GPIOC->PSOR |= (1 << 5)) : (GPIOC->PCOR |= (1 << 5));  //D0

	GPIOC->PSOR |= (1 << 10); // WR
	GPIOE->PSOR |= (1 << 24); // CS
}

// FOLLOWING NEW FUNCTION TO DRAWING: IMPROVED VELOCITY FOR ANIMATION.

void SSD1327ZB_setBuffer(uint8_t xPos, uint8_t yPos, uint8_t greyScale)
{
	if (greyScale > 0x0F)
	{
		greyScale = 0x0F;
	}

	if (xPos %2 != 0) // if odd...
	{
		buffer[xPos/2][yPos] = buffer[xPos/2][yPos] | greyScale;
	}
	else
	{
		buffer[xPos/2][yPos] = buffer[xPos/2][yPos] | (greyScale << 4);
	}

}

void SSD1327ZB_setAreaOnScreen (uint8_t xStart,  uint8_t xStop, uint8_t yStart,  uint8_t yStop)
{
	SSD1327ZB_sendCommand(SSD1327ZB_CMD_SETCOLUMNADDR);
	SSD1327ZB_sendCommand(xStart / 2);
	SSD1327ZB_sendCommand(xStop / 2);

	SSD1327ZB_sendCommand(SSD1327ZB_CMD_SETROWADDR);
	SSD1327ZB_sendCommand(yStart);
	SSD1327ZB_sendCommand(yStop);
}

void SSD1327ZB_drawPartialScreen(uint8_t xStart, uint8_t xStop, uint8_t yStart, uint8_t yStop)
{
	SSD1327ZB_setAreaOnScreen (xStart, xStop, yStart, yStop);

	for (uint8_t j = yStart; j <= yStop; j++)
	{

		if (xStart == xStop)
		{
			if (xStart % 2 != 0)
			{
				for (uint8_t i = (xStart / 2); i <= (xStart / 2) + ((xStop - xStart) / 2); i++)
				{
					SSD1327ZB_sendData(buffer[i][j] & 0xF0);
				}
			}
			else
			{
				for (uint8_t i = (xStart / 2); i <= (xStart / 2) + ((xStop - xStart) / 2); i++)
				{
					SSD1327ZB_sendData(buffer[i][j] & 0x0F);
				}
			}
		}
		else if (((xStart % 2) == 0) && ((xStop % 2) == 0))
		{
			for (uint8_t i = (xStart / 2); i <= (xStart / 2) + ((xStop - xStart) / 2); i++)
			{
				if (i != (xStart / 2) + ((xStop - xStart) / 2))
				{
					SSD1327ZB_sendData(buffer[i][j]);
				}
				else
				{
					SSD1327ZB_sendData(buffer[i][j] & 0x0F);
				}
			}
		}
		else if (((xStart % 2) != 0) && ((xStop % 2) == 0))
		{
			for (uint8_t i = (xStart / 2); i <= (xStart / 2) + ((xStop - xStart) / 2); i++)
			{
				if (i == (xStart / 2))
				{
					SSD1327ZB_sendData(buffer[i][j] & 0xF0);
					break;
				}
				else if ((i != (xStart / 2) )&& (i != (xStart / 2) + ((xStop - xStart) / 2)))
				{
					SSD1327ZB_sendData(buffer[i][j]);
					break;
				}
				else if (i == (xStart / 2) + ((xStop - xStart) / 2))
				{
					SSD1327ZB_sendData(buffer[i][j] & 0x0F);
					break;
				}
			}
		}
		else if (((xStart % 2) == 0) && ((xStop % 2) != 0))
		{
			for (uint8_t i = (xStart / 2); i <= (xStart / 2) + ((xStop - xStart) / 2); i++)
			{
				SSD1327ZB_sendData(buffer[i][j]);
			}
		}
		else if (((xStart % 2) != 0) && ((xStop % 2) != 0))
		{
			for (uint8_t i = (xStart / 2); i <= (xStart / 2) + ((xStop - xStart) / 2); i++)
			{
				if (i == (xStart / 2))
				{
					SSD1327ZB_sendData(buffer[i][j] & 0xF0);
					break;
				}
				else
				{
					SSD1327ZB_sendData(buffer[i][j]);
				}
			}
		}
	}
}

void SSD1327ZB_drawFullScreen()
{
	SSD1327ZB_setAreaOnScreen (0, 127, 0, 127);
	for (uint8_t j = 0; j <= 127; j++)
	{
		for (uint8_t i = 0 / 2; i <= 127 / 2; i++)
		{
			SSD1327ZB_sendData(buffer[i][j]);
		}
	}
}

// FOLLOWING HOLD FUNCTION TO DRAWING...

void SSD1327ZB_drawPixel (uint8_t xPos, uint8_t yPos)
{
	uint16_t pos = ((uint16_t) xPos/2) + ((uint16_t) yPos * 64);

	if (xPos%2)
	{
		SSD1327ZB_buffer[pos] = SSD1327ZB_buffer[pos] | 0xF0;
	}
	else
	{
		SSD1327ZB_buffer[pos] = SSD1327ZB_buffer[pos] | 0x0F;
	}
}

void SSD1327ZB_drawPicture(uint8_t bitmap[SSD1327ZB_bufferLenght])
{
	for (uint16_t i = 0; i < SSD1327ZB_bufferLenght; ++i)
	{
		SSD1327ZB_buffer[i] = bitmap[i];
	}

	SSD1327ZB_flush();
}

void SSD1327ZB_clearAllPixel (void)
{
	for (uint16_t i = 0; i < SSD1327ZB_bufferLenght; i++)
	{
		SSD1327ZB_buffer[i]=0;
	}
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
		((SSD1327ZB_buffer[i] & 0x40) > 0) ? (GPIOD->PSOR |= (1 << 3)) : (GPIOD->PCOR |= (1 << 3)); //D6
		((SSD1327ZB_buffer[i] & 0x80) > 0) ? (GPIOB->PSOR |= (1 << 20)) : (GPIOB->PCOR |= (1 << 20));  //D7

		GPIOC->PSOR |= (1 << 10);  // WR
		GPIOE->PSOR |= (1 << 24); // CS
	}
}

