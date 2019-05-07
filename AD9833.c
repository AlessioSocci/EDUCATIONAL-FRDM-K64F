/*
 * AD9833.c
 *
 *  Created on: 04 mar 2019
 *      Author: Alessio
 */
#include "AD9833.h"


uint8_t isFirstSet = 0;

void SPI_write (uint16_t dataOut)
{
	while((SPI0->SR  & (1 << 25)) != (1 << 25)); // Wait TX FIFO register is empty

	SPI0->PUSHR |= dataOut | (1 << 16); // Assert the PCS[1] signal.

	SPI0->MCR &= ~(1 << 0); // Start transfer

    while ((SPI0->SR & (1 << 31)) != (1 << 31)); // Wait transfer complete flag became 1

    SPI0->MCR |= (1 << 0); // Stop transfer

    SPI0->SR |= (1 << 31); // Clear transfer complete flag by writing 1 ...TCF became 0!
}

void AD9833_init(uint16_t baudRate)
{
	SIM->SCGC6 |= (1 << 12); // Clock on SPI0

	PORTD->PCR[0] |= (1 << 9); // PTD0 in Alt2 Mode SPI0_PCS0
	PORTD->PCR[1] |= (1 << 9); // PTD1 in Alt2 Mode SPI0_SCK
	PORTD->PCR[2] |= (1 << 9); // PTD2 in Alt2 Mode SPI0_SOUT
//	PORTD->PCR[3] |= (1 << 9); // PTD3 in Alt2 Mode SPI0_SIN - NOT USED

	uint16_t baudRatePossibleValue[] = {2, 4, 6, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
	uint8_t baudRatePrescalerPossibleValue[] = {2, 3, 5, 7};

	uint16_t resultClock = 0;
	uint8_t scalerValue = 0;
	uint8_t prescalerValue = 0;
	uint16_t error = 0xFFFF;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			resultClock = (60000000 * baudRatePrescalerPossibleValue[i] * baudRatePossibleValue[j]);

			if(baudRate < resultClock)
			{
				if((resultClock - baudRate) < error)
				{
					error = resultClock - baudRate;
					prescalerValue = i;
					scalerValue = j;
				}
			}
			else
			{
				if((baudRate - resultClock) < error)
				{
					error = baudRate - resultClock;
					prescalerValue = i;
					scalerValue = j;
				}
			}
		}
	}

	SPI0->MCR |= (1 << 14) | (1 << 13) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 0); // Stop Hardware, Disable TX FIFO, disable RX FIFO, clear TX FIFO, clear RX FIFO, Stop transfers

	SPI0->MCR |= (1 << 31) | (1 << 16); // SPI0 in Master Mode, PCS inactive state when High

	SPI0->MCR |= (1 << 30); // Continuous clock

	SPI0->CTAR[0] |= (1 << 30) | (1 << 29) | (1 << 28); // Frame Size at: 15 + 1 = 16 bit,

//	SPI0->CTAR[0] |= (1 << 25); // data is changed on the leading edge of SCK and captured on the following edge

	SPI0->CTAR[0] |= (1 << 12); // CS assertion to CLK first edge delay prescaler and scaler; Time = (1 / fp) * prescaler (PCSSCK) * scaler(CSSCK) --> 16 * 1 * 4 = 64 ns

	SPI0->CTAR[0] |= (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8); // CLK last edge to CS negation delay prescaler and scalrer; Time = (1 / fp) * prescaler (PASC) * scaler (ASC)

	SPI0->CTAR[0] |= (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4); // time between two frame is: (1 / fp) * prescaler (PDT) * deleytime (DT)

	SPI0->CTAR[0] &= ~((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)) ; // set BR scaler; Baud Rate = (fp / prescaler) * [(1 + double BR) / BR scaler]

	SPI0->CTAR[0] |= (uint32_t)scalerValue; // Baud Rate scaler

	SPI0->CTAR[0] |= (((uint32_t)prescalerValue) << 16); // Baud Rate prescaler

	SPI0->MCR &= ~((1 << 14) | (1 << 13) | (1 << 12)); // Start hardware, Enable TX and RX FIFO.

	delay(5);
}

void AD9833_internal_Clock_Disable(void)
{
	SPI_write(AD9833_CONTROL_INTERNAL_CLOCK_OFF);
	SPI_write(AD9833_CONTROL_DAC_POWER_DOWN_ON);
}

void AD9833_internal_Clock_Enable(void)
{
	SPI_write(AD9833_CONTROL_INTERNAL_CLOCK_ON);
	SPI_write(AD9833_CONTROL_DAC_POWER_DOWN_OFF);
}

void AD9833_setting(uint8_t selectFreq0_1, uint32_t freq, uint16_t phase, WaveForm c)
{
	if (freq > 12500000)
	{
		freq = 12500000;
	}
	if (freq < 0)
	{
		freq = 0;
	}
	uint32_t freqREG = (pow(2, 28) * freq) / 25000000;

	uint16_t freqLSB_REG = (uint16_t)(freqREG &  0b00000000000000000011111111111111); // 14 bit to set frequency LSB
	uint16_t freqMSB_REG = (uint16_t)((freqREG & 0b00001111111111111100000000000000) >> 14); // 14 bit to set frequency MSB

	if (phase > 360 )
	{
		phase = 360;
	}
	if (phase < 0 )
	{
		phase = 0;
	}
	phase = ((4096/360) * phase);
	phase &= AD9833_WRITE_PHASE_MASK;

	if (isFirstSet)
	{
		SPI_write(AD9833_CONTROL_RESET_ON);
	}

	switch (c)
	{
		case SQUARE:
		{
			if (selectFreq0_1 == FREQ0)
			{
				SPI_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_LSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_OFF);
				SPI_write(AD9833_WRITE_FREQ0_REG | freqLSB_REG);

				SPI_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_MSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_OFF);
				SPI_write(AD9833_WRITE_FREQ0_REG | freqMSB_REG);

				SPI_write(AD9833_CONTROL_PHASE0_SELECT);
				SPI_write(AD9833_WRITE_PHASE0_REG | phase);
			}
			else if (selectFreq0_1 == FREQ1)
			{
				SPI_write(AD9833_CONTROL_FREQ1_SELECT | AD9833_CONTROL_LSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_OFF);
				SPI_write(AD9833_WRITE_FREQ1_REG | freqLSB_REG);

				SPI_write(AD9833_CONTROL_FREQ1_SELECT | AD9833_CONTROL_MSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_OFF);
				SPI_write(AD9833_WRITE_FREQ1_REG | freqMSB_REG);

				SPI_write(AD9833_CONTROL_PHASE1_SELECT);
				SPI_write(AD9833_WRITE_PHASE1_REG | phase);
			}
		}

		case TRI:
		{
			if (selectFreq0_1 == FREQ0)
			{
				SPI_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_LSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON | AD9833_CONTROL_DAC_OUT_IS_TRI);
				SPI_write(AD9833_WRITE_FREQ0_REG | freqLSB_REG);

				SPI_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_MSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON | AD9833_CONTROL_DAC_OUT_IS_TRI);
				SPI_write(AD9833_WRITE_FREQ0_REG | freqMSB_REG);

				SPI_write(AD9833_CONTROL_PHASE0_SELECT);
				SPI_write(AD9833_WRITE_PHASE0_REG | phase);
			}
			else if (selectFreq0_1 == FREQ1)
			{
				SPI_write(AD9833_CONTROL_FREQ1_SELECT | AD9833_CONTROL_LSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON | AD9833_CONTROL_DAC_OUT_IS_TRI);
				SPI_write(AD9833_WRITE_FREQ1_REG | freqLSB_REG);

				SPI_write(AD9833_CONTROL_FREQ1_SELECT | AD9833_CONTROL_MSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON | AD9833_CONTROL_DAC_OUT_IS_TRI);
				SPI_write(AD9833_WRITE_FREQ1_REG | freqMSB_REG);

				SPI_write(AD9833_CONTROL_PHASE1_SELECT);
				SPI_write(AD9833_WRITE_PHASE1_REG | phase);
			}
		}

		case SIN:
		{
			if (selectFreq0_1 == FREQ0)
			{
				SPI_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_LSB_FREQ_REG |AD9833_CONTROL_DAC_TO_VOUT_ON);
				SPI_write(AD9833_WRITE_FREQ0_REG | freqLSB_REG);

				SPI_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_MSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON);
				SPI_write(AD9833_WRITE_FREQ0_REG | freqMSB_REG);

				SPI_write(AD9833_CONTROL_PHASE0_SELECT);
				SPI_write(AD9833_WRITE_PHASE0_REG | phase);

				// but, by the data sheet, the right sequences must be...

//				SPI_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_MSB_BOTH_LSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON | AD9833_CONTROL_RESET_ON);
//				SPI_write(AD9833_WRITE_FREQ0_REG | freqLSB_REG);
//				SPI_write(AD9833_WRITE_FREQ0_REG | freqMSB_REG);
//
//				SPI_write(AD9833_WRITE_PHASE0_REG | phase);
//
//				SPI_write(AD9833_CONTROL_RESET_OFF);
			}
			else if (selectFreq0_1 == FREQ1)
			{
				SPI_write(AD9833_CONTROL_FREQ1_SELECT | AD9833_CONTROL_LSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON);
				SPI_write(AD9833_WRITE_FREQ1_REG | freqLSB_REG);

				SPI_write(AD9833_CONTROL_FREQ1_SELECT | AD9833_CONTROL_MSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON);
				SPI_write(AD9833_WRITE_FREQ1_REG | freqMSB_REG);

				SPI_write(AD9833_CONTROL_PHASE1_SELECT);
				SPI_write(AD9833_WRITE_PHASE1_REG | phase);
			}
		}
	}
	if (isFirstSet)
	{
		SPI_write(AD9833_CONTROL_RESET_OFF);
		isFirstSet = 0;
	}
}

