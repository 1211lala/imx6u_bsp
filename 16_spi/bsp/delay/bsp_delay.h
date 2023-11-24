#ifndef _BSP_DELAY_H_
#define _BSP_DELAY_H_

#include "main.h"

void my_gpt_it_init(uint32_t psc, uint32_t ocr_value, void (*gpt1_callback)(uint32_t, void *));
uint8_t my_gpt1_get_of1_flag(void);
void my_gpt1_clear_of1_flag(void);
void my_gpt1_stop(void);
void my_gpt1_start(void);
void my_gpt_delay_init(uint32_t psc, uint32_t ocr_value);
void my_delay_us(uint32_t value);
void my_delay_ms(uint32_t value);
#endif // ! _BSP_DELAY_H_