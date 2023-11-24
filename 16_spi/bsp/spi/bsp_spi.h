#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_


#include "main.h"

void spi_enable(ECSPI_Type* base);
void spi_disable(ECSPI_Type* base);
void spi_init(ECSPI_Type* base);
uint8_t spi_writeread_byte(ECSPI_Type* base, uint8_t Txchar);


#endif // ! _BSP_SPI_H_