/*!
 ******************************************************************************
 **
 ** \file resource.h
 **
 ** \brief Configure corresponding peripheral used in this example
 **
 ** \author FSLA AE Team 
 **
 ** \version V0.10
 **
 ** \date 2013-01-19
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note As the peripheral pins to control the on-board devices may differ 
 **       among each evaluation board, the configuration channel and pin 
 **       assignment in the example will change accordingly. 
 **       Each example can run on some of evaluation boards as expected. If 
 **       user want to port the example on other boards, he needs to make 
 **       certain setting referring to current configuration.
 **
 ******************************************************************************
 */ 

#ifndef _RESOURCE_H_
#define _RESOURCE_H_
/*!
 ******************************************************************************
 ** \note Select the evaluaiton board by setting a definition at the project
 **       option or pdl_user.h.
 ******************************************************************************
 */ 
#if defined (LSD1EV_BEMB9BF218S0)
/*------------------ Neonode Control mapping ---------*/
/* IIC */
/*! \brief IIC IO port */ 
#define IIC_IO_PORT        				IO_PORT3
/*! \brief IIC_SDA_IO_PIN */
#define IIC_SDA_IO_PIN     				IO_PINx7
/*! \brief IIC IO SCL pin */ 
#define IIC_SCL_IO_PIN     				IO_PINx8
/*! \brief IIC_SDA_IO_PIN */
#define IIC_RDY_IO_PIN     				IO_PINx9
/*! \brief IIC reset port */ 
#define IIC_RST_PORT							IO_PORT3
/*! \brief IIC IO SCL pin */ 
#define IIC_RST_IO_PIN     				IO_PINxA		
/* UART */
/*! \brief IO MFS channel */
#define   IO_MFS_CH             (MFS_Ch0)
/*! \brief IO MFS port */
#define   IO_MFS_PORT           (IO_PORT2)
/*! \brief IO MFS SOT pin */
#define   IO_MFS_SOT_PIN        (IO_PINx2) 
/*! \brief IO MFS SIN pin */
#define   IO_MFS_SIN_PIN        (IO_PINx1)
/*! \brief IO MFS SOT pin location */
#define   IO_MFS_SOT_PIN_LOC    (IO_MFS_SOTx_SOTx_0)
/*! \brief IO MFS SIN pin location */
#define   IO_MFS_SIN_PIN_LOC    (IO_MFS_SINx_SINx_0)	
/*! \brief User BT channel */
#define USER_BT_CH         (BT_CH_0)
/*! \brief User timer channel */
#define USER_TIMER_MODE    (BT_32BIT_TIMER)

#elif defined (LSD1DM_TFT218N0)
/*------------------ Neonode Control mapping ---------*/
/* IIC */
/*! \brief IIC IO port */ 
#define IIC_IO_PORT        				IO_PORT6
/*! \brief IIC_SDA_IO_PIN */
#define IIC_SDA_IO_PIN     				IO_PINx1
/*! \brief IIC IO SCL pin */ 
#define IIC_SCL_IO_PIN     				IO_PINx2
/*! \brief IIC_SDA_IO_PIN */
#define IIC_RDY_IO_PIN     				IO_PINx0
/*! \brief IIC reset port */ 
#define IIC_RST_PORT							IO_PORTF
/*! \brief IIC IO SCL pin */ 
#define IIC_RST_IO_PIN     				IO_PINx5
/* UART */
/*! \brief IO MFS channel */
#define   IO_MFS_CH             (MFS_Ch0)
/*! \brief IO MFS port */
#define   IO_MFS_PORT           (IO_PORT2)
/*! \brief IO MFS SOT pin */
#define   IO_MFS_SOT_PIN        (IO_PINx2) 
/*! \brief IO MFS SIN pin */
#define   IO_MFS_SIN_PIN        (IO_PINx1)
/*! \brief IO MFS SOT pin location */
#define   IO_MFS_SOT_PIN_LOC    (IO_MFS_SOTx_SOTx_0)
/*! \brief IO MFS SIN pin location */
#define   IO_MFS_SIN_PIN_LOC    (IO_MFS_SINx_SINx_0)	
/*! \brief User BT channel */
#define USER_BT_CH         (BT_CH_0)
/*! \brief User timer channel */
#define USER_TIMER_MODE    (BT_32BIT_TIMER)
#else
    #error Select a certain evaluation board!	
#endif	


/*
  interrupt define for fm32
*/
#define disable_int()							__disable_interrupt 
#define enable_int()							__enable_interrupt 
/*
 mclk
*/		
#define MCLK											(__HCLK/1000000)
/*
  soft delay define for fm32
*/
#define delay_us(x)						  	DELAY_US(x*MCLK)
#define delay_ms(x)								DELAY_MS(x)

/*
  global function define
*/
void DELAY_US (uint32_t cycles);
void DELAY_MS (uint32_t cycles);

#define __nop()										__no_operation()



#endif /* _RESOURCE_H_ */

