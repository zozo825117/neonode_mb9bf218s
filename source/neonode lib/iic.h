/*****************************************************************************
**Copyright (C), 2009-2010, 
**FileName:    ** iic.h
**Author:      ** zozo
**Description: ** 
**History:     ** 2013/8/1   V0.1  创建
******************************************************************************/
#ifndef __IIC_MSP430_H__
#define __IIC_MSP430_H__



//****************************************************************************
// 宏定义
//****************************************************************************/


/*
 device address
*/
#define	ADDR_NEONODE   0x50
#define READ		       0x01	// I2C READ bit
#define WRITE		       0x00	// I2C WRITE bit


/*
	iic define
*/
void i2ct(void);
void i2cstart(void);
void i2cstop(void);

// i2c quarter-bit delay
#define QDEL    delay_us(2)
// i2c half-bit delay
#define HDEL    delay_us(4)
// i2c wait ack delay
#define ADEL    delay_us(4) //clock-stretching


#define IIC_SEND_START          i2cstart()
#define IIC_SEND_STOP           i2cstop()  

#define I2C_SDA_IN              IO_ReadGPIOPin(IIC_IO_PORT , IIC_SDA_IO_PIN)
#define I2C_SDA_OUT_LOW         IO_WriteGPIOPin(IIC_IO_PORT,IIC_SDA_IO_PIN,IO_BIT_CLR)
#define I2C_SCL_OUT_LOW         IO_WriteGPIOPin(IIC_IO_PORT,IIC_SCL_IO_PIN,IO_BIT_CLR)

#define I2C_SDA_DIR_INPUT       IO_ConfigGPIOPin(IIC_IO_PORT,IIC_SDA_IO_PIN,\
																								 IO_DIR_INPUT,IO_PULLUP_ON)
#define I2C_SCL_DIR_INPUT       IO_ConfigGPIOPin(IIC_IO_PORT,IIC_SCL_IO_PIN,\
																								 IO_DIR_INPUT,IO_PULLUP_ON)

#define I2C_SDA_LO              IO_ConfigGPIOPin(IIC_IO_PORT,IIC_SDA_IO_PIN,\
																								 IO_DIR_OUTPUT,IO_PULLUP_ON)
#define I2C_SCL_LO              IO_ConfigGPIOPin(IIC_IO_PORT,IIC_SCL_IO_PIN,\
																								 IO_DIR_OUTPUT,IO_PULLUP_ON)
#define I2C_SDA_HI              IO_ConfigGPIOPin(IIC_IO_PORT,IIC_SDA_IO_PIN,\
																								 IO_DIR_INPUT,IO_PULLUP_ON)
#define I2C_SCL_HI              IO_ConfigGPIOPin(IIC_IO_PORT,IIC_SCL_IO_PIN,\
																								 IO_DIR_INPUT,IO_PULLUP_ON)
#define I2C_SCL_TOGGLE          i2ct()


//****************************************************************************
// 函数声明
//****************************************************************************/
void I2cGpioInit(void);
uint8_t i2cWaitBusy(void);
uint8_t i2cPutbyte(uint8_t b);
uint8_t i2cGetbyte(uint8_t last);
uint8_t i2cWriteStart(void);
uint8_t i2cReadStart(void);



#endif//__IIC_MSP430_H__

