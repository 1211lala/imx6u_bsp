
#include "main.h"

/* 使能所有外设时钟 */
void system_clk_enable(void)
{
    CCM_CCGR1 = 0xffffffff;
    CCM_CCGR2 = 0xffffffff;
    CCM_CCGR3 = 0xffffffff;
    CCM_CCGR4 = 0xffffffff;
    CCM_CCGR5 = 0xffffffff;
    CCM_CCGR6 = 0xffffffff;
}

void led_gpio_init(void)
{
    SW_MUX_CTL_PAD_GPIO1_IO03 = 0x00000005;     /* 设置复用 */
    SW_PAD_CTL_PAD_GPIO1_IO03 = 0x000010b0;     /* 设置电气*/

    GPIO1_GDIR = 0x00000008;    /* 设置输入输出 */               
    GPIO1_DR = 0x00000000;      /* 设置高低电平 */ 
}

void led_on(void)
{
    GPIO1_DR &= ~(1<<3);    /* 输出低电平 第三位赋0*/
}

void led_off(void)
{
    GPIO1_DR |= (1<<3); /* 输出高电平 第三位赋1*/
}


void delay_1ms(void)
{
    uint32_t delay_time = 0x7ff;
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


int main(void)
{
    system_clk_enable();
    led_gpio_init();
    while(1)
    {
        led_on();
        delay_ms(200);
        led_off();
        delay_ms(200);
    }
    return 0;
}