#ifndef _BSP_BEEP_H_
#define _BSP_BEEP_H_

#include "main.h"

void my_beep_gpio_init(void);
void my_beep_control(volatile uint8_t status);

#endif // ! _BSP_BEEP_H_