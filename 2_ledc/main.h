#ifndef _MAIN_H_
#define _MAIN_H_


#define uint32_t    unsigned int

/* 定义系统时钟寄存器 */
#define CCM_CCGR0       *((volatile uint32_t*)0x020c4068)
#define CCM_CCGR1       *((volatile uint32_t*)0x020c406C)
#define CCM_CCGR2       *((volatile uint32_t*)0x020c4070)
#define CCM_CCGR3       *((volatile uint32_t*)0x020c4074)
#define CCM_CCGR4       *((volatile uint32_t*)0x020c4078)
#define CCM_CCGR5       *((volatile uint32_t*)0x020c407c)
#define CCM_CCGR6       *((volatile uint32_t*)0x020c4080)

/* 定义GPIO1_IO3的电气属性和复用寄存器 */
#define SW_MUX_CTL_PAD_GPIO1_IO03   *((uint32_t*)0x020e0068)
#define SW_PAD_CTL_PAD_GPIO1_IO03   *((uint32_t*)0x020e02f4)

/* 定义GPIO1_IO3 GPIO配置寄存器 */
#define GPIO1_DR         *((volatile uint32_t*)0x0209c000)
#define GPIO1_GDIR       *((volatile uint32_t*)0x0209c004)
#define GPIO1_PSR        *((volatile uint32_t*)0x0209c008)
#define GPIO1_ICR1       *((volatile uint32_t*)0x0209c00c)
#define GPIO1_ICR2       *((volatile uint32_t*)0x0209c010)
#define GPIO1_IMR        *((volatile uint32_t*)0x0209c014)
#define GPIO1_ISR        *((volatile uint32_t*)0x0209c018)

#endif