#include "bsp_epit.h"


/* 初始化EPIT 默认不启动 */
void my_epit1_it_init(uint32_t psc, uint32_t load, uint32_t compare, void(*epit_callback)(uint32_t, void*))
{
    /* EPIT_CR寄存器 */
    EPIT1->CR = 0;
    EPIT1->CR |= (1 << 1);  /* 设置EPIT模式 */
    EPIT1->CR |= (1 << 2);  /* 使能比较中断 */
    EPIT1->CR |= (1 << 3);  /* 设置 SET_ADD_FORGET模式 */
    EPIT1->CR |= (psc << 4);/* 设置分频 */
    EPIT1->CR |= (1 << 24); /* 设置时钟源选择 */
    /* EPIT_LR寄存器 */
    EPIT1->LR = load;
    /* EPIT_LR寄存器 */
    EPIT1->CMPR = compare;

    /* 配置中断设置和中断函数 */
    GIC_EnableIRQ(EPIT1_IRQn);
    system_register_irqhandler(EPIT1_IRQn, epit_callback, NULL);
    EPIT1->CR |= (1 << 0); /* 启动定时器 */
}

/* 关闭定时器 */
void my_epit1_stop(void)
{
    EPIT1->CR &= ~(1 << 0); /* 关闭定时器 */
}


/* 开启定时器 */
void my_epit1_start(void)
{
    EPIT1->CR |= (1 << 0); /* 启动定时器 */
}   


/* 重启定时器 */
void my_epit1_restart(uint32_t load)
{
    my_epit1_stop();
    EPIT1->LR = load;   /* 设置新的load值 */
    my_epit1_start();
}

void my_epit1_clear_flag(void)
{
    EPIT1->SR |= (1 << 0);  /* 写一清零 */
}



