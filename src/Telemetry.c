/*
 * Telemetry.c
 *
 *  Created on: 22.04.2016
 *      Author: main
 */

#include "BMS_config.h"
#include "Telemetry.h"
#include "LTC6804_handler.h"
#include "LTC6804.h"

#include <string.h>

volatile uint8_t RxBuffer[RXBUFFERSIZE];
volatile uint8_t TxBuffer[TXBUFFERSIZE];
volatile uint32_t TXWp = 0, TXRp = 0, RXWp = 0, RXRp = 0;

char rx_buffer[20];
char buf[50];
char icache[20];
uint8_t cc;
bool tx_schedule = false;

uint32_t uart_rx_size()
{
	if (RXWp < RXRp)
		return RXBUFFERSIZE - RXRp + RXWp;
	else
		return RXWp - RXRp;
}

uint8_t uart_receive_buf(uint8_t *buffer)
{
	uint32_t readcounter = 0;

	while (RXRp != RXWp)
	{
		*(buffer++) = RxBuffer[RXRp++];

		if (RXRp == RXBUFFERSIZE)
			RXRp = 0;
		readcounter++;
	}

	*buffer = 0;
	return readcounter;
}

uint8_t uart_transmit_char(uint8_t data)
{
	cc += data;

	if ((TXWp == (TXRp-1)) || ((TXWp == (TXBUFFERSIZE-1)) && (TXRp == 0)))
		return 1;  // Buffer overrun


	TxBuffer[TXWp] = data; // cue data

	if (TXWp == (TXBUFFERSIZE-1))  // update pointer
		TXWp = 0;
	else
		TXWp++;

	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	return 0;
}

uint8_t uart_transmit_string(char* string)
{
	while(*string)
	{
		if (uart_transmit_char(*(string++)))
			return 1;
	}

	return 0;
}

uint8_t uart_transmit_block(uint8_t* data, uint8_t count)
{
	for (uint8_t i = 0; i < count; count--)
	{
		if (uart_transmit_char(*(data++)))
			return 1;
	}

	return 0;
}

void Telemetry_Init()
{
	GPIO_InitTypeDef GPIO_Inittd;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// Enable Clocks for GPIO and USART
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// PB10 as TX Output
	GPIO_Inittd.GPIO_Pin = GPIO_Pin_10;
	GPIO_Inittd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Inittd.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_Inittd);

	// Configure USART3 as 115200 8N1 TX only
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART3, &USART_InitStructure);

	USART_Cmd(USART3, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	// Enable the USARTx Interrupt
//	NVIC_EnableIRQ(USART3_IRQn);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Enable the WAKEUP_BUTTON_EXTI_IRQn Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	tx_schedule = false;
	strcpy(rx_buffer, "");
}

void Telemtry_receive()
{
	if (uart_receive_buf((uint8_t*) buf) > 0)
		strcat(rx_buffer, buf);

	char *newline = strstr(rx_buffer, "\n");
	if (newline != NULL)
	{
		if (strstr(rx_buffer, "NeoBMS ping\n") != NULL)
			tx_schedule = true;
		else if (strstr(rx_buffer, "NeoBMS full\n") != NULL)
			BMS_OV = BMS_full_OV;
		else if (strstr(rx_buffer, "NeoBMS std\n") != NULL)
			BMS_OV = BMS_std_OV;
		else if (strstr(rx_buffer, "NeoBMS chst\n") != NULL)
		{
			BMS_Charge_permitted = true;
			BMS_fault_latch = false;
		}

		strcpy(rx_buffer, "");
	}

	if (strlen(rx_buffer) >= 35)
		strcpy(rx_buffer, "");
}

void Telemetry_handler()
{
	Telemtry_receive();

	if (tx_schedule)
	{
		tx_schedule = false;
		cc = 0;

		strcpy(buf, "---NeoBMS---\n");
		uart_transmit_string(buf);


		strcpy(buf, "CellCount: ");
		itoa(Cell_count, icache, 10);
		strcat(buf, icache);
		strcat(buf, "\n");
		uart_transmit_string(buf);

		strcpy(buf, "Slavecount: ");
#if BMS_chip_count > 0
		itoa(BMS_chip_count, icache, 10);
#else
		strcpy(buf, "1");
#endif
		strcat(buf, icache);
		strcat(buf, "\n");
		uart_transmit_string(buf);

		strcpy(buf, "dU: ");
		itoa((Global_Max_U - Global_Min_U)/1000, icache, 10);
		strcat(buf, icache);
		strcat(buf, "mV\n");
		uart_transmit_string(buf);

		strcpy(buf, "Max U: ");
		itoa(Global_Max_U/1000, icache, 10);
		strcat(buf, icache);
		strcat(buf, "mV\n");
		uart_transmit_string(buf);

		strcpy(buf, "Min U: ");
		itoa(Global_Min_U/1000, icache, 10);
		strcat(buf, icache);
		strcat(buf, "mV\n");
		uart_transmit_string(buf);

		strcpy(buf, "SOC: ");
		itoa(Global_SOC, icache, 10);
		strcat(buf, icache);
		strcat(buf, "%\n");
		uart_transmit_string(buf);

#ifdef BMS_I_SENSOR
		strcpy(buf, "Battery I: ");
		itoa(Battery_I, icache, 10);
		strcat(buf, icache);
		strcat(buf, "mA\n");
		uart_transmit_string(buf);
#endif

		strcpy(buf, "Balancing: ");  // Cell Test
		if (BMS_Balance_Scheduled)
		{
			strcat(buf, "Scheduled and ");
			strcpy(icache, "Permitted\n");
#if BMS_chip_count > 0
			for (uint8_t j = 0; j < BMS_chip_count; j++)
#else
			uint8_t j = 0;
#endif
				if (!chips[j].Balance_Permission)
					strcat(buf, "Not ");
			strcat(buf, icache);
		}
		else
			strcat(buf, "Not Scheduled\n");
		uart_transmit_string(buf);

		strcpy(buf, "Charge Permitted: ");  // Cell Test
		if (BMS_Charge_permitted)
			strcat(buf, "Yes\n");
		else
			strcat(buf, "No\n");
		uart_transmit_string(buf);

		strcpy(buf, "Discharge Permitted: ");  // Cell Test
		if (BMS_Discharge_permitted)
			strcat(buf, "Yes\n");
		else
			strcat(buf, "No\n");
		uart_transmit_string(buf);

		strcpy(buf, "BMS_OV: ");  // Active OV Limit
		itoa(BMS_OV/1000, icache, 10);
		strcat(buf, icache);
		strcat(buf, "mV\n\n");
		uart_transmit_string(buf);

#if BMS_chip_count > 0
		for (uint32_t i = 0; i < BMS_chip_count; i++)
#else
		uint8_t i = 0;
#endif
		{
			strcpy(buf, "---BMS ");             // Titel
			itoa(i, icache, 16);
			strcat(buf, icache);
			strcat(buf, "---\n");
			uart_transmit_string(buf);

			strcpy(buf, "Health: ");            // Health
			if (chips[i].Health == 0)
				strcat(buf, "OK");
			else if (chips[i].Health == 1)
				strcat(buf, "Error Operational");
			else if (chips[i].Health == 2)
				strcat(buf, "Error Nonoperational");
			else
				strcat(buf, "Illegal");
			strcat(buf, "\n");
			uart_transmit_string(buf);

			strcpy(buf, "ITMP: ");              // ITMP
			itoa(chips[i].Int_Temp, icache, 10);
			strcat(buf, icache);
			strcat(buf, "C\n");
			uart_transmit_string(buf);

			strcpy(buf, "Cell Test Passed: ");  // Cell Test
			if (chips[i].Cell_Test_Passed)
				strcat(buf, "yes\n");
			else
				strcat(buf, "no\n");
			uart_transmit_string(buf);

			strcpy(buf, "GPIO Test Passed: ");  // GPIO Test
			if (chips[i].GPIO_Test_Passed)
				strcat(buf, "yes\n");
			else
				strcat(buf, "no\n");
			uart_transmit_string(buf);

			strcpy(buf, "Status Test Passed: ");  // Status Test
			if (chips[i].Status_Test_Passed)
				strcat(buf, "yes\n");
			else
				strcat(buf, "no\n");
			uart_transmit_string(buf);

			strcpy(buf, "MUX Test Passed: ");  // MUX Test
			if (chips[i].MUX_Test_Passed)
				strcat(buf, "yes\n");
			else
				strcat(buf, "no\n");
			uart_transmit_string(buf);

			strcpy(buf, "Secondary Reference: ");  // Secondary Reference
			if (chips[i].Secondary_Reference_OK)
				strcat(buf, "OK\n");
			else
				strcat(buf, "Not OK\n");
			uart_transmit_string(buf);

			strcpy(buf, "Internal Temperatur: ");  // Secondary Reference
			if (chips[i].Int_Temp_OK)
				strcat(buf, "OK\n");
			else
				strcat(buf, "Not OK\n");
			uart_transmit_string(buf);

			strcpy(buf, "VA: ");  // Secondary Reference
			if (chips[i].VA_OK)
				strcat(buf, "OK\n");
			else
				strcat(buf, "Not OK\n");
			uart_transmit_string(buf);

			strcpy(buf, "VD: ");  // Secondary Reference
			if (chips[i].VD_OK)
				strcat(buf, "OK\n");
			else
				strcat(buf, "Not OK\n");
			uart_transmit_string(buf);

			strcpy(buf, "Balancing: ");  // Secondary Reference
			if (chips[i].Balance_Permission)
			{
				itoa(chips[i].Balance_derating, icache, 10);
				strcat(buf, "Permitted ");
				strcat(buf, icache);
				strcat(buf, " Cells\n");
			}
			else
				strcat(buf, "Not Permitted\n");
			uart_transmit_string(buf);

			for (uint8_t j = 0; j < BMS_temp_count[i]; j++)
			{
				strcpy(buf, "Temperature");
				itoa(j + 1, icache, 10);
				strcat(buf, icache);
				strcat(buf, ": ");
				itoa(chips[i].Temp_sensors[j], icache, 10);
				strcat(buf, icache);
				strcat(buf, "°C\n");
				uart_transmit_string(buf);
			}

			for (uint8_t j = 0; j < BMS_cell_count[i]; j++)
			{
				strcpy(buf, "Cell");
				itoa(j + 1, icache, 10);
				strcat(buf, icache);
				strcat(buf, ": ");
				itoa(chips[i].Cell_U[j]/1000, icache, 10);
				strcat(buf, icache);
				strcat(buf, "mV");

				if (chips[i].Cell_Bleed[j])
					strcat(buf, " Bleeding");
				if (chips[i].Open_Cell_Connection[j])
					strcat(buf, " Open");
				if (chips[i].Cell_OV[j])
					strcat(buf, " OV");
				if (chips[i].Cell_UV[j])
					strcat(buf, " UV");

				strcat(buf, "\n");
				uart_transmit_string(buf);
			}

			strcpy(buf, "\n\n");
			uart_transmit_string(buf);
		}

		strcpy(buf, "CC: ");
		itoa(cc, icache, 10);
		strcat(buf, icache);
		strcat(buf, "\n---End Frame---\n\n");
		uart_transmit_string(buf);
	}
}
