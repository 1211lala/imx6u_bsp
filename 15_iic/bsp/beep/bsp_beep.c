#include "bsp_beep.h"

void my_beep_gpio_init(void)
{
    /* 配置复用为GPIO5_IO01 */
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);
    /* 配置电气属性 */
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x10b0);
    /* GDIR输入输出配置位 0:输入  1：输出 */
    GPIO5->GDIR |= (1<<1); 
    /* DR高低电平配置位 0:低电平  1：高电平 */ 
    GPIO5->DR |= (1<<1);
}

void my_beep_control(volatile uint8_t status)
{
    if(status == ON)
    {
        GPIO5->DR &= ~(1<<1);
    }
    else if(status == OFF)
    {
        GPIO5->DR |= (1<<1);
    }
}