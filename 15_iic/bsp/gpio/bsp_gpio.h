#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_

#include "main.h"

/* 设置GPIO的输入输出方向 */
typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput = 0U,
    kGPIO_DigitalOutput= 1U
} gpio_pin_direction_t;

/* 输出有效 设置输出电平 */
typedef enum _gpio_pin_level
{
    kGPIO_PinLevelLow = 0U,
    kGPIO_PinLevelHigh= 1U
} gpio_pin_level_t;

/* 中断触发类型枚举 ICR1 2 寄存器 */
typedef enum _gpio_interuupt_mode
{
    int_nomode = 0,
    int_low_level,
    int_high_level,
    int_rising,
    int_falling,
    int_falling_rising
}gpio_interuupt_mode_t;

/* GPIO调用结构体 */
typedef struct _gpio_pin_config
{
    gpio_pin_direction_t direction;
    gpio_pin_level_t PinLevel;
    gpio_interuupt_mode_t InterruptMode;
}gpio_pin_config_t;

void my_gpio_init(GPIO_Type * base, int pin, gpio_pin_config_t* config);
void my_gpio_write_pin(GPIO_Type* base, int pin, uint8_t value);
gpio_pin_level_t my_gpio_read_pin(GPIO_Type* base, int pin);
void gpio_int_config(GPIO_Type* base, uint32_t pin, gpio_interuupt_mode_t intmode);
void gpio_int_enable(GPIO_Type* base, uint32_t pin);
void gpio_int_disenable(GPIO_Type* base, uint32_t pin);
void gpio_int_clearflag(GPIO_Type* base, uint32_t pin);
void gpio_int_config(GPIO_Type* base, uint32_t pin, gpio_interuupt_mode_t intmode);
#endif // ! _BSP_GPIO_H_