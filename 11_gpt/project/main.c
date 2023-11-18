#include "main.h"


int main(void)
{
    system_interrupt_init();    /* 设置中断向量表和偏移 */
    system_clk_enable();        /* 设置所有时钟使能 */
    my_imx6ull_clk_init();      /* 设置具体arm时钟和一些外设时钟的频率*/
    my_led_gpio_init();         /* 设置led的初始化 默认关闭 */
    my_beep_gpio_init();        /* 设置蜂鸣器的初始化 默认关闭*/
    /* GPIO中断模式的初始化 默认打开中断 */
    my_gpio_exti_init(gpio1_16_18_irqhandler);        
    my_epit1_it_init(66-1,10000, 0, epit1_irqhander); /* EPIT1定时器的初始化，默认打开中断 */
    my_gpt_delay_init(66-1, 0xffffffff);
    while(1) 
    {

        static uint32_t state = 1;

        state = !state;
        my_led_control(LED0, state);

        my_delay_us(100);

        // static volatile uint32_t count;
        // key_num = my_key_get_status();
        // if(key_num == KEY0_DOWN)
        // {
        //     my_beep_control(ON);
        // }
        // else if(key_num == KEY0_NONE)
        // {
        //     my_beep_control(OFF);
        // }

        // my_delay_ms(10);
        // if(count++ >= 100) /* 1s */
        // {
        //     static volatile uint8_t flag=0;
        //     count = 0;
        //     if(flag == 0)
        //     {
        //         flag = 1;
        //         my_led_control(LED0, ON);
        //     }
        //     else
        //     {
        //         flag = 0;
        //          my_led_control(LED0, OFF);
        //     } 
        // }
    }
    return 0;
}


void epit1_irqhander(uint32_t gicciar, void* param)
{
    static uint32_t state = 0;

    if(EPIT1->SR &(1 << 0)) /* 判断中断标志位是否置一 */
    {
        my_epit1_stop();
        if(my_gpio_read_pin(GPIO1, 18) == LOW)
        {
            state = !state;
            my_beep_control(state);
        }
    }
    EPIT1->SR |= (1 << 0);  /* 清除EPIT1中断标志位 */
}


void gpio1_16_18_irqhandler(uint32_t gicciar, void* param)
{
    my_epit1_restart(10000);

    gpio_int_clearflag(GPIO1, 18);
}
