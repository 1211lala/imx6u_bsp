#include "main.h"
#include "stdio.h"

volatile uint32_t count = 0;
volatile uint8_t flag = 1;

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
    my_epit1_it_init(66-1,1000000, 0, epit1_irqhander); 
    /* 设置gpt定时器做延时不开启中断 */
    my_gpt_delay_init(66-1, 0xffffffff);
    /* 设置串口初始化 115200 不开启中断 */
    my_uart1_init(115200);
    
    my_lcd_init();
    lcd_clear(color_buf[0]);

    uint32_t i = 0;
    for(i=100; i< 1000; i++)
    {
        lcd_draw_point(i, 100, LCD_RED);
        lcd_draw_point(i, 200, LCD_RED);
        lcd_draw_point(i, 300, LCD_RED);
    }
    
    while(1) 
    {
        static uint8_t color = 0;
        if(flag == 0)
        {
            color += 1;
            if(color ==4) color = 0;
            lcd_clear(color_buf[color]);
            count += 1;
            lcd_clear(color_buf[color]);
            count += 1;
        }
        tftlcd_dev.backcolor = color_buf[color];
        sprintf(lcd_buf, "fps = %d", count);    
        lcd_show_string(0,0,300, 32, 32, lcd_buf);        
    }
    return 0;
}


void epit1_irqhander(uint32_t gicciar, void* param)
{
    static uint32_t state = 0;

    if(EPIT1->SR &(1 << 0)) /* 判断中断标志位是否置一 */
    {
        flag = 1;

        my_epit1_stop();
        printf("count = %d\r\n", count);
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
    my_epit1_restart(1000000);
    printf("按键按下\r\n");
    flag = 0;
    count = 0;
    gpio_int_clearflag(GPIO1, 18);
}
