#ifndef _BSP_EXTI_H_
#define _BSP_EXTI_H_

#include "main.h"

void my_gpio_exti_init(void(*gpio_callback)(uint32_t, void*));
#endif // ! _BSP_EXTI_H_