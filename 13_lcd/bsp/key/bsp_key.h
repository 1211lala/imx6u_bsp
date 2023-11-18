#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

#include "main.h"

typedef enum
{
    KEY0_NONE = 0,
    KEY0_DOWN
}Key_Status;

void my_key_gpio_init(void);
Key_Status my_key_get_status(void);


#endif // ! _BSP_KEY_H_