
#include "main.h"

#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"

volatile uint32_t waittime = 20;

int main(void)
{
    system_clk_enable();
    my_led_gpio_init();
    my_beep_gpio_init();
    while(1)
    {
        my_led_control(LED0, OFF);
        my_beep_control(OFF);
        delay_ms(waittime);
        my_led_control(LED0, ON);
        my_beep_control(ON);
        delay_ms(waittime);
    }
    return 0;
}