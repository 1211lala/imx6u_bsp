
#include "main.h"


volatile uint32_t key_num = 0;

int main(void)
{
    system_clk_enable();
    my_led_gpio_init();
    my_beep_gpio_init();
    my_key_gpio_init();
    while(1)
    {
        key_num = my_key_get_status();
        if(key_num == KEY0_DOWN)
        {
            my_led_control(LED0, ON);
            my_beep_control(ON);
        }
        else if(key_num == KEY0_NONE)
        {
            my_led_control(LED0, OFF);
            my_beep_control(OFF);
        }
    }
    return 0;
}