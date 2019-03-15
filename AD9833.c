/*
 * AD9833.c
 *
 *  Created on: 04 mar 2019
 *      Author: Alessio
 */
#include "AD9833.h"

void AD9833_init(void)
{
	SIM->SCGC6 |= (1 << 12); // Clock on SPI0

	PORTD->PCR[0] |= (1 << 9); // PTD0 in Alt2 Mode SPI0_PCS0
	PORTD->PCR[1] |= (1 << 9); // PTD1 in Alt2 Mode SPI0_SCK
	PORTD->PCR[2] |= (1 << 9); // PTD2 in Alt2 Mode SPI0_SOUT
	PORTD->PCR[3] |= (1 << 9); // PTD3 in Alt2 Mode SPI0_SIN

	SPI0->MCR |= (1 << 14) | (1 << 13) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 0); // Stop Hardware, Disable TX FIFO, disable RX FIFO, clear TX FIFO, clear RX FIFO, Stop sransfers

	SPI0->MCR |= (1 << 31) | (1 << 30) | (1 << 16); // SPI0 in Master Mode, PCS inactive state when High

	SPI0->CTAR[0] |= (1 << 30) | (1 << 29) | (1 << 28) | (1 << 25); // Frame Size at 15 bit + 1 = 16 bit, data change on leading edge, captured on following edge

	SPI0->CTAR[0] &= ~((1 << 31) | (1 << 17) | (1 << 16) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)); // set as 0 DBR, PBR and BR bit field
	SPI0->CTAR[0] |= (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // Baud rate scaler set as max value 32768

	SPI0->MCR &= ~((1 << 14) | (1 << 13) | (1 << 12)); // Start hardware, Enable TX and RX FIFO.

}

void AD9833_firstStart(uint8_t selectFreq0_1, uint32_t freq, uint16_t phase, WaveForm c)
{
	if (freq > 12500000)
	{
		freq = 12500000;
	}
	if (freq < 0)
	{
		freq = 0;
	}
	uint32_t freqREG = (freq * (pow(2, 28))) / 25000000;
	int16_t freqMSB_REG = (int16_t)((freqREG & 0b00111111111111111100000000000000) >> 14); // 14 bit to set frequency MSB
	int16_t freqLSB_REG = (int16_t)(freqREG & 0b00000000000000000011111111111111); // 14 bit to set frequency LSB

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

	AD9833_write(AD9833_CONTROL_RESET_ON);

	switch (c)
	{
		case SQUARE:
		{
			if (selectFreq0_1 == FREQ0)
			{
				//	AD9833_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_MSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_OFF);
				//	AD9833_write(AD9833_WRITE_FREQ0_REG | freqMSB_REG);

				AD9833_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_LSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_OFF);
				AD9833_write(AD9833_WRITE_FREQ0_REG | freqLSB_REG);
			}
			else if (selectFreq0_1 == FREQ1)
			{
				//	AD9833_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_MSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_OFF);
				//	AD9833_write(AD9833_WRITE_FREQ0_REG | freqMSB_REG);

				AD9833_write(AD9833_CONTROL_FREQ1_SELECT | AD9833_CONTROL_LSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_OFF);
				AD9833_write(AD9833_WRITE_FREQ1_REG | freqLSB_REG);
			}

			break;
		}

		case TRI:
		{
			if (selectFreq0_1 == FREQ0)
			{
//				AD9833_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_MSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON | AD9833_CONTROL_DAC_OUT_IS_TRI);
//				AD9833_write(AD9833_WRITE_FREQ0_REG | freqMSB_REG);

				AD9833_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_LSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON | AD9833_CONTROL_DAC_OUT_IS_TRI);
				AD9833_write(AD9833_WRITE_FREQ0_REG | freqLSB_REG);

				break;
			}
			else if (selectFreq0_1 == FREQ1)
			{
//				AD9833_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_MSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON | AD9833_CONTROL_DAC_OUT_IS_TRI);
//				AD9833_write(AD9833_WRITE_FREQ0_REG | freqMSB_REG);

				AD9833_write(AD9833_CONTROL_FREQ1_SELECT | AD9833_CONTROL_LSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON | AD9833_CONTROL_DAC_OUT_IS_TRI);
				AD9833_write(AD9833_WRITE_FREQ1_REG | freqLSB_REG);

				break;
			}
		}

		case SIN:
		{
			if (selectFreq0_1 == FREQ0)
			{
//				AD9833_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_MSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON);
//				AD9833_write(AD9833_WRITE_FREQ0_REG | freqMSB_REG);

				AD9833_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_LSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON);
				AD9833_write(AD9833_WRITE_FREQ0_REG | freqLSB_REG);

				break;
			}
			else if (selectFreq0_1 == FREQ1)
			{
//				AD9833_write(AD9833_CONTROL_FREQ0_SELECT | AD9833_CONTROL_MSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON);
//				AD9833_write(AD9833_WRITE_FREQ0_REG | freqMSB_REG);

				AD9833_write(AD9833_CONTROL_FREQ1_SELECT | AD9833_CONTROL_LSB_FREQ_REG | AD9833_CONTROL_DAC_TO_VOUT_ON);
				AD9833_write(AD9833_WRITE_FREQ1_REG | freqLSB_REG);

				break;
			}
		}
	}

	AD9833_write(AD9833_CONTROL_PHASE0_SELECT);
	AD9833_write(AD9833_WRITE_PHASE0_REG | phase);

	AD9833_write(AD9833_CONTROL_RESET_OFF);
}

void AD9833_write (uint16_t dataOut)
{

	while((SPI0->SR  & (1 << 25)) != (1 << 25)); // Wait TX FIFO register is empty

	SPI0->PUSHR |= dataOut | (1 << 16);

	SPI0->MCR &= ~(1 << 0); // Start transfer

    while ((SPI0->SR & (1 << 31)) != (1 << 31)); // Wait transfer complete flag became 1

    SPI0->MCR |= (1 << 0); // Stop transfer

    SPI0->SR |= (1 << 31); // Clear transfer complete flag by writing 1 ...TCF became 0!
}




