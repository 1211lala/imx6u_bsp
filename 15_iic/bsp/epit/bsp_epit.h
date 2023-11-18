#ifndef _BSP_EPIT_H_
#define _BSP_EPIT_H_

#include "main.h"

void my_epit1_it_init(uint32_t psc, uint32_t load, uint32_t compare, void(*epit_callback)(uint32_t, void*));
void my_epit1_stop(void);
void my_epit1_start(void);
void my_epit1_restart(uint32_t load);
void my_epit1_clear_flag(void);

#endif // ! _BSP_EPIT_H_