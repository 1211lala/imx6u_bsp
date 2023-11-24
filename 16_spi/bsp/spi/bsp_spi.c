#include "bsp_spi.h"


void spi_enable(ECSPI_Type* base)
{
    base->CONREG |= (1 << 0);
}



void spi_disable(ECSPI_Type* base)
{
    base->CONREG &= ~(1 << 0);
}


void spi_init(ECSPI_Type* base)
{
    base->CONREG = 0;
    spi_enable(base);
    base->CONREG |= (1 << 3);   /* 数据立刻发送 */
    base->CONREG |= (1 << 4);   /* 做主机 */
    base->CONREG |= (7 << 20);

    base->CONFIGREG = 0;

    base->PERIODREG = 0x2000;

    base->CONREG &= ~(0xf << 12);   /* 一级分频和二级分频清零 再设置 */
    base->CONREG &= ~(0xf << 8);

    base->CONREG |= (0x9 << 12);     /* 设置10分频 60/10 = 6M*/ 
}


uint8_t spi_writeread_byte(ECSPI_Type* base, uint8_t Txchar)
{
    uint32_t rx_data;
    uint32_t tx_data;

    tx_data = Txchar;                                           
    base->CONREG &= ~(3 << 18);     /* 选择通道0 ？？？？*/
    base->CONREG |= (0 << 18);

    while((base->STATREG & (1 << 0)) == 0); /* 等待发送 */
    base->TXDATA = tx_data;

    while((base->STATREG & (1 << 3)) == 0); /* 等待读取 */
    rx_data = base->RXDATA;

    return rx_data;
}