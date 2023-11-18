#include "bsp_gpio.h"


void my_gpio_init(GPIO_Type * base, int pin, gpio_pin_config_t* config)
{
    if(config->direction == kGPIO_DigitalInput)
    {
        base->GDIR &= ~(1 << pin);
    }
    else if(config->direction == kGPIO_DigitalOutput)
    {
        base->GDIR |= (1 << pin);
    }

    if(config->PinLevel == kGPIO_PinLevelHigh)
    {
        base->DR |= (1 << pin);
    }
    else if(config->PinLevel == kGPIO_PinLevelLow)
    {
        base->DR &= ~(1 << pin);
    }
}

void my_gpio_write_pin(GPIO_Type* base, int pin, uint8_t value)
{
    if(value == HIGH)
    {
        base->DR |= (1 << pin);
    }
    else if(value == LOW)
    {
        base->DR &= ~(1 << pin);
    }
}

gpio_pin_level_t my_gpio_read_pin(GPIO_Type* base, int pin)
{
    if(((base->DR) >> pin) & 0x01)
    {
        return kGPIO_PinLevelHigh;
    }
    return kGPIO_PinLevelLow;
}