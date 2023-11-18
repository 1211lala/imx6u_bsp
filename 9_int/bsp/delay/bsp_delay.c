#include "bsp_delay.h"

void delay(volatile unsigned int n)
{
    /* 加 volatile 关键字会防止02优化时出现问题 */
    while(n--){}
}

void my_delay_ms(volatile uint32_t nms)
{
    while(nms--)
    {
        delay(0x7ff);
    }
}