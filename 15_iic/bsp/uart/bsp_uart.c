#include "bsp_uart.h"


/* 在设置这个函数前还要设置串口的时钟源选择为80Mhz */
void my_uart1_init(uint32_t baud)
{

    /* TX --> UART1_TX */
    /* RX --> UART1_RX */
    /* 初始化uart1引脚 */
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0);
    /* 设置输入的电气属性 */
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0x10b0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, 0x10b0);

    my_uart_disable(UART1);
    my_uart_softreset(UART1);

    UART1->UCR1 = 0;
    UART1->UCR1 &= ~(1<<14);

    UART1->UCR2 |= (1 << 1);    /* 使能接收 */
    UART1->UCR2 |= (1 << 2);    /* 使能发送 */
    UART1->UCR2 |= (1 << 5);    /* 设置数据位为8位 */
    UART1->UCR2 &= ~(1 << 6);   /* 设置一个停止位 */
    UART1->UCR2 &= ~(1 << 8);   /* 不使用校验 */
    UART1->UCR2 |= (1 << 14);   /* 不使用RTS引脚 */

    UART1->UCR3 |= (1 << 2);
    /* 设置波特率*/
    #if 0
    UART1->UFCR &= ~(7 << 7);     
    UART1->UFCR = 5<<7; //ref freq等于ipg_clk/1=80Mhz
    /** 一定要先设置UBIR **/
    UART1->UBIR = 1;
    UART1->UBMR = 85;
    #endif
    uart_setbaudrate(UART1, baud, 80000000);

    my_uart_enable(UART1);
}


void my_uart_disable(UART_Type *base)
{
    base->UCR1 &= ~(1 << 0);
}

void my_uart_enable(UART_Type *base)
{
    base->UCR1 |= (1 << 0);
}

/* 复位 */
void my_uart_softreset(UART_Type *base)
{
    base->UCR2 &= ~(1 << 0);
    while((base->UCR2 & 0x01) == 0);
}

/* 串口发送一个数据 */
void putc(uint8_t c)
{
    /* USR2的比特3位 为0是没发送完，1是发送完成*/
    while(((UART1->USR2 >> 3) & 0x01) == 0);
    /* UTXD是发送缓存寄存器 */
    UART1->UTXD = c;
}

void puts(uint8_t* str)
{
    while(*str)
    {
        putc(*str++);
    }
}
/* 串口接收一个字符 */
unsigned char getc(void)
{
    /* USR2的比特0位 为0等待读取，1可以读取 */
    while(((UART1->USR2) >> 0 & 0x01) == 0);
    /* UTXD是发送缓存寄存器 */
    return UART1->URXD;
}

/*
 * @description 		: 波特率计算公式 需要修改makefile添加数学包
 *    			  	  	  可以用此函数计算出指定串口对应的UFCR，
 * 				          UBIR和UBMR这三个寄存器的值
 * @param - base		: 要计算的串口。
 * @param - baudrate	: 要使用的波特率。
 * @param - srcclock_hz	:串口时钟源频率，单位Hz
 * @return		: 无
 */
void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz)
{
    uint32_t numerator = 0u;		//分子
    uint32_t denominator = 0U;		//分母
    uint32_t divisor = 0U;
    uint32_t refFreqDiv = 0U;
    uint32_t divider = 1U;
    uint64_t baudDiff = 0U;
    uint64_t tempNumerator = 0U;
    uint32_t tempDenominator = 0u;

    /* get the approximately maximum divisor */
    numerator = srcclock_hz;
    denominator = baudrate << 4;
    divisor = 1;

    while (denominator != 0)
    {
        divisor = denominator;
        denominator = numerator % denominator;
        numerator = divisor;
    }

    numerator = srcclock_hz / divisor;
    denominator = (baudrate << 4) / divisor;

    /* numerator ranges from 1 ~ 7 * 64k */
    /* denominator ranges from 1 ~ 64k */
    if ((numerator > (UART_UBIR_INC_MASK * 7)) || (denominator > UART_UBIR_INC_MASK))
    {
        uint32_t m = (numerator - 1) / (UART_UBIR_INC_MASK * 7) + 1;
        uint32_t n = (denominator - 1) / UART_UBIR_INC_MASK + 1;
        uint32_t max = m > n ? m : n;
        numerator /= max;
        denominator /= max;
        if (0 == numerator)
        {
            numerator = 1;
        }
        if (0 == denominator)
        {
            denominator = 1;
        }
    }
    divider = (numerator - 1) / UART_UBIR_INC_MASK + 1;

    switch (divider)
    {
        case 1:
            refFreqDiv = 0x05;
            break;
        case 2:
            refFreqDiv = 0x04;
            break;
        case 3:
            refFreqDiv = 0x03;
            break;
        case 4:
            refFreqDiv = 0x02;
            break;
        case 5:
            refFreqDiv = 0x01;
            break;
        case 6:
            refFreqDiv = 0x00;
            break;
        case 7:
            refFreqDiv = 0x06;
            break;
        default:
            refFreqDiv = 0x05;
            break;
    }
    /* Compare the difference between baudRate_Bps and calculated baud rate.
     * Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)).
     * baudDiff = (srcClock_Hz/divider)/( 16 * ((numerator / divider)/ denominator).
     */
    tempNumerator = srcclock_hz;
    tempDenominator = (numerator << 4);
    divisor = 1;
    /* get the approximately maximum divisor */
    while (tempDenominator != 0)
    {
        divisor = tempDenominator;
        tempDenominator = tempNumerator % tempDenominator;
        tempNumerator = divisor;
    }
    tempNumerator = srcclock_hz / divisor;
    tempDenominator = (numerator << 4) / divisor;
    baudDiff = (tempNumerator * denominator) / tempDenominator;
    baudDiff = (baudDiff >= baudrate) ? (baudDiff - baudrate) : (baudrate - baudDiff);

    if (baudDiff < (baudrate / 100) * 3)
    {
        base->UFCR &= ~UART_UFCR_RFDIV_MASK;
        base->UFCR |= UART_UFCR_RFDIV(refFreqDiv);
        base->UBIR = UART_UBIR_INC(denominator - 1); //要先写UBIR寄存器，然后在写UBMR寄存器，3592页 
        base->UBMR = UART_UBMR_MOD(numerator / divider - 1);
    }
}


void raise(int sig_nr)
{

}