#include "MCIMX6Y2.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"

/* 使能所有外设时钟 */
void system_clk_enable(void)
{
    CCM->CCGR0 = 0xffffffff;
    CCM->CCGR1 = 0xffffffff;
    CCM->CCGR2 = 0xffffffff;
    CCM->CCGR3 = 0xffffffff;
    CCM->CCGR4 = 0xffffffff;
    CCM->CCGR5 = 0xffffffff;
    CCM->CCGR6 = 0xffffffff;
}

void led_gpio_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10b0);

    GPIO1->GDIR = 0x08;                     /* 设置为输出模式 */
    GPIO1->DR = 0x00;                       /* 设置为低电平 */
}

void led_on(void)
{
    GPIO1->DR &= ~(1<<3);
}

void led_off(void)
{
    GPIO1->DR |= (1<<3);
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


uint32_t waittime = 1000;

int main(void)
{
    system_clk_enable();
    led_gpio_init();
    while(1)
    {
        led_on();
        delay_ms(waittime);
        led_off();
        delay_ms(waittime);
    }
    return 0;
}