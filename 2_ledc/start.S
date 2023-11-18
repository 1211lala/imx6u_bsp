.global _start              /* 全局标号 */

_start:
    /* 设置IMX进入SVC模式 */
    mrs r0, cpsr        /* 读取CPSR寄存器的值报保存在r0中 */
    bic r0, r0,#0x1f    /* 清空r0的低5位 */
    orr r0, r0,#0x13     /* 设置r0的低五位为0x13 使用SVC模式 */
    msr cpsr, r0        /* 将r0写入cpsr */

    /* 设置sp指针到0x80200000 */
    /* 外部的DDR的地址为0x80000000 同时设置 0x200000的堆空间自己设置的 使用设置sp映射到 0x80200000 */
    ldr sp, = 0x80200000
    b main      /* 设置跳转到main不返回 */
