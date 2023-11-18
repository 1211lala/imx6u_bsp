#ifndef  _BSP_IIC_H_
#define _BSP_IIC_H_

#include "main.h"

#define I2C_STATUS_OK                   (0)
#define I2C_STATUS_BUSY                 (1)
#define I2C_STATUS_IDLE                 (2)
#define I2C_STATUS_NAK                  (3)
#define I2C_STATUS_ARBITRATIONLOST      (4)
#define I2C_STATUS_TIMEOUT              (5)
#define I2C_ADDRNAK                     (6)


enum i2c_directilon
{
    i2c_write = 0,
    i2c_read
};


/*
 * 主机传输结构体
 */
struct i2c_transfer
{
    unsigned char slaveAddress;      	/* 7位从机地址 			*/
    enum i2c_directilon direction; 		/* 传输方向 			*/
    unsigned int subaddress;       		/* 寄存器地址			*/
    unsigned char subaddressSize;    	/* 寄存器地址长度 			*/
    unsigned char *volatile data;    	/* 数据缓冲区 			*/
    volatile unsigned int dataSize;  	/* 数据缓冲区长度 			*/
};



void iic_enable(I2C_Type* base);
void iic_disable(I2C_Type* base);
void iic_init(I2C_Type* base);
uint8_t iic_master_start(I2C_Type* base, uint8_t address, enum i2c_directilon i2c_direction);
uint8_t iic_master_stop(I2C_Type* base);
uint8_t iic_check_and_clear_error(I2C_Type* base, uint32_t status);
unsigned char iic_master_transfer(I2C_Type *base, struct i2c_transfer *xfer);
#endif // ! _BSP_IIC_H_