#ifndef _BSP_LED_H_
#define _BSP_LED_H_


#include "main.h"


#define LED0    0

void my_led_gpio_init(void);
void my_led_control(uint8_t led_id, uint8_t status);




#endif // ! _BSP_LED_H_