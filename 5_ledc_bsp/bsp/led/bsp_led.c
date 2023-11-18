#include "bsp_led.h"

/* led的引脚复用和电气设置 */
void led_gpio_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10b0);

    GPIO1->GDIR |= (1 << 3);                     /* 设置为输出模式 */
    GPIO1->DR &= ~(1 << 3);                       /* 设置为低电平 */
}

void led_on(void)
{
    GPIO1->DR &= ~(1<<3);
}

void led_off(void)
{
    GPIO1->DR |= (1<<3);
}


void led_switch(uint8_t led_id, uint8_t status)
{
    switch (led_id)
    {
        case(LED0):if(status == ON)
                    {
                        GPIO1->DR &= ~(1<<3);
                    }
                    else if(status == OFF)
                    {
                        GPIO1->DR |= (1<<3);
                    } break;
        default:
            break;
    }
}