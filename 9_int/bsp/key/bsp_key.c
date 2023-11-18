#include "bsp_key.h"

void my_key_gpio_init(void)
{
    /* 设置复用 */
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    /* 设置输入的电气属性 */
    /* IOMUXC_SW_PAD_CTL_PAD_UART1_CTS_B 设置为 0000 0000 0000 0000 1111 0000 1000 0000 */
    /* 看参考手册 */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xf080);

    /* GDIR输入输出配置位 0:输入  1：输出 */
    GPIO1->GDIR &= ~(1<<18); 
}

Key_Status my_key_get_status(void)
{
    volatile uint8_t flag = 0;
    /* 在未按下之前按键为高电平右移18位与上0x01 == 0x01 */
    if(flag ==0 && ((GPIO1->DR >> 18) & 0x01) != 0x01)
    {
        flag = 1;
        return KEY0_DOWN;
    }
    /* 在没有按键按下时复位标志位 */
    if(((GPIO1->DR >> 18) & 0x01) == 0x01)
    {
        flag = 0;
    }

    return KEY0_NONE;    
}