
#include "main.h"

#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"


volatile uint32_t waittime = 500;

int main(void)
{
    system_clk_enable();
    led_gpio_init();
    while(1)
    {
        led_switch(LED0, OFF);
        delay_ms(waittime);
        led_switch(LED0, ON);
        delay_ms(waittime);
    }
    return 0;
}