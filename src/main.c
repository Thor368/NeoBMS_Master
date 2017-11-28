/**
  ******************************************************************************
  * @file    main.c
  * @author  Alexander Schröder
  * @version V1.0
  * @date
  * @brief
  ******************************************************************************
*/

#include "BMS_config.h"
#include "IO.h"
#include "Telemetry.h"
#include "LTC6804_handler.h"

volatile uint32_t Tick = 0;
uint32_t tt = 0;

void _delay_ms(uint32_t time)
{
	uint32_t start = Tick;

	while ((start + time) > Tick);
}

void _delay_us(uint32_t time)
{
	_delay_ms(time / 1000);
}

int main(void)
{
	SysTick_Config(SystemCoreClock / 1000);

	IO_Init();
	LTC_handler_Init();
	Telemetry_Init();

	while (1)
	{
		LTC_handler();
		Telemetry_handler();
	}
}
