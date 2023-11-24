#include "bsp_clk.h"

/* 使能所有外设时钟 */
void system_clk_enable(void)
{
    CCM->CCGR0 = 0xffffffff;
    CCM->CCGR1 = 0xffffffff;
    CCM->CCGR2 = 0xffffffff;
    CCM->CCGR3 = 0xffffffff;
    CCM->CCGR4 = 0xffffffff;
    CCM->CCGR5 = 0xffffffff;
    CCM->CCGR6 = 0xffffffff;
}


void my_imx6ull_clk_init(void)
{
    /***************************************设置PLL1*****************************************/
    /* 此位为1当前使用的是 step_clk */
    if(((CCM->CCSR) >> 2) & 0x01 )
    {
        /* 不需要操作 */
    }
    else    /* 此位为0当前使用的是 pll1_main_clk */
    {
        CCM->CCSR &= ~(1 << 8);     /* 设置外部时钟设置为step_clk的来源*/
        CCM->CCSR |= (1 << 2);      /* 设置step_clk 为 pll1_sw_clk 的来源 */  
    }
    /*
    1.首先使能时钟输出
    2.我们目标时钟为792Mhz  
      公式为 24 * div_select/2.0 = 792 推出 div_select == 66   
      所以先 bit(6,0) 写入66就可以
    */
    CCM_ANALOG->PLL_ARM = (1 << 13)|((66 << 0) & 0x7f);
    // CCM_ANALOG->PLL_ARM |= 66 & 0x7f;
    /* 时钟设置完成切换回来 */
    CCM->CCSR &= ~(1 << 2);     /* 设置pll1_main_clk 为 pll1_sw_clk 的来源 */
    /* 设置1分频 */
    CCM->CACRR = 0;


    /***************************************设置PLL2的四路PFD*****************************************/
    volatile uint32_t reg;
    reg = CCM_ANALOG->PFD_528;      /* 读取 CCM_ANALOG->PFD_528 的值*/
    reg &= ~0x3f3f3f3f;             /* 将对应的位清零 */

    reg |=  (32 << 24);             /* 设置PLL2_PFD3为297MHz*/
    reg |=  (24 << 16);             /* 设置PLL2_PFD2为396MHz*/
    reg |=  (16 << 8);              /* 设置PLL2_PFD1为594MHz*/
    reg |=  (27 << 0);              /* 设置PLL2_PFD0为352MHz*/

    CCM_ANALOG->PFD_528 = reg;

    /***************************************设置PLL3的四路PFD*****************************************/
    reg = 0;
    reg = CCM_ANALOG->PFD_480;      /* 读取 CCM_ANALOG->PFD_480 的值*/
    reg &= ~0x3f3f3f3f;             /* 将对应的位清零 */
    reg |=  (19 << 24);             /* 设置PLL3_PFD3为454.7MHz*/
    reg |=  (17 << 16);             /* 设置PLL3_PFD2为508.2MHz*/
    reg |=  (16 << 8);              /* 设置PLL3_PFD1为540MHz*/
    reg |=  (12 << 0);              /* 设置PLL3_PFD1为740MHz*/
    
    CCM_ANALOG->PFD_480 = reg;


    /***************************************AHB_CLK_ROOT(6-132Mhz)*****************************************/
    CCM->CBCMR &= ~(0x03 << 18);        /* CBCMR[PRE_PERIPH_CLK_SEL] */
    CCM->CBCMR |= (0x01 << 18);         

    CCM->CBCDR &= ~(1 << 25);           /* CBCDR[PERIPH_CLK_SEL] */
    while(CCM->CDHIPR & (1 << 5));      /* 等待设置完成 */
#if 0
    CCM->CBCDR &= ~(0x07 << 10);        /* CBCDR[AHB_PODF] */
    CCM->CBCDR |= (0x02 << 10);
    while(CCM->CDHIPR & (1 << 1));      /* 等待设置完成 */
#endif



     /***************************************PERCLK_CLK_ROOT(3-66Mhz) 和 IPG_CLK_ROOT(3-66Mhz) *****************************************/
    CCM->CBCDR &= ~(0x03 << 8); /* CBCDR[IPG_PODF] */
    CCM->CBCDR |= (0x01 << 8);  /* 设置二分频 132/2 */

    CCM->CSCMR1 &= ~(0x01 << 6);    /* CSCMR1[PERCLK_CLK_SEL]*/
    CCM->CSCMR1 &= ~(0x07 << 0);    /* CSCMR1[PERCLK_PODF] 1分频 66Mhz/1*/



    // /*****************************************设置串口时钟源80Mhz************************************************/
    CCM->CSCDR1 &= ~(1 << 6);       /* CSCDR1[UART_CLK_SEL]设置时钟来源为80Mhz*/
    CCM->CSCDR1 &= ~(0x3f << 0);    /* CSCDR1[UART_CLK_PODF]设置为一分频*/

    /*********************************************设置SPI的时钟源为60Mhz*********************************************/
    CCM->CSCDR2 &= ~(1 << 18);      /* 设置spi的时钟源来之PLL3(480)/8*/
    CCM->CSCDR2 &= ~(0x3f << 19);   /* 设置一份频 */
}