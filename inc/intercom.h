/*	SolarCar Projekt
		Hochschule Bochum
		Saison 2013-2015
		
		Modul:		NVBM
		Author:		Alexander Schroeder
*/

#include "can.h"

uint8_t INT_CAN_Transmit(CanTxMsgTypeDef* tx);

void INT_CAN_Receive_handler(CAN_HandleTypeDef* CANx, uint8_t FIFONumber);
