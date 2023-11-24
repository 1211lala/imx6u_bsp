.global _start              /* 全局标号 */


@ .global _bss_start
@ _bss_start:
@     .word __bss_start

@ .global _bss_end
@ _bss_end:
@     .word __bss_end
    
@     /* 清除程序中定义了但是没有使用的值 */
@     ldr r0, _bss_start
@     ldr r1, _bss_end
@     mov r2, #0
@ bss_loop:
@     stmia r0!, {r2}     /* 向r0写值值为r2保存的内容 */
@     cmp r0, r1          /* 比较r0和r1的值 */
@     ble bss_loop        /* r0<=r1继续循环 */



_start:
    
    ldr pc, =Reset_Handler      /* 复位中断服务函数 */
    ldr pc, =Undefined_Handler  /* 未定义指令中断服务函数 */
    ldr pc, =SVC_Handler        /* SVC中断服务函数 */
    ldr pc, =PreAbort_Handler   /* 预取终止中断服务函数 */
    ldr pc, =DataAbort_Handler  /* 数据终止中断服务函数 */
    ldr pc, =NotUsed_Handler    /* 未使用中断服务函数 */
    ldr pc, =IRQ_Handler        /* 中断服务函数 */
    ldr pc, =FIQ_Handler        /* 快速中断服务函数 */

Reset_Handler:
    cpsid i                     /* 关闭IRQ中断 */
    /* 关闭I,D Cache和MMU SCTLR寄存器 读 - 改 -写*/
    MRC p15, 0, r0, c1, c0, 0   /* 读取 SCTLR到R0中 */
    bic r0, r0,#(1 << 12)   /* 关闭ICache */
    bic r0, r0,#(1 << 11)   /* 关闭分支预测 */
    bic r0, r0,#(1 << 2)   /* 关闭DCache */
    bic r0, r0,#(1 << 1)   /* 关闭对齐 */
    bic r0, r0,#(1 << 0)   /* 关闭MMU */
    MCR p15, 0, r0, c1, c0, 0 

    /* 设置中断向量偏移 --CP15的c12寄存器 实际测试写在这里程序中断时卡死 */
    @ ldr r0, = 0x87800000
    @ dsb
    @ isb
    @ MCR p15, 0, r0, c1, c0, 0
    @ dsb
    @ isb
    /* 设置IMX进入IRQ模式 */
    mrs r0, cpsr        /* 读取CPSR寄存器的值报保存在r0中 */
    bic r0, r0,#0x1f    /* 清空r0的低5位 */
    orr r0, r0,#0x12    /* 设置r0的低五位为0x12 使用IRQ模式 */
    msr cpsr, r0        /* 将r0写入cpsr */
    ldr sp, = 0x80600000

    /* 设置IMX进入SYS模式 */
    mrs r0, cpsr        /* 读取CPSR寄存器的值报保存在r0中 */
    bic r0, r0,#0x1f    /* 清空r0的低5位 */
    orr r0, r0,#0x1f    /* 设置r0的低五位为0x1f 使用sys模式 */
    msr cpsr, r0        /* 将r0写入cpsr */
    ldr sp, = 0x80400000

    /* 设置IMX进入SVC模式 */
    mrs r0, cpsr        /* 读取CPSR寄存器的值报保存在r0中 */
    bic r0, r0,#0x1f    /* 清空r0的低5位 */
    orr r0, r0,#0x13     /* 设置r0的低五位为0x13 使用SVC模式 */
    msr cpsr, r0        /* 将r0写入cpsr */
    ldr sp, = 0x80200000
    /* 外部的DDR的地址为0x80000000 同时设置 0x200000的堆空间自己设置的 使用设置sp映射到 0x80200000 */
    cpsie i                     /* 打开IRQ中断 */
    b main      /* 设置跳转到main不返回 */


Undefined_Handler:
    ldr r0, = Undefined_Handler
    bx r0

SVC_Handler:
    ldr r0, = SVC_Handler
    bx r0


PreAbort_Handler:
    ldr r0, = PreAbort_Handler
    bx r0

DataAbort_Handler:
    ldr r0, = DataAbort_Handler
    bx r0

NotUsed_Handler:
    ldr r0, = NotUsed_Handler
    bx r0

IRQ_Handler:
   	push {lr}					/* 保存lr地址 */
	push {r0-r3, r12}			/* 保存r0-r3，r12寄存器 */

	mrs r0, spsr				/* 读取spsr寄存器 */
	push {r0}					/* 保存spsr寄存器 */

	mrc p15, 4, r1, c15, c0, 0 /* 从CP15的C0寄存器内的值到R1寄存器中
								* 参考文档ARM Cortex-A(armV7)编程手册V4.0.pdf P49
								* Cortex-A7 Technical ReferenceManua.pdf P68 P138
								*/							
	add r1, r1, #0X2000			/* GIC基地址加0X2000，也就是GIC的CPU接口端基地址 */
	ldr r0, [r1, #0XC]			/* GIC的CPU接口端基地址加0X0C就是GICC_IAR寄存器，
								 * GICC_IAR寄存器保存这当前发生中断的中断号，我们要根据
								 * 这个中断号来绝对调用哪个中断服务函数
								 */
	push {r0, r1}				/* 保存r0,r1 */
	
	cps #0x13					/* 进入SVC模式，允许其他中断再次进去 */
	
	push {lr}					/* 保存SVC模式的lr寄存器 */
	ldr r2, =system_irqhandler	/* 加载C语言中断处理函数到r2寄存器中*/
	blx r2						/* 运行C语言中断处理函数，带有一个参数，保存在R0寄存器中 */

	pop {lr}					/* 执行完C语言中断服务函数，lr出栈 */
	cps #0x12					/* 进入IRQ模式 */
	pop {r0, r1}				
	str r0, [r1, #0X10]			/* 中断执行完成，写EOIR */

	pop {r0}						
	msr spsr_cxsf, r0			/* 恢复spsr */

	pop {r0-r3, r12}			/* r0-r3,r12出栈 */
	pop {lr}					/* lr出栈 */
	subs pc, lr, #4				/* 将lr-4赋给pc */

FIQ_Handler:
    ldr r0, = FIQ_Handler
    bx r0




