#ifndef _BSP_EPIT_H_
#define _BSP_EPIT_H_

#include "main.h"

void my_epit_init(uint32_t psc, uint32_t load, uint32_t compare);
void my_epit1_stop(void);
void my_epit1_start(void);
void my_epit1_restart(uint32_t load);
void my_epit1_clear_flag(void);
void my_epit_set_gic_callback(uint32_t IRQn, void(*epit_callback)(uint32_t, void*));

#endif // ! _BSP_EPIT_H_