/*
 * SSD1306_OLED.h
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "core_cm4.h"

#include "time.h"

#ifndef SSD1306_OLED_H_
#define SSD1306_OLED_H_

// OLED 128 x 64 Driver SSD1306
#define SSD1306_SETADDRESSINGMODE          			0x20
#define SSD1306_SETCOLUMNADDRESS          			0x21
#define SSD1306_SETPAGEADDRESS             			0x22
#define SSD1306_DEACTIVATESCROLL           			0x2E
#define SSD1306_ACTIVATESCROLL             			0x2F
#define SSD1306_SETDISPLAYSTARTLINE        			0x40
#define SSD1306_SETCONTRAST                			0x81
#define SSD1306_CHARGEPUMP                 			0x8D
#define SSD1306_SEGMENTREMAP               			0xA0
#define SSD1306_DISPLAYALLONRESUME         			0xA4
#define SSD1306_DISPLAYALLON               			0xA5
#define SSD1306_DISPLAYNORMAL              			0xA6
#define SSD1306_DISPLAYINVERSE             			0xA7
#define SSD1306_SETMUXRATIO                			0xA8
#define SSD1306_DISPLAYOFF                 			0xAE
#define SSD1306_DISPLAYON                  			0xAF
#define SSD1306_COMSCANDIRECTIONUP         			0xC0
#define SSD1306_COMSCANDIRECTIONDOWN       			0xC8
#define SSD1306_SETDISPLAYOFFSET           			0xD3
#define SSD1306_SETDISPLAYCLK              			0xD5
#define SSD1306_COMPINS                    			0xDA
#define SSD1306_SETDESELECTLEVEL           			0xDB
#define SSD1306_SETPRECHARGE						0xD9

#define SSD1306_bufferLenght						(8*128)
#define SSD1306_sendOneCommand 						0x00
#define SSD1306_sendMoreThanOneCommand 				0x80
#define SSD1306_sendOneData 						0x40
#define SSD1306_sendMoreThanOneData					0xC0

void SSD1306_flush (void);
void SSD1306_clearAllPixel(void);
void SSD1306_drawPixel (uint8_t xPos, uint8_t yPos);
void SSD1306_drawPixel2 (uint8_t xPos, uint8_t yPos);
void SSD1306_drawPicture (uint8_t bitmap[]);
void SSD1306_init(uint32_t inputBaudRate);

#endif /* SSD1306_OLED_H_ */
