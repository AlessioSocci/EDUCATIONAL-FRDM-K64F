/*
 * SN65HVD230.h
 *
 *  Created on: 08 apr 2019
 *      Author: Alessio
 */

#ifndef CAN_COMMAND_LINE_H_
#define CAN_COMMAND_LINE_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "core_cm4.h"

#include "time.h"

typedef struct _TxFrame
{
	uint8_t data_length;

	enum _data_format
	{
		standard,
		extended,
	}data_format;

	enum _data_type
	{
		frame,
		remote,
	}data_type;

}TxFrame;

void CAN_init(void);
void CAN_readRX_MB(uint8_t MB_number, uint8_t* cs, uint32_t* id, uint32_t* word0, uint32_t* word1);
void CAN_writeTX_MB(uint8_t MB_number, uint32_t id, uint32_t word0, uint32_t word1, TxFrame data_type);


#endif /* CAN_COMMAND_LINE_H_ */
