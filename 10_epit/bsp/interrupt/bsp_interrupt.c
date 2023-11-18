#include "bsp_interrupt.h"

static uint32_t irqNesting;
/* 中断向量表 */
static system_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS];

/* 默认中断处理函数 */
void system_default_irqFuntion(uint32_t gicciar, void* param)
{
    while(1)
    {

    }
}
/* 中断表初始化 */
void system_irqTable_init(void)
{
    uint8_t i = 0;
    irqNesting = 0;
    for(i=0; i<NUMBER_OF_INT_VECTORS; i++)
    {
        irqTable[i].userParam = NULL;
        irqTable[i].irqHandler = system_default_irqFuntion;
    }
}


/* 注册中断处理函数 */
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void* param)
{
    irqTable[irq].irqHandler = handler;
    irqTable[irq].userParam = param;
}

void system_interrupt_init(void)
{
    GIC_Init();
    system_irqTable_init();
    // /* 中断向量偏移 以及在启动文件中设置了 Reset_Handler */
    __set_VBAR(0x87800000);

}

/* 发生中断时汇编调用的函数 */
void system_irqhandler(uint32_t gicciar)
{
    uint32_t intNum  = gicciar & 0x3ff;

    /* IMX的中断是0-160 */
    if(intNum >= NUMBER_OF_INT_VECTORS) return ;

    irqNesting += 1;
    /* 执行对应中断函数 */
    irqTable[intNum].irqHandler(intNum, irqTable[intNum].userParam);
    irqNesting -= 1;
}