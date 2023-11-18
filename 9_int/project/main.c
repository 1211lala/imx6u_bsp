#include "main.h"


volatile uint32_t key_num = 0;

int main(void)
{
    system_interrupt_init();
    system_clk_enable();
    my_imx6ull_clk_init();
    my_led_gpio_init();
    my_beep_gpio_init();
    gpio_exti_init();
    while(1) 
    {
        static volatile uint32_t count;
        // key_num = my_key_get_status();
        // if(key_num == KEY0_DOWN)
        // {
        //     my_beep_control(ON);
        // }
        // else if(key_num == KEY0_NONE)
        // {
        //     my_beep_control(OFF);
        // }

        my_delay_ms(10);
        if(count++ >= 100) /* 1s */
        {
            static volatile uint8_t flag=0;
            count = 0;
            if(flag == 0)
            {
                flag = 1;
                my_led_control(LED0, ON);
            }
            else
            {
                flag = 0;
                 my_led_control(LED0, OFF);
            } 
        }
    }
    return 0;
}