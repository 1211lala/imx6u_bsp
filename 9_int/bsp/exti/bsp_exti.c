#include "bsp_exti.h"

void gpio_exti_init(void)
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
    /* 使能GIC 相当于NVIC */
    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);

    system_register_irqhandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)gpio1_io18_irqhandler,NULL);

    gpio_int_enable(GPIO1, 18);
}

void gpio1_io18_irqhandler(void)
{
    static uint8_t flag = 0;

    if(flag == 0)
    {
        my_beep_control(HIGH);
        flag = 1;
    }
    else
    {    
        my_beep_control(LOW);
        flag = 0;
    }

    my_delay_ms(20);
    gpio_int_clearflag(GPIO1, 18);
}