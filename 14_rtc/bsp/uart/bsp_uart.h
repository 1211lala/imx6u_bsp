#ifndef _BSP_UART_H_
#define _BSP_UART_H_


#include "main.h"


void my_uart_disable(UART_Type *base);
void my_uart_enable(UART_Type *base);
void my_uart_softreset(UART_Type *base);

void my_uart1_init(uint32_t baud);
void putc(uint8_t c);
void puts(uint8_t* str);
unsigned char getc(void);
void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz);
#endif // ! _BSP_UART_H_