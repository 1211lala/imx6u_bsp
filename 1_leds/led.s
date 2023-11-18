.global _start              /* 全局标号 */

_start:

    /* 功能:使能所有外设时钟 */
    ldr r0, = 0x020c4068     /* 把时钟寄存器GCGR0的地址写入r0 */      
    ldr r1, = 0xffffffff     /* 把要写入时钟寄存器的值写入r1 */        
    str r1, [r0]              /* 把r1的值写入r0代表的地址中 */

    ldr r0, = 0x020c406C     /* 把时钟寄存器GCGR1的地址写入r0 */      
    ldr r1, = 0xffffffff     /* 把要写入时钟寄存器的值写入r1 */        
    str r1, [r0]              /* 把r1的值写入r0代表的地址中 */

    ldr r0, = 0x020c4070     /* 把时钟寄存器GCGR2的地址写入r0 */      
    ldr r1, = 0xffffffff     /* 把要写入时钟寄存器的值写入r1 */        
    str r1, [r0]              /* 把r1的值写入r0代表的地址中 */

    ldr r0, = 0x020c4074     /* 把时钟寄存器GCGR3的地址写入r0 */      
    ldr r1, = 0xffffffff     /* 把要写入时钟寄存器的值写入r1 */        
    str r1, [r0]              /* 把r1的值写入r0代表的地址中 */

    ldr r0, = 0x020c4078     /* 把时钟寄存器GCGR4的地址写入r0 */      
    ldr r1, = 0xffffffff     /* 把要写入时钟寄存器的值写入r1 */        
    str r1, [r0]              /* 把r1的值写入r0代表的地址中 */

    ldr r0, = 0x020c407c     /* 把时钟寄存器GCGR5的地址写入r0 */      
    ldr r1, = 0xffffffff     /* 把要写入时钟寄存器的值写入r1 */        
    str r1, [r0]              /* 把r1的值写入r0代表的地址中 */

    ldr r0, = 0x020c4080     /* 把时钟寄存器GCGR6的地址写入r0 */      
    ldr r1, = 0xffffffff     /* 把要写入时钟寄存器的值写入r1 */        
    str r1, [r0]              /* 把r1的值写入r0代表的地址中 */

    /* 配置PIN的复用模式为GPIO */
    /* 将地址为 0x020e0068 的寄存器 IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03 的 MUX_MODE 位设置成 0101  */
    ldr r0, = 0x020e0068
    ldr r1, = 0x5
    str r1, [r0]

    /* 配置PIN的GPIO电气属性 */
    /* 将地址为 0x020e02f4 的寄存器 IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03 配置成下列电气属性 */
    /*
        bit0:   设置压摆率      设置成0 低压摆率
        bit5:3  设置IO驱动能力  设置 110
        bit7:6  设置IO速度     设置 10 100Mhz
        bit11   设置开漏输出    设置 0 关闭开漏输出
        bit12   设置上拉       设置 1 开启上拉
        bit13   设置 0 keeper
        bit15:14设置电阻    设置 00 100k下拉
        bit16   设置 0        关闭phy                     
    
        0000 0000 0000 0000 0001 0000 1011 0000 == 0x000010b0
     */
    ldr r0, = 0x020e02f4
    ldr r1, = 0x000010b0
    str r1, [r0]   

    /* 配置GPIO的功能 */ 
    /* 配置GPIO1_GDIR(0x0209c004)寄存器的bit3位为1将 GPIO1_IO03 设置为输出 */
    /* 0000 0000 0000 0000 0000 0000 0000 1000  */
    ldr r0, = 0x0209c004
    ldr r1, = 0x08
    str r1, [r0]  

    /* 配置GPIO1_DR(0x0209c000)寄存器的bit3位为1将 GPIO1_IO03 设置为低电平 */
    ldr r0, = 0x0209c000
    ldr r1, = 0x00
    str r1, [r0]  

loop:
    b loop
    