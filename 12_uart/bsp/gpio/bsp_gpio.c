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
    gpio_int_config(base, pin, config->InterruptMode);
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


/* GPIO中断使能 IMR寄存器 */
void gpio_int_enable(GPIO_Type* base, uint32_t pin)
{
    base->IMR |= (1 << pin);
}

/* GPIO中断失能 IMR寄存器 */
void gpio_int_disenable(GPIO_Type* base, uint32_t pin)
{
    base->IMR &= ~(1 << pin);  
}

/* 清除中断标志位 ISR寄存器 写1清零 */
void gpio_int_clearflag(GPIO_Type* base, uint32_t pin)
{
    base->ISR |= (1 << pin);
}

void gpio_int_config(GPIO_Type* base, uint32_t pin, gpio_interuupt_mode_t intmode)
{
    /* 清零 EDGE_SEL EDGE_SEL对应的为为1 则ICR寄存器失效 上升和下降都会触发 */
    base->EDGE_SEL &= (1 << pin);

    volatile uint32_t *icr;
    /* ICR寄存器是两个位控制一个pin */
    if(pin < 16)
    {
        icr = &(base->ICR1);
    }else
    {
        icr = &(base->ICR2);
        pin = pin - 16;
    }
    switch(intmode)
    {
        case(int_nomode): break;
        case(int_low_level): *icr &= ~(3 << pin*2); break;
        case(int_high_level): *icr &= ~(3 << pin*2); *icr |= (1 << pin*2); break;
        case(int_rising): *icr &= ~(3 << pin*2); *icr |= (2 << pin*2); break;
        case(int_falling): *icr &= ~(3 << pin*2); *icr |= (3 << pin*2); break;
        case(int_falling_rising): base->EDGE_SEL |= (1 << pin*2);break;
    }
}