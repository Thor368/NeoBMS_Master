#include "spi.h"
#include "BMS_config.h"

void InitSPI(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef GPIO_Inittd;
	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	// Software CS Output
	GPIO_Inittd.GPIO_Pin = SPI_CS;
	GPIO_Inittd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Inittd.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_Inittd);

	// Alternate Function Outputs
	GPIO_Inittd.GPIO_Pin = SPI_SCK | SPI_MOSI;
	GPIO_Inittd.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_Inittd);

	GPIOB->ODR |= SPI_SCK | SPI_MOSI;

	// Inputs
	GPIO_Inittd.GPIO_Pin = SPI_MISO;
	GPIO_Inittd.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_Inittd);

	SPI_CS_RES;

	SPI_InitTypeDef   SPI_InitStructure;

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);
}

uint8_t  SPI_Transceive(uint8_t* data, uint8_t count)
{
	SPI_I2S_ReceiveData(SPI1);

	while (count)
	{
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(SPI1, *data);

		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		*(data++) = (uint8_t) SPI_I2S_ReceiveData(SPI1);

		count--;
	}

	return 0;
}
