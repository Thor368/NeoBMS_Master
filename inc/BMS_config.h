#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f10x.h"

#include "Battery_config.h"

#define TXBUFFERSIZE   1500
#define RXBUFFERSIZE   100

extern volatile uint32_t Tick;

extern volatile uint8_t RxBuffer[RXBUFFERSIZE];
extern volatile uint8_t TxBuffer[TXBUFFERSIZE];
extern volatile uint32_t TXWp, TXRp, RXWp, RXRp;

void _delay_ms(uint32_t time);
void _delay_us(uint32_t time);
