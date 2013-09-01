/*****************************************************************************
**Copyright (C), 2009-2010, 
**FileName:    ** iic.c
**Author:      ** zozo
**Description: ** 
**History:     ** 2013/8/1   V0.1  创建
******************************************************************************/

//****************************************************************************
// 头文件
//****************************************************************************/
#include "fm3_common.h"
#include "Pdl_header.h"
#include "resource.h"
#include "iic.h"


/*******************************************************************************
* Function Name  : I2cGpioInit
* Description   : 
* Input           : none
* Output         : none
*******************************************************************************/
void I2cGpioInit(void)
{
	IO_DisableFunc(IIC_IO_PORT,IIC_SDA_IO_PIN|IIC_SCL_IO_PIN);
	IO_ConfigGPIOPin(IIC_IO_PORT,IIC_SDA_IO_PIN|IIC_SCL_IO_PIN,
									 IO_DIR_INPUT,IO_PULLUP_ON);
	I2C_SDA_OUT_LOW;
	I2C_SCL_OUT_LOW;


}

/*************************************************************************
** Function Name  : 
** Description    :   
** Input       		: 
** Output     		: 
*************************************************************************/
void i2ct(void)
{
    QDEL;//HDEL;modified zozo
		I2C_SCL_HI; 
		QDEL;//HDEL;modified zozo
		I2C_SCL_LO;
}

/*************************************************************************
** Function Name  : 
** Description    :   
** Input       		: 
** Output     		: 
*************************************************************************/
void i2cstart(void)
{
//		I2C_SCL_LO; HDEL;  modified zozo
		
		I2C_SDA_HI; HDEL;
		
		I2C_SCL_HI; HDEL;//QDEL;modified zozo
		
		I2C_SDA_LO; HDEL;//QDEL;modified zozo
		
		I2C_SCL_LO; HDEL;
}
/*************************************************************************
** Function Name  : 
** Description    :   
** Input       		: 
** Output     		: 
*************************************************************************/
void i2cstop(void)
{
		HDEL;
		I2C_SDA_LO; HDEL; 
		
		I2C_SCL_HI; HDEL;//QDEL;modified zozo
		
		I2C_SDA_HI; HDEL;
}
/*************************************************************************
** Function Name  : 
** Description    :   
** Input       		: 
** Output     		: 
*************************************************************************/
uint8_t i2cWaitBusy(void)
{

return 0;
}

/*************************************************************************
** Function Name  : i2cPutbyte
** Description    :   
** Input       		: 
** Output     		: TRUE: ack FALSE:nack
*************************************************************************/
uint8_t i2cPutbyte(uint8_t b)
{

	int i;

	//I2C_SDA_OUT;
	for (i=8;i>0;i--)
	{
		if ( b & 0x80 )
				I2C_SDA_HI;
		else
				I2C_SDA_LO; 							// address bit
		b <<= 1;
		I2C_SCL_TOGGLE; 							// clock HI, delay, then LO
	}
	I2C_SDA_DIR_INPUT; 							// leave SDL HI,change direction to input on SDA line

	ADEL; 													//clock-stretching
	
	I2C_SCL_DIR_INPUT; 							// clock back up
	HDEL;
	
 	b= I2C_SDA_IN;

	I2C_SCL_LO; 										// not really ??

	if(b == 0)											// return ACK value
			return (TRUE);
	else
			return (FALSE);

}
/*************************************************************************
** Function Name  : 
** Description    :   
** Input       		: 
** Output     		: TRUE: ack FALSE:nack
*************************************************************************/
uint8_t i2cGetbyte(uint8_t last)
{

		uint8_t i;
		uint8_t c,b = 0;


		I2C_SDA_DIR_INPUT; 										// make sure pullups are ativated,change direction to input on SDA line

		for(i=8;i>0;i--)
		{
				QDEL;//HDEL;modified zozo
				I2C_SCL_HI; 								// clock HI
        c = I2C_SDA_IN;
				b <<= 1;
				if(c) b |= 1;
				QDEL;//HDEL;modified zozo
				I2C_SCL_LO; 								// clock LO
		}

		QDEL;

		if (last)
				I2C_SDA_HI; 								// set NAK
		else
				I2C_SDA_LO; 								// set ACK
				
		I2C_SCL_TOGGLE; 								// clock pulse

		I2C_SDA_HI; 										// leave with SDL HI
		return b; 											// return received byte



}



/*************************************************************************
** Function Name  : 
** Description    :   
** Input       		: 
** Output     		: 
*************************************************************************/
uint8_t i2cWriteStart(void)
{


		IIC_SEND_START;																			// do start transition
		if(i2cPutbyte((ADDR_NEONODE<<1)|WRITE)) 						// send DEVICE address
		  return TRUE;
		else
			return FALSE;


}

/*************************************************************************
** Function Name  : 
** Description    :   
** Input       		: 
** Output     		: 
*************************************************************************/
uint8_t i2cReadStart(void)
{


		IIC_SEND_START; 																// do start transition
		if(i2cPutbyte((ADDR_NEONODE<<1)|READ))					// send DEVICE address
			return TRUE;
		else
			return FALSE;

}




