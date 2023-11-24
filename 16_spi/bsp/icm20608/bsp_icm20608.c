#include "bsp_icm20608.h"
#include "stdio.h"

struct icm20608_dev_struc icm20608_dev;


uint8_t icm20608_init(void)
{
    /* SPI */
    IOMUXC_SetPinMux(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0);
	IOMUXC_SetPinMux(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0);
    IOMUXC_SetPinMux(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0);

	IOMUXC_SetPinConfig(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0x10B0);
	IOMUXC_SetPinConfig(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0x10B0);
    IOMUXC_SetPinConfig(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0x10B0);
    /* 片选 */
    IOMUXC_SetPinMux(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0x10B0);

    gpio_pin_config_t  gpio_pin_config;
    gpio_pin_config.direction = kGPIO_DigitalOutput;
    gpio_pin_config.PinLevel = kGPIO_PinLevelLow;
    my_gpio_init(GPIO1, 20, &gpio_pin_config);

    spi_init(ECSPI3);

	icm20608_write_reg(ICM20_PWR_MGMT_1, 0x80);		/* 复位，复位后为0x40,睡眠模式 */
	my_delay_ms(50);
	icm20608_write_reg(ICM20_PWR_MGMT_1, 0x01);		/* 关闭睡眠，自动选择时钟 */
	my_delay_ms(50);

    if((icm20608_read_reg(ICM20_WHO_AM_I) != ICM20608G_ID) && (icm20608_read_reg(ICM20_WHO_AM_I) != ICM20608D_ID))
        return 1;

	icm20608_write_reg(ICM20_SMPLRT_DIV, 0x00); 	/* 输出速率是内部采样率 */
	icm20608_write_reg(ICM20_GYRO_CONFIG, 0x18); 	/* 陀螺仪±2000dps量程 */
	icm20608_write_reg(ICM20_ACCEL_CONFIG, 0x18); 	/* 加速度计±16G量程 */
	icm20608_write_reg(ICM20_CONFIG, 0x04); 		/* 陀螺仪低通滤波BW=20Hz */
	icm20608_write_reg(ICM20_ACCEL_CONFIG2, 0x04); 	/* 加速度计低通滤波BW=21.2Hz */
	icm20608_write_reg(ICM20_PWR_MGMT_2, 0x00); 	/* 打开加速度计和陀螺仪所有轴 =*/
	icm20608_write_reg(ICM20_LP_MODE_CFG, 0x00); 	/* 关闭低功耗 */
	icm20608_write_reg(ICM20_FIFO_EN, 0x00);		/* 关闭FIFO	*/

    printf("ICM20_GYRO_CONFIG  0x%02X\r\n", icm20608_read_reg(ICM20_GYRO_CONFIG));
    return 0;
}

void icm20608_write_reg(uint8_t reg, uint8_t data)
{
    ICM20608_CSN(0);

    reg &= ~0x80;

    spi_writeread_byte(ECSPI3, reg);
    spi_writeread_byte(ECSPI3, data);

    ICM20608_CSN(1);
}

uint8_t icm20608_read_reg(uint8_t reg)
{
    uint8_t rec = 0;
    ICM20608_CSN(0);

    reg |= 0x80;
    spi_writeread_byte(ECSPI3, reg);
    rec = spi_writeread_byte(ECSPI3, 0xff);

    ICM20608_CSN(1);

    return rec;
}


void icm20608_read_regs(uint8_t start_reg, uint8_t* Rxbuf, uint8_t len)
{
    uint8_t i = 0;
    ICM20608_CSN(0);

    start_reg |= 0x80;
    spi_writeread_byte(ECSPI3, start_reg);
    for(i=0; i<len; i++)
    {
        Rxbuf[i] = spi_writeread_byte(ECSPI3, 0xff);
    }
    ICM20608_CSN(1);
}

void icm20608_get_data(void)
{
    uint8_t data[14];
    icm20608_read_regs(ICM20_ACCEL_XOUT_H, data, 14);

    icm20608_dev.accel_x_adc = (signed short)((data[0] << 8) | data[1]); 
	icm20608_dev.accel_y_adc = (signed short)((data[2] << 8) | data[3]); 
	icm20608_dev.accel_z_adc = (signed short)((data[4] << 8) | data[5]); 
	icm20608_dev.temp_adc    = (signed short)((data[6] << 8) | data[7]); 
	icm20608_dev.gyro_x_adc  = (signed short)((data[8] << 8) | data[9]); 
	icm20608_dev.gyro_y_adc  = (signed short)((data[10] << 8) | data[11]);
	icm20608_dev.gyro_z_adc  = (signed short)((data[12] << 8) | data[13]);

    #if 1
    printf("icm20608_dev.accel_x_adc = %d\r\n",icm20608_dev.accel_x_adc);
    printf("icm20608_dev.accel_y_adc = %d\r\n",icm20608_dev.accel_y_adc);
    printf("icm20608_dev.accel_z_adc = %d\r\n",icm20608_dev.accel_z_adc);
    printf("icm20608_dev.temp_adc = %d\r\n",icm20608_dev.temp_adc);
    printf("icm20608_dev.gyro_x_adc = %d\r\n",icm20608_dev.gyro_x_adc);
    printf("icm20608_dev.gyro_y_adc = %d\r\n",icm20608_dev.gyro_y_adc);
    printf("icm20608_dev.gyro_z_adc = %d\r\n",icm20608_dev.gyro_z_adc);
    #endif
};


