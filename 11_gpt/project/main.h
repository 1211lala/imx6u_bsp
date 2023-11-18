#ifndef _MAIN_H_
#define _MAIN_H_

#include "imx6u.h"


#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_gpio.h"
#include "bsp_interrupt.h"
#include "bsp_exti.h"
#include "bsp_epit.h"

void gpio1_16_18_irqhandler(uint32_t gicciar, void* param);
void epit1_irqhander(uint32_t gicciar, void* param);
void gpt1_irqhander(uint32_t gicciar, void* param);
#endif // ! _MAIN_H_