#ifndef _BSP_LED_H_
#define _BSP_LED_H_


#include "main.h"


#define LED0    0

void led_gpio_init(void);
void led_on(void);
void led_off(void);
void led_switch(uint8_t led_id, uint8_t status);




#endif // ! _BSP_LED_H_