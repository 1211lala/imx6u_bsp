#include "bsp_lcd.h"
#include "stdio.h"

struct tftlcd_typedef tftlcd_dev;



/* 读取屏幕ID  0000 0 M2 M1 M0 */
/* 1024 *600 对应的是 0  1   0 */
uint16_t lcd_read_id(void)
{
    /* 设置LCD_VSYNC为高电平，打开模拟开关 */
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0);
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0x10b0);

    gpio_pin_config_t lcdio_config;
    lcdio_config.direction = kGPIO_DigitalOutput;
    lcdio_config.PinLevel = kGPIO_PinLevelHigh;
    my_gpio_init(GPIO3, 3, &lcdio_config);

    /* 设置 LCD_DATA23 (R7)M0 为输入 */
    /* 设置 LCD_DATA15 (G7)M1 为输入 */
    /* 设置 LCD_DATA07 (B7)M2 为输入 */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_GPIO3_IO28, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_GPIO3_IO20, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_GPIO3_IO12, 0);  
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_GPIO3_IO28, 0xf080);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_GPIO3_IO20, 0xf080);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_GPIO3_IO12, 0xf080);

    lcdio_config.direction = kGPIO_DigitalInput;
    my_gpio_init(GPIO3, 28, &lcdio_config);
    my_gpio_init(GPIO3, 20, &lcdio_config);
    my_gpio_init(GPIO3, 12, &lcdio_config);


    uint8_t id = (uint8_t)my_gpio_read_pin(GPIO3, 12);
    id = (id << 1)| (uint8_t)my_gpio_read_pin(GPIO3, 20);
    id = (id << 1)| (uint8_t)my_gpio_read_pin(GPIO3, 28);

    /* 这里只写了自己的屏幕 */
    if(id == 0x02) return ATK7016;
    else return id;
}

/* 设置LCD的复用和电气属性 */
void lcd_gpio_init(void)
{
    /* 设置引脚复用 */
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA00_LCDIF_DATA00,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA01_LCDIF_DATA01,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA02_LCDIF_DATA02,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA03_LCDIF_DATA03,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA04_LCDIF_DATA04,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA05_LCDIF_DATA05,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA06_LCDIF_DATA06,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_LCDIF_DATA07,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA08_LCDIF_DATA08,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA09_LCDIF_DATA09,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA10_LCDIF_DATA10,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA11_LCDIF_DATA11,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA12_LCDIF_DATA12,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA13_LCDIF_DATA13,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA14_LCDIF_DATA14,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_LCDIF_DATA15,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA16_LCDIF_DATA16,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA17_LCDIF_DATA17,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA18_LCDIF_DATA18,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA19_LCDIF_DATA19,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA20_LCDIF_DATA20,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA21_LCDIF_DATA21,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA22_LCDIF_DATA22,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_LCDIF_DATA23,0);

	IOMUXC_SetPinMux(IOMUXC_LCD_CLK_LCDIF_CLK,0);	            /* 设置控制引脚 */
	IOMUXC_SetPinMux(IOMUXC_LCD_ENABLE_LCDIF_ENABLE,0);	
	IOMUXC_SetPinMux(IOMUXC_LCD_HSYNC_LCDIF_HSYNC,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_LCDIF_VSYNC,0);


	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_GPIO1_IO08,0);			/* 背光BL引脚      */
	IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0xF080);


	/* 2、配置LCD IO电气属性	
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 0 默认22K上拉
	 *bit [13]: 0 pull功能
	 *bit [12]: 0 pull/keeper使能 
	 *bit [11]: 0 关闭开路输出
	 *bit [7:6]: 10 速度100Mhz
	 *bit [5:3]: 111 驱动能力为R0/7
	 *bit [0]: 1 高转换率
	 */
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA00_LCDIF_DATA00,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA01_LCDIF_DATA01,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA02_LCDIF_DATA02,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA03_LCDIF_DATA03,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA04_LCDIF_DATA04,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA05_LCDIF_DATA05,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA06_LCDIF_DATA06,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_LCDIF_DATA07,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA08_LCDIF_DATA08,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA09_LCDIF_DATA09,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA10_LCDIF_DATA10,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA11_LCDIF_DATA11,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA12_LCDIF_DATA12,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA13_LCDIF_DATA13,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA14_LCDIF_DATA14,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_LCDIF_DATA15,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA16_LCDIF_DATA16,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA17_LCDIF_DATA17,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA18_LCDIF_DATA18,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA19_LCDIF_DATA19,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA20_LCDIF_DATA20,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA21_LCDIF_DATA21,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA22_LCDIF_DATA22,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_LCDIF_DATA23,0xB9);

	IOMUXC_SetPinConfig(IOMUXC_LCD_CLK_LCDIF_CLK,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_ENABLE_LCDIF_ENABLE,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_HSYNC_LCDIF_HSYNC,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_LCDIF_VSYNC,0xB9);


	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_GPIO1_IO08,0xB9);	/* 背光BL引脚 		*/

	/* 背光引脚初始化 */
    gpio_pin_config_t bl_config;
    
    bl_config.direction = kGPIO_DigitalOutput;
    bl_config.PinLevel = kGPIO_PinLevelHigh;
    my_gpio_init(GPIO1, 8, &bl_config);
	my_gpio_write_pin(GPIO1, 8, HIGH);
}

/* 设置LCD复位 */
void lcd_reset(void)
{
	LCDIF->CTRL |= (1 << 31);
}

/* 取消LCD复位 */
void lcd_no_reset(void)
{
	LCDIF->CTRL &= ~(1 << 31);
}

/* lcd使能 */
void lcd_enable(void)
{
	LCDIF->CTRL |= 1 << 0;
}

/* 初始化像素时钟 */
/*
	DIV_SELECT 时钟系数 27 - 54 
	LCDIF1_PRED 时钟分频 参数1-8
	LCDIF1_PODF 时钟分频 参数1-8

	lcd_clk = 24 * DIV_SELECT / LCDIF1_PRED / LCDIF1_PODF
*/
void lcd_clk_init(uint32_t DIV_SELECT, uint32_t LCDIF1_PRED, uint32_t LCDIF1_PODF)
{
	/* PLL5 = fref * (DIV_SELECT + NUM/DENOM) */

	/* 不使用小数分频 设置 NUM = 0 和 DENOM = 0 */
	CCM_ANALOG->PLL_VIDEO_NUM = 0;
	CCM_ANALOG->PLL_VIDEO_DENOM = 0;

	/* 设置DIV_SELECT 并设置分频 */
	CCM_ANALOG->PLL_VIDEO  = (DIV_SELECT << 0);
	CCM_ANALOG->PLL_VIDEO |= (1 << 13);	/* 手册指出要置一 */
	CCM_ANALOG->PLL_VIDEO |= (2 << 19);	/* 设置分频 */
	
	CCM_ANALOG->MISC2 &= ~( 3 << 30);	/* 设置分频 */
	CCM_ANALOG->MISC2 = 0 << 30;

	/* 设置 LCDIF1_PRED， LCDIF1_PODF 的分频值 和时钟选择器 */
	CCM->CSCDR2 &= ~(7 << 15); 	/* 设置时钟选择器 先清零 再赋值 */
	CCM->CSCDR2 |= (2 << 15);

	CCM->CSCDR2 &= ~(7 << 12); 	/* 设置分频 先清零 再赋值 */
	CCM->CSCDR2 |= ((LCDIF1_PRED - 1) << 12);

	CCM->CBCMR &= ~(7 << 23);	/* 设置分频 先清零 再赋值 */
	CCM->CBCMR |= ((LCDIF1_PODF - 1) << 23);
	

	CCM->CSCDR2 &= ~(7 << 9); 	/* 设置时钟选择器 先清零 再赋值 */
	CCM->CSCDR2 |= (0 << 9);
}


/* LCD初始化 */
void my_lcd_init(void)
{
    uint16_t id = lcd_read_id();
    printf("ID = 0x%04X\r\n",id);

    lcd_gpio_init();
	lcd_reset();
	my_delay_ms(10);
	lcd_no_reset();

	if(id == ATK7016)
	{
		tftlcd_dev.height = 600;
		tftlcd_dev.width = 1024;

		tftlcd_dev.hbpd = 140;
		tftlcd_dev.hfpd = 160;
		tftlcd_dev.hspw = 20;

		tftlcd_dev.vbpd = 20;
		tftlcd_dev.vfpd = 12;
		tftlcd_dev.vspw = 3;

		tftlcd_dev.pixsize = 4;
		tftlcd_dev.framebuffer = LCD_FRAMEBUF_ADDR;
		
		tftlcd_dev.forecolor = LCD_RED;
		tftlcd_dev.backcolor = LCD_BLACK;
	}else
	{
		printf("屏幕ID未定义\r\n");
	}
	lcd_clk_init(32, 3, 5);	/* 51.2Mhz*/

	LCDIF->CTRL = 0;
	LCDIF->CTRL |= (1 << 5);	/* 设置LCDIF为主机模式 */
	LCDIF->CTRL |= (3 << 8);	/* 设置24位数据 */
	LCDIF->CTRL |= (3 << 10);	/* 设置总线为24位 */
	LCDIF->CTRL |= (1 << 17);	/* 设置为DOTCLK模式 */
	LCDIF->CTRL |= (1 << 19);	/* DOTCLK模式下 必须为一*/

	LCDIF->CTRL1 = 0;
	LCDIF->CTRL1 |= (7 << 16);

	LCDIF->TRANSFER_COUNT = 0;	/* 设置行数 */
	LCDIF->TRANSFER_COUNT |= (tftlcd_dev.height << 16);	/* 设置行数 */
	LCDIF->TRANSFER_COUNT |= (tftlcd_dev.width << 0);	/* 设置一行的像素点个数 */

	LCDIF->VDCTRL0 = 0;
	LCDIF->VDCTRL0 |= (tftlcd_dev.vspw << 0);
	LCDIF->VDCTRL0 |= (1 << 20);
	LCDIF->VDCTRL0 |= (1 << 21);
	LCDIF->VDCTRL0 |= (1 << 24);	/* 设置ENABLE有效极性为高电平 */
	LCDIF->VDCTRL0 |= (0 << 25);	/* 设置DOTCLK_POL有效极性为低 */
	LCDIF->VDCTRL0 |= (0 << 26);	/* 设置HSYNC_POL有效极性为低 */
	LCDIF->VDCTRL0 |= (0 << 27);	/* 设置VSYNC_POL有效极性为低 */
	LCDIF->VDCTRL0 |= (1 << 28);	/* 设置使用ENABLE信号 */
	LCDIF->VDCTRL0 |= (0 << 29);	/* 设置VSYNC为输出 */
	/* 设置两个VSYNC信号之间的长度 */
	LCDIF->VDCTRL1 = tftlcd_dev.vspw + tftlcd_dev.vbpd + tftlcd_dev.height + tftlcd_dev.vfpd ;
	/* 设置两个HSYNC信号之间的长度 */
	LCDIF->VDCTRL2 = tftlcd_dev.hspw +tftlcd_dev.hbpd + tftlcd_dev.width +tftlcd_dev.hfpd;
	LCDIF->VDCTRL2 |= (tftlcd_dev.hspw << 18);


	LCDIF->VDCTRL3 = (tftlcd_dev.vspw + tftlcd_dev.vbpd) << 0;
	LCDIF->VDCTRL3 |= (tftlcd_dev.hspw + tftlcd_dev.hbpd) << 16;

	LCDIF->VDCTRL4 = tftlcd_dev.width;
	LCDIF->VDCTRL4 |= (1 << 18);

	LCDIF->CUR_BUF = (uint32_t)tftlcd_dev.framebuffer;
	LCDIF->NEXT_BUF = (uint32_t)tftlcd_dev.framebuffer;

	lcd_enable();
	lcd_clear(LCD_YELLOW);
}


/* lcd打点函数 */
inline void lcd_draw_point(uint16_t x, uint16_t y, uint32_t color)
{
	/* 计算出地址的偏移地址 */
	*(uint32_t*)((uint32_t)tftlcd_dev.framebuffer + (y * tftlcd_dev.width + x) * tftlcd_dev.pixsize) = color;

} 
/* lcd读点函数 */
inline uint32_t lcd_read_point(uint16_t x, uint16_t y)
{
	return *(uint32_t*)((uint32_t)tftlcd_dev.framebuffer + (y * tftlcd_dev.width + x) * tftlcd_dev.pixsize);
}

void lcd_clear(uint32_t color)
{	
	uint32_t i =0;
	/* 得到起始地址 */
	uint32_t* startaddr = (uint32_t*)tftlcd_dev.framebuffer;
	/* 得出写入个数 */
	uint32_t num = (uint32_t)tftlcd_dev.width * (uint32_t)tftlcd_dev.height;

	for(i=0; i<num; i++)
	{
		startaddr[i] = color;
	}
}