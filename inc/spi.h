#include "BMS_config.h"

#define SPI_CS_RES     GPIOB->BSRR = (SPI_CS << 16)
#define SPI_CS_SET     GPIOB->BSRR = SPI_CS

#define SPI_MOSI       GPIO_Pin_5
#define SPI_MISO       GPIO_Pin_4
#define SPI_SCK        GPIO_Pin_3
#define SPI_CS         GPIO_Pin_6

void InitSPI(void);

uint8_t SPI_Transceive(uint8_t* data, uint8_t count);
