#include "main.h"
#include "stdio.h"

volatile uint8_t lcd_buf[100] = {0};
volatile uint32_t color_buf[] = {LCD_BLUE, LCD_RED, LCD_GREEN, LCD_YELLOW};
int main(void)
{
    system_interrupt_init();    /* 设置中断向量表和偏移 */
    system_clk_enable();        /* 设置所有时钟使能 */
    my_imx6ull_clk_init();      /* 设置具体arm时钟和一些外设时钟的频率*/
    /* 设置led的初始化 默认关闭 */
    my_led_gpio_init();         
    /* 设置蜂鸣器的初始化 默认关闭 */
    my_beep_gpio_init();        
    /* GPIO中断模式的初始化 默认打开中断 */
    my_gpio_exti_init(gpio1_16_18_irqhandler);
    /* EPIT1定时器的初始化，默认打开中断 */        
    my_epit1_it_init(66-1,10000, 0, epit1_irqhander); 
    /* 设置gpt定时器做延时不开启中断 */
    my_gpt_delay_init(66-1, 0xffffffff);
    /* 设置串口初始化 115200 不开启中断 */
    my_uart1_init(115200);
    /* LCD初始化 黄屏 */
    my_lcd_init();
    tftlcd_dev.backcolor = LCD_BLACK;
    tftlcd_dev.forecolor = LCD_RED;
    /* RTC初始化 */
    // my_rtc_init();
    ap3216c_init();
    while(1) 
    {
        uint16_t ir;
        uint16_t ps;
        uint16_t als;
        sprintf((char*)lcd_buf, "ir = %4d  ps= %4d als= %4d     ", ir, ps, als);
        lcd_show_string(100, 100, 1200, 32, 32 , (char*)lcd_buf);
        ap3216c_readdata(&ir, &ps, &als);
        my_delay_ms(100);
        printf("%d %d %d \r\n", ir, ps, als );
    }
    return 0;
}


void epit1_irqhander(uint32_t gicciar, void* param)
{
    static uint32_t state = 0;

    if(EPIT1->SR &(1 << 0)) /* 判断中断标志位是否置一 */
    {
        my_epit1_stop();
        if(my_gpio_read_pin(GPIO1, 18) == LOW)
        {
            state = !state;
            my_beep_control(state);
        }
    }
    EPIT1->SR |= (1 << 0);  /* 清除EPIT1中断标志位 */
}


void gpio1_16_18_irqhandler(uint32_t gicciar, void* param)
{
    my_epit1_restart(10000);
    gpio_int_clearflag(GPIO1, 18);
}
