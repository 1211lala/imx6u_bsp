#ifndef _BSP_INTERRUPT_H_
#define _BSP_INTERRUPT_H_

#include "imx6u.h"

/* 中断处理函数 */
typedef void (*system_irq_handler_t)(uint32_t gicciar, void* param);

/* 中断处理函数结构体 */
typedef struct _sys_irq_handle
{
    system_irq_handler_t irqHandler;
    void *userParam;

}system_irq_handle_t;


void system_interrupt_init(void);
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void* param);


#endif // !_BSP_INTERRUPT_H_