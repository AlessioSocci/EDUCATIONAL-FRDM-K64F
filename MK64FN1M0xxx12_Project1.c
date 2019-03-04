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
 * @file    MK64FN1M0xxx12_Project1.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

#include "logo.h"
#include "core_cm4.h"
//#include "core_armv8mbl.h"


#include "time.h"
#include "HW_interrupt.h"
#include "adc.h"
#include "dac.h"
#include "uart.h"
#include "low_power_timer.h"
#include "pwm.h"
#include "SSD1306_OLED.h"
#include "SSD_1327ZB_OLED.h"
#include "stepper.h"
/* TODO: insert other definitions and declarations here. */

// ADC
uint32_t dataADC = 0;

/*
 * @brief   Application entry point.
 */

int main(void)
{
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    NVIC_SetPriorityGrouping(0);

	SIM->SCGC5 |= (1 << 10); // Clock on PORT B
    SIM->SCGC5 |= (1 << 11); // Clock on PORT C
 	SIM->SCGC5 |= (1 << 12); // Clock on PORT D
    SIM->SCGC5 |= (1 << 13); // Clock on PORT E

    // HW interrupt
    HW_interrupt_init();

    // PIT
    pit_init();

    // ADC
    adc_init();

    // DAC
    dac_init();

    // LPTMR
    LPTMR0_init();

    // PWM
    pwm_init();

    // UART
    uart_init();

    // OLED 128x64 (I2C)
	SSD1306_init();

	// OLED 128x128 (Parallel 8080)
	SSD1327ZB_init();

	// Step Motor with L298N driver
	stepper_init();

	/* Force the counter to be placed into memory. */
    int i = 0;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1)
    {
    	if (i==0) // test display by drawing some pixel
    	{
    		SSD1306_drawPixel (3, 4);
    		SSD1306_drawPixel (3, 7);
    		SSD1306_drawPixel (3, 10);
    		SSD1306_drawPixel (3, 13);

    		SSD1306_drawPixel (6, 4);
    		SSD1306_drawPixel (6, 7);
    		SSD1306_drawPixel (6, 10);
    		SSD1306_drawPixel (6, 13);

    		SSD1306_drawPixel (20, 4);
    		SSD1306_drawPixel (20, 7);
    		SSD1306_drawPixel (20, 10);
    		SSD1306_drawPixel (20, 13);

    		SSD1306_drawPixel (50, 4);
    		SSD1306_drawPixel (50, 7);
    		SSD1306_drawPixel (50, 10);
    		SSD1306_drawPixel (50, 13);


    		SSD1306_drawPixel (50, 4);
    		SSD1306_drawPixel (50, 7);
    		SSD1306_drawPixel (50, 40);
    		SSD1306_drawPixel (50, 50);

    		SSD1306_flush();
    	}

    	if (task1Flag == 1) // Test ADC and DAC
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
    		dacOut (dataADC);
    	}

       	if (task2Flag == 1) // Test Command Line and Step Motor
    	{
        	task2Flag = 0;
        	UART_get (&UARTdataIn);
            runStepper (1, 2);
            UART_send (UARTdataIn);
        }
    	i++;
    }


    return 0 ;
}
