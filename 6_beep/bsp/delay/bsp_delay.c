#include "bsp_delay.h"

void delay_1ms(void)
{
    /* 加 volatile 关键字会防止02优化时出现问题 */
    volatile uint32_t delay_time = 0x7ff;
    while(delay_time)
    {
        delay_time -= 1;
    }
}

void delay_ms(volatile uint32_t nms)
{
    while(nms)
    {
        nms -= 1;
        delay_1ms();
    }
}