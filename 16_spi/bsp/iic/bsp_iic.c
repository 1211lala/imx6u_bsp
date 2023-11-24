#include "bsp_iic.h"


/* 使能IIC */
void i2c_enable(I2C_Type* base)
{
    base->I2CR |= (1 << 7);    
}

/* 失能IIC */
void i2c_disable(I2C_Type* base)
{
    base->I2CR &= ~(1 << 7);    
}

/* 初始化IIC */
void i2c_init(I2C_Type* base)
{
    i2c_disable(base);           /* 先关闭IIC */
    base->IFDR = 0x15;          /* 设置ipg_clk的分频给IIC提供时钟 0x15对应 640分频 66M/640 约等于 100K*/
    i2c_enable(base);
}


/* 产生起始信号和从机地址发送 */
uint8_t i2c_master_start(I2C_Type* base, uint8_t address, enum i2c_directilon direction)
{
    if(base->I2SR & (1 << 5))   return I2C_STATUS_BUSY;   /* IIC忙 直接返回 */

    base->I2CR |= (1 <<  5);    /* 设置为主机模式 */
    base->I2CR |= (1 <<  4);    /* 设置为发送模式 */

    /* IIC在写数据时就自动的产生了一个start信号 */
    base->I2DR = (address << 1)| direction;
    return I2C_STATUS_OK;
}

/* 设置iic产生stop信号 */
uint8_t i2c_master_stop(I2C_Type* base)
{
    uint16_t timeout = 0xffff;
    
    base->I2CR &= ~(1 << 5);
    base->I2CR &= ~(1 << 4);
    base->I2CR &= ~(1 << 3);
    while((base->I2SR & (1 << 5)))
    {
        timeout --;
        if(timeout == 0) return I2C_STATUS_TIMEOUT;
    }
    return I2C_STATUS_OK;
}

/* 设置重新产生start信号 */
uint8_t i2c_master_repeated_start(I2C_Type* base, uint8_t address, enum i2c_directilon direction)
{
    if(base->I2SR & (1 << 5) && (((base->I2CR) & (1 << 5)) == 0))	/* 从机忙并且是在从机模式下 */	

    base->I2CR |= (1 << 4);    /* 对bit4 置一 进入发送模式 */
    base->I2CR |= (1 << 2);    /* 对bit2 置一 生成 repeated_start信号 */

    base->I2DR = (address << 1)| direction;

    return I2C_STATUS_OK;
}

/* 检查iic数据传送过程中的错误 */
uint8_t i2c_check_and_clear_error(I2C_Type* base, uint32_t status)
{
    /* 检查是否发生仲裁丢失错误 */
	if(status & (1<<4))
	{
		base->I2SR &= ~(1<<4);		/* 清除仲裁丢失错误位 			*/

		base->I2CR &= ~(1 << 7);	/* 先关闭I2C 				*/
		base->I2CR |= (1 << 7);		/* 重新打开I2C 				*/
		return I2C_STATUS_ARBITRATIONLOST;
	} 
	else if(status & (1 << 0))     	/* 没有接收到从机的应答信号 */
	{
		return I2C_STATUS_NAK;		/* 返回NAK(No acknowledge) */
	}
	return I2C_STATUS_OK;
}



void i2c_master_write(I2C_Type* base, uint8_t* Txbuf, uint16_t size)
{
    /* 等待上一个发送完成 */
    while(((base->I2SR) & (1 << 7)) == 0);
    /* 清除中断标志位*/
    base->I2SR &= ~(1 << 1);

    base->I2CR |= (1 << 4); /* 设置为发送模式 */

    while(size--)
    {
        base->I2DR = *Txbuf++;      /* 发送数据 */
        while(((base->I2SR) & (1 << 1)) == 0);  /* 等待传输完成 不是用的 bit7位 */
        base->I2SR &= ~(1 << 1);    /* 清除标志位 */
        if(i2c_check_and_clear_error(base, base->I2SR)) break;
    }
    base->I2SR &= ~(1 << 1);
    i2c_master_stop(base);
}



void i2c_master_read(I2C_Type* base, uint8_t* Rxbuf, uint16_t size)
{
	volatile uint8_t dummy = 0;

	dummy++; 	/* 防止编译报错 */
	
	/* 等待传输完成 */
	while(!(base->I2SR & (1 << 7))); 
	
	base->I2SR &= ~(1 << 1); 				/* 清除中断挂起位 */
	base->I2CR &= ~((1 << 4) | (1 << 3));	/* 接收数据 */
	
	/* 如果只接收一个字节数据的话发送NACK信号 */
	if(size == 1)
        base->I2CR |= (1 << 3);

	dummy = base->I2DR; /* 假读 */
	
	while(size--)
	{
		while(!(base->I2SR & (1 << 1))); 	/* 等待传输完成 */	
		base->I2SR &= ~(1 << 1);			/* 清除标志位 */

	 	if(size == 0)
        {
        	i2c_master_stop(base); 			/* 发送停止信号 */
        }

        if(size == 1)
        {
            base->I2CR |= (1 << 3);
        }
		*Rxbuf++ = base->I2DR;
	}
}


/*
 * @description	: I2C数据传输，包括读和写
 * @param - base: 要使用的IIC
 * @param - xfer: 传输结构体
 * @return 		: 传输结果,0 成功，其他值 失败;
 */
unsigned char i2c_master_transfer(I2C_Type *base, struct i2c_transfer *xfer)
{
	unsigned char ret = 0;
	 enum i2c_directilon direction = xfer->direction;	

	base->I2SR &= ~((1 << 1) | (1 << 4));			/* 清除标志位 */

	/* 等待传输完成 */
	while(!((base->I2SR >> 7) & 0X1)){}; 

	/* 如果是读的话，要先发送寄存器地址，所以要先将方向改为写 */
    if ((xfer->subaddressSize > 0) && (xfer->direction == kI2C_Read))
    {
        direction = kI2C_Write;
    }

	ret = i2c_master_start(base, xfer->slaveAddress, direction); /* 发送开始信号 */
    if(ret)
    {	
		return ret;
	}

	while(!(base->I2SR & (1 << 1))){};			/* 等待传输完成 */

    ret = i2c_check_and_clear_error(base, base->I2SR);	/* 检查是否出现传输错误 */
    if(ret)
    {
      	i2c_master_stop(base); 						/* 发送出错，发送停止信号 */
        return ret;
    }
	
    /* 发送寄存器地址 */
    if(xfer->subaddressSize)
    {
        do
        {
			base->I2SR &= ~(1 << 1);			/* 清除标志位 */
            xfer->subaddressSize--;				/* 地址长度减一 */
			
            base->I2DR =  ((xfer->subaddress) >> (8 * xfer->subaddressSize)); //向I2DR寄存器写入子地址
  
			while(!(base->I2SR & (1 << 1)));  	/* 等待传输完成 */

            /* 检查是否有错误发生 */
            ret = i2c_check_and_clear_error(base, base->I2SR);
            if(ret)
            {
             	i2c_master_stop(base); 				/* 发送停止信号 */
             	return ret;
            }  
        } while ((xfer->subaddressSize > 0) && (ret == I2C_STATUS_OK));

        if(xfer->direction == kI2C_Read) 		/* 读取数据 */
        {
            base->I2SR &= ~(1 << 1);			/* 清除中断挂起位 */
            i2c_master_repeated_start(base, xfer->slaveAddress, kI2C_Read); /* 发送重复开始信号和从机地址 */
    		while(!(base->I2SR & (1 << 1))){};/* 等待传输完成 */

            /* 检查是否有错误发生 */
			ret = i2c_check_and_clear_error(base, base->I2SR);
            if(ret)
            {
             	ret = I2C_ADDRNAK;
                i2c_master_stop(base); 		/* 发送停止信号 */
                return ret;  
            }
           	          
        }
    }	


    /* 发送数据 */
    if ((xfer->direction == kI2C_Write) && (xfer->dataSize > 0))
    {
    	i2c_master_write(base, xfer->data, xfer->dataSize);
	}

    /* 读取数据 */
    if ((xfer->direction == kI2C_Read) && (xfer->dataSize > 0))
    {
       	i2c_master_read(base, xfer->data, xfer->dataSize);
	}
	return 0;	
}