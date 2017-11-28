/*
 * IO.h
 *
 *  Created on: 15.04.2016
 *      Author: main
 */

#ifndef IO_H_
#define IO_H_

#include "stm32f10x.h"

#define ISO_OUT_1 GPIO_Pin_1
#define ISO_IN_1 GPIO_Pin_0

#define POWER_ST GPIO_Pin_8

#define POWER_OUT_1 GPIO_Pin_1
#define POWER_OUT_2 GPIO_Pin_0


// Switch Iso Outputs
#define IO_IO_1_ON     GPIO_SetBits(GPIOA, ISO_OUT_1)
#define IO_IO_1_OFF    GPIO_ResetBits(GPIOA, ISO_OUT_1)

// Read Iso Inputs
#define IO_II_1        GPIO_ReadBit(GPIOA, ISO_IN_1)


// Read Power IO Status
#define IO_PS          GPIO_ReadBit(GPIOA, POWER_ST)

// Switch Power IO Outputs
#define IO_PO_1_ON     GPIO_SetBits(GPIOB, POWER_OUT_1)
#define IO_PO_1_OFF    GPIO_ResetBits(GPIOB, POWER_OUT_1)

#define IO_PO_2_ON     GPIO_SetBits(GPIOB, POWER_OUT_2)
#define IO_PO_2_OFF    GPIO_ResetBits(GPIOB, POWER_OUT_2)


void IO_Init();

#endif /* IO_H_ */
