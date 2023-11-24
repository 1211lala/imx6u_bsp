#ifndef _BSP_LCD_H_
#define _BSP_LCD_H_

#include "main.h"


#define ATK7016 0x7016      /* 7寸 1024*800*/

#define LCD_BLUE		  0x000000FF
#define LCD_GREEN		  0x0000FF00
#define LCD_RED 		  0x00FF0000
#define LCD_CYAN		  0x0000FFFF
#define LCD_MAGENTA 	  0x00FF00FF
#define LCD_YELLOW		  0x00FFFF00
#define LCD_LIGHTBLUE	  0x008080FF
#define LCD_LIGHTGREEN	  0x0080FF80
#define LCD_LIGHTRED	  0x00FF8080
#define LCD_LIGHTCYAN	  0x0080FFFF
#define LCD_LIGHTMAGENTA  0x00FF80FF
#define LCD_LIGHTYELLOW   0x00FFFF80
#define LCD_DARKBLUE	  0x00000080
#define LCD_DARKGREEN	  0x00008000
#define LCD_DARKRED 	  0x00800000
#define LCD_DARKCYAN	  0x00008080
#define LCD_DARKMAGENTA   0x00800080
#define LCD_DARKYELLOW	  0x00808000
#define LCD_WHITE		  0x00FFFFFF
#define LCD_LIGHTGRAY	  0x00D3D3D3
#define LCD_GRAY		  0x00808080
#define LCD_DARKGRAY	  0x00404040
#define LCD_BLACK		  0x00000000
#define LCD_BROWN		  0x00A52A2A
#define LCD_ORANGE		  0x00FFA500
#define LCD_TRANSPARENT   0x00000000


extern struct tftlcd_typedef tftlcd_dev;
/* 设置显存的首地址 */
#define LCD_FRAMEBUF_ADDR   0x89000000
/* lcd屏幕信息结构体 */
struct tftlcd_typedef
{
    uint16_t height;    /* 屏幕高度 */
    uint16_t width;     /* 屏幕宽度 */
    uint8_t pixsize;    /* 每个像素占用的字节数 */
    uint16_t vspw;      /* 时间参数 */
    uint16_t vbpd;      /* 时间参数 */
    uint16_t vfpd;      /* 时间参数 */
    uint16_t hspw;      /* 时间参数 */
    uint16_t hbpd;      /* 时间参数 */
    uint16_t hfpd;      /* 时间参数 */
    uint32_t framebuffer;   /* 屏幕显存起始地址 */
    uint32_t forecolor; /* 画笔颜色 */
    uint32_t backcolor; /* 背景颜色 */
};


uint16_t lcd_read_id(void);
void lcd_gpio_init(void);
void lcd_reset(void);
void lcd_no_reset(void);
void lcd_enable(void);
void lcd_clk_init(uint32_t DIV_SELECT, uint32_t LCDIF1_PRED, uint32_t LCDIF1_PODF);

void my_lcd_init(void);
inline void lcd_draw_point(uint16_t x, uint16_t y, uint32_t color);
inline uint32_t lcd_read_point(uint16_t x, uint16_t y);
void lcd_clear(uint32_t color);
#endif // ! _BSP_LCD_H_