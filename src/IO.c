/*
 * IO.c
 *
 *  Created on: 15.04.2016
 *      Author: Alexander Schröder
 *       Brief: Master Input Output
 */

#include "IO.h"
#include "BMS_config.h"


void IO_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_Inittd;

	// GPIOA Outputs
	GPIO_Inittd.GPIO_Pin = ISO_OUT_1;
	GPIO_Inittd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Inittd.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_Inittd);

	// GPIOB Outputs
	GPIO_Inittd.GPIO_Pin = POWER_OUT_1 | POWER_OUT_2;
	GPIO_Init(GPIOB, &GPIO_Inittd);

	// GPIOA Inputs
	GPIO_Inittd.GPIO_Pin = ISO_IN_1 | POWER_ST;
	GPIO_Inittd.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_Inittd);
}
