#include "bsp_delay.h"


/* 开启中断模式的初始化 开启定时器 */
void my_gpt_it_init(uint32_t psc, uint32_t ocr_value, void(*gpt1_callback)(uint32_t, void*))
{
    GPT1->CR = 0;   /* 先清零 */
    GPT1->CR |= (1 << 15);  /* 软复位定时器 */
    while((GPT1->CR >> 15) & 0x01); /* 等待软复位完成 */
    GPT1->CR |= (1 << 1);   /* 设置定时器被关闭时值为0,不保留原始值 */
    GPT1->CR |= (1 << 6);   /* 设置时钟源是ipg_clk */
    GPT1->CR &= ~(1 << 9);  /* 设置 Restart mode 模式 */

    GPT1->PR = (psc << 0);  /* 设置分频 */

    GPT1->OCR[0] = ocr_value;/* 设置输出比较通道一的比较值 */

    GPT1->IR |= (1 << 0);    /* 开启定时器的输出通道1中断 */ 

    /* 配置GIC */
    GIC_EnableIRQ(GPT1_IRQn);
    system_register_irqhandler(GPT1_IRQn, gpt1_callback,NULL);

    GPT1->CR |= (1 << 0);   /* 开启定时器 */
}


uint8_t my_gpt1_get_of1_flag(void)
{
    return GPT1->SR & (1 << 0);
}

/* 清除GPT1的输出通道1的中断标志位 */
void my_gpt1_clear_of1_flag(void)
{
    GPT1->SR |= (1 << 0);

}

void my_gpt1_stop(void)
{
    GPT1->CR &= ~(1 << 0);   /* 关闭定时器 */
}

void my_gpt1_start(void)
{
    GPT1->CR |= (1 << 0);   /* 开启定时器 */
}




/* 延时的初始化 不开启定时器  71min在右 */
void my_gpt_delay_init(uint32_t psc, uint32_t ocr_value)
{
    GPT1->CR = 0;   /* 先清零 */
    GPT1->CR |= (1 << 15);  /* 软复位定时器 */
    while((GPT1->CR >> 15) & 0x01); /* 等待软复位完成 */
    GPT1->CR |= (1 << 1);   /* 设置定时器被关闭时值为0,不保留原始值 */
    GPT1->CR |= (1 << 6);   /* 设置时钟源是ipg_clk */
    GPT1->CR &= ~(1 << 9);  /* 设置 Restart mode 模式 */

    GPT1->PR = (psc << 0);  /* 设置分频 */

    GPT1->OCR[0] = ocr_value;/* 设置输出比较通道一的比较值 */

    // GPT1->CR |= (1 << 0);   /* 开启定时器 */
}


 void my_delay_us(uint32_t value)
 {
     my_gpt1_stop();
     GPT1->CNT = 0;
     my_gpt1_start();
     while(GPT1->CNT < value);
     my_gpt1_stop();
 }
 void my_delay_ms(uint32_t value)
 {
     my_gpt1_stop();
     GPT1->CNT = 0;
     my_gpt1_start();
     while(GPT1->CNT < value * 1000);
     my_gpt1_stop();
 }