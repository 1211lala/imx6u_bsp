#include "bsp_exti.h"

void my_gpio_exti_init(void)
{
    /* 设置复用 */
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    /* 设置输入的电气属性 */
    /* IOMUXC_SW_PAD_CTL_PAD_UART1_CTS_B 设置为 0000 0000 0000 0000 1111 0000 1000 0000 */
    /* 看参考手册 */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xf080);

    gpio_pin_config_t key_int_config;

    key_int_config.direction = kGPIO_DigitalInput;
    key_int_config.InterruptMode = int_falling;
    my_gpio_init(GPIO1, 18, &key_int_config);
}

void my_gpio_set_gic_callback(uint32_t IRQn, void(*gpio_callback)(uint32_t, void*))
{
    /* 使能GIC 相当于NVIC */
    GIC_EnableIRQ(IRQn);
    system_register_irqhandler(IRQn, gpio_callback,NULL);
    gpio_int_enable(GPIO1, 18); /* 开启中断 */
}



