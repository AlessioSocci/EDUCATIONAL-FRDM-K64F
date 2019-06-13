/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    MK64FN1M0xxx12_Project.c
 * @brief   Application entry point.
 */

#include <CAN_command_line.h>
#include <UART_command_line.h>
#include "time.h"
#include "picture.h"
#include "HW_interrupt.h"
#include "adc.h"
#include "dac.h"
#include "low_power_timer.h"
#include "pwm.h"
#include "SSD1306_OLED.h"
#include "SSD_1327ZB_OLED.h"
#include "stepper.h"
#include "AD9833.h"
/* TODO: insert other definitions and declarations here. */

// ADC
uint16_t dataADC = 0;

/*
 * @brief   Application entry point.
 */

int main(void)
{
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    NVIC_SetPriorityGrouping(0);

	SIM->SCGC5 |= (1 << 10); // Clock on PORT B
    SIM->SCGC5 |= (1 << 11); // Clock on PORT C
 	SIM->SCGC5 |= (1 << 12); // Clock on PORT D
    SIM->SCGC5 |= (1 << 13); // Clock on PORT E

    PORTB->PCR[21] |= (1 << 8);	// Blue led enable
    GPIOB->PDDR |= (1 << 21);
    GPIOB->PSOR |= (1 << 21);

    PORTB->PCR[22] = (1 << 8); // Red led enable
    GPIOB->PDDR |= (1 << 22);
    GPIOB->PSOR |= (1 << 22);

    // HW interrupt
    HW_interrupt_init();

    // PIT
    pit_init();

    // I2C --- Raystar OLED 128x64
	SSD1306_init(115200);

	// Parallel 8080 --- Raystar OLED 128x128
	SSD1327ZB_init();

	// ADC
	adc_init();

	// DAC
	dac_init();

	// PWM
	pwm_init();

	// UART --- command line interface with UART-USB converter (FTDI chip)
	UART_init();

	// SPI --- DDS AD5932
	AD9833_init(9600);

	// Step Motor with L298N driver
	stepper_init();

	// LPTMR
	LPTMR0_init();

	// CAN --- command line interface with transceiver and CAN-USB converter
	CAN_init();

    uint8_t MB_number = 0;
    uint8_t cs = 0;
    uint32_t id = 0;
    uint32_t word0 = 0;
    uint32_t word1 = 0;

    TxFrame features =
    {
    	8,
      	standard,
  		frame,
    };

    /* Force the counter to be placed into memory. */
    int i = 0;

    /* Enter an infinite loop, just incrementing a counter. */
    while(1)
    {
    	CAN_writeTX_MB(0, 1, 0, 0, features);

    	CAN_writeTX_MB(0, 1, 0XFFFF, 0, features);

    	CAN_writeTX_MB(0, 1, 0, 0XFFFF, features);

        CAN_writeTX_MB(0, 1, 0XFFFF, 0XFFFF, features);


        CAN_writeTX_MB(0, 255, 0, 0, features);

    	CAN_writeTX_MB(0, 255, 0XFF, 0, features);

    	CAN_writeTX_MB(0, 255, 0, 0XFF, features);

        CAN_writeTX_MB(0, 255, 0XFF, 0XFF, features);


        CAN_writeTX_MB(0, 0, 0, 0, features);

    	CAN_writeTX_MB(0, 0, 0XFF, 0, features);

    	CAN_writeTX_MB(0, 0, 0, 0XFF, features);

        CAN_writeTX_MB(0, 0, 0XFF, 0XFF, features);



    	CAN_writeTX_MB(1, 0, 0, 0, features);

    	CAN_writeTX_MB(1, 0, 0XFF, 0, features);

    	CAN_writeTX_MB(1, 0, 0, 0XFF, features);

        CAN_writeTX_MB(1, 0, 0XFF, 0XFF, features);

//    	CAN_readRX_MB(MB_number, &cs, &id, &word0, &word1);

    	if (i==0) // test 128 x 64 display by drawing some pixel
    	{
    		delay(50);

    		SSD1306_drawPixel(10, 4);
    		SSD1306_drawPixel(20, 4);

    		SSD1306_flush();

    		SSD1306_drawPicture(logo1); // draw picture
    	}

    	if (i==1) // Waveform out from DDS
    	{
    		AD9833_setting(FREQ0, 1000, 0, SIN);
    	}

    	if (i==2) // test 128 x 128 display by drawing some pixel in "velocity mode" and "classic mode"
    	{
    		SSD1327ZB_setBuffer(0, 10, 0x0F);
    		SSD1327ZB_setBuffer(1, 10, 0x0F);
    		SSD1327ZB_setBuffer(2, 10, 0x0F);
    		SSD1327ZB_setBuffer(3, 10, 0x0F);
    		SSD1327ZB_setBuffer(4, 10, 0x0F);
    		SSD1327ZB_setBuffer(5, 10, 0x0F);
    		SSD1327ZB_setBuffer(6, 10, 0x0F);
    		SSD1327ZB_setBuffer(7, 10, 0x0F);
    		SSD1327ZB_setBuffer(8, 10, 0x0F);
    		SSD1327ZB_setBuffer(9, 10, 0x0F);
    		SSD1327ZB_setBuffer(10, 10, 0x0F);

    		SSD1327ZB_drawPartialScreen(0, 10, 10, 10);

    		SSD1327ZB_setBuffer(127, 127, 0x0F);

    		SSD1327ZB_drawPartialScreen(126, 126, 127, 127);
    	}

    	if (task1Flag == 1) // Test ADC and DAC, change picture on 128x64
    	{
    		task1Flag = 0;

    		dataADC = adc_read();
    		if ( dataADC > 1650)
    		{
    			GPIOB->PCOR |= (1 << 21); // Blue led
    		}
    		else
    		{
    			GPIOB->PSOR |= (1 << 21);
    		}
    		dacOut(dataADC);
    	}

       	if (task2Flag == 1) // Test Command Line, Step Motor, change picture on 128x64
    	{
        	task2Flag = 0;

        	UART_get (&UARTdataIn);
//        	runStepper (1, 2);
        	UART_send (UARTdataIn);

        }
    	i++;
    }


    return 0 ;
}

