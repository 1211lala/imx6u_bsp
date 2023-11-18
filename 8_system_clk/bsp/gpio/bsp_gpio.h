#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_

#include "main.h"

typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput = 0U,
    kGPIO_DigitalOutput= 1U
} gpio_pin_direction_t;

typedef enum _gpio_pin_level
{
    kGPIO_PinLevelLow = 0U,
    kGPIO_PinLevelHigh= 1U
} gpio_pin_level_t;


typedef struct _gpio_pin_config
{
    gpio_pin_direction_t direction;
    gpio_pin_level_t PinLevel;

}gpio_pin_config_t;



void my_gpio_init(GPIO_Type * base, int pin, gpio_pin_config_t* config);
void my_gpio_write_pin(GPIO_Type* base, int pin, uint8_t value);
gpio_pin_level_t my_gpio_read_pin(GPIO_Type* base, int pin);


#endif // ! _BSP_GPIO_H_