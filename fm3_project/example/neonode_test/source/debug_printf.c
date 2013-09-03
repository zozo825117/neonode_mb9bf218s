/*!
 ******************************************************************************
 **
 ** \file debug_printf.c
 **
 ** \brief mcu platform FM32
 **
 ** \author 
 **
 ** \version V0.10
 **
 ** \date 
 **
 ** \brief 
 **
 ** \attention 
 **
 ** (C) Copyright 
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "Pdl_header.h"
#include "resource.h"
#include <stdarg.h> 
#include <string.h>
/*---------------------------------------------------------------------------*/
/* Macro definition                                                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
MFS_UARTModeConfigT tUARTModeConfigT = 
{
    115200,
    UART_DATABITS_8,
    UART_STOPBITS_1,
    UART_PARITY_NONE,
    UART_BITORDER_LSB,
    UART_NRZ,
};
/*---------------------------------------------------------------------------*/
/* IO definition                                                             */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/* Local functions prototypes                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Global data                                                               */
/*---------------------------------------------------------------------------*/



/*!
 ******************************************************************************
 ** \brief  Initialize the I/O port for UART: select MFS ch2/SIN2_1 and SOT2_1
 **
 ** \param  none
 **
 ** \return none
 ******************************************************************************
 */
void InitUartPort(void)
{

	IO_DisableAnalogInput(IO_AN31);
	/* Enable SOT and SIN */
	IO_EnableFunc(IO_MFS_PORT, IO_MFS_SOT_PIN);
	IO_EnableFunc(IO_MFS_PORT, IO_MFS_SIN_PIN);
	IO_ConfigFuncMFSSOTxPin(IO_MFS_CH, IO_MFS_SOT_PIN_LOC);
	IO_ConfigFuncMFSSINxPin(IO_MFS_CH, IO_MFS_SIN_PIN_LOC);
}
/*!
 ******************************************************************************
 ** \brief Config the UART mode
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **         
 ** \param  Mode initialization data
 ** \param MFS_UARTModeConfigT
 ** \arg   Mode info
 **
 ** \retval None
 **
 ******************************************************************************
 */
void UARTConfigMode(uint8_t Ch, MFS_UARTModeConfigT *pModeConfig)
{
    /* Config the MOD as UART */
    MFS_UARTSetUARTMode(Ch);
    /* Enable Output */
    MFS_UARTEnableSOE(Ch);
    /* LSB or MSB */
    if(pModeConfig->Bitorder == UART_BITORDER_MSB)
    {
        MFS_UARTConfigMSB(Ch);
    }
    if(pModeConfig->Bitorder == UART_BITORDER_LSB)
    {
        MFS_UARTConfigLSB(Ch);
    }
    /* Stop bit */
    MFS_UARTConfigST(Ch, pModeConfig);
    /* Parity */
    if(pModeConfig->Parity == UART_PARITY_NONE)
    {
        MFS_UARTDisableParity(Ch);
    }
    else 
    {
        if(pModeConfig->Parity == UART_PARITY_ODD)
        {
            MFS_UARTConfigOddParity(Ch);
            MFS_UARTEnableParity(Ch);
        }
        if(pModeConfig->Parity == UART_PARITY_EVEN)
        {
            MFS_UARTConfigEvenParity(Ch);
            MFS_UARTEnableParity(Ch);
        }
    }
    /* Data bits */
    MFS_UARTConfigDT(Ch, pModeConfig);
    /* INV */
    if(pModeConfig->NRZ == UART_NRZ)
    {
        MFS_UARTConfigNoInv(Ch);
    }
    if(pModeConfig->NRZ == UART_INV_NRZ)
    {
        MFS_UARTConfigInv(Ch);
    }
    MFS_UARTConfigBaudrate(Ch, pModeConfig->Baudrate);
    return;
}
/*!
 ******************************************************************************
 ** \brief Initial the UART
 **
 ** \param None
 **
 ** \retval None
 **
 ******************************************************************************
 */

void DebugUartInit(void)
{
  InitUartPort();
  UARTConfigMode(IO_MFS_CH,&tUARTModeConfigT);
}
//---------------------------------------------------------------------------//
//
//! @brief 			向串口终端发送一个字符 
//! @author 		zozo
//! @note
//! @param 			Txdata为待发送的字符 
//! @return 		void
//!
//!	@deprecated none
//! @since 2012.6.15
//! @see   
//---------------------------------------------------------------------------//
void UartPutchar(char Txdata) 
{ 

  /* Enable the  TX*/
  MFS_UARTEnableTX(IO_MFS_CH);

	if(Txdata=='\n')
		{ 
	  //Txdata='\r'; 
		} 
	/* TX one byte */
	MFS_UARTTXOneData(IO_MFS_CH, (uint16_t)Txdata);
	  
	/* TX Finished? */
	while(!MFS_UARTGetTXBusIdleStatus(IO_MFS_CH));
	/* Disable the  TX after TX all bytes*/
	MFS_UARTDisableTX(IO_MFS_CH);	 
} 
//---------------------------------------------------------------------------//
//
//! @brief 			向串口终端发送字符串 
//! @author 		zozo
//! @note
//! @param 			pt为字符串指针。 
//! @return 		void
//!
//!	@deprecated none
//! @since 2012.6.15
//! @see   
//---------------------------------------------------------------------------//
void UartSendString(char *pt) 
{ 
    while(*pt) 
        UartPutchar(*pt++); 
} 

/*
	
*/
/******************************************************************* 
 * 函数名   :  IntToString 
 * 函数功能 ：  将一个整形的书转化成为一个表现形式一样的字符串 
 * 说明    ：  举例说明功能，若有一个整数为65535，该函数作用就是将其转化为字符串 
*           “65535” 
 * 形参    ：  temp为待装换的整数，pchar为转化成的字符串所存储的地址 
 * 返回值   ：  无 
********************************************************************/ 
void IntToString(int temp,char *pchar) 
{ 
   char ch,*p=pchar; 
   while(temp!=0) 
   { 
      *pchar++=(char)(temp%10+0x30); 
      temp/=10; 
   } 
   *pchar--='\0'; 
   while(pchar>p) 
   { 
      ch=*p; 
      *p++=*pchar; 
      *pchar--=ch; 
   } 
} 
/******************************************************************** 
 * 函数名   :  Vs_Printf 
 * 函数功能 ：  将参数指针arg所指的不定参数按照格式字符串format中的一一对应的格 
*           式转化后生成一个新的字符串存于地址s中。 
 * 形参    ：  s为最终形成的字符串存储地址，format为格式字符串，arg为不定参数指 
*           针。 
 * 返回值   ：  无 
********************************************************************/ 
void Vs_Printf(char *s,char *format,va_list arg) 
{ 
   char *pchar; 
   char *temp; 
   for(pchar=format;*pchar;pchar++){ 
      if(*pchar !='%'){ 
         *s++=*pchar; 
         continue; 
      } 
      switch(*++pchar){ 
         case 'd'  :{ 
				 	IntToString(va_arg(arg,int),s); 
            while(*s++); 
            *--s='0'; 
            break; 
         } 
         case 's'  :{ 
            temp=va_arg(arg,char *); 
            while(*s++=*temp++); 
            *--s='0'; 
            break; 
         } 
         case 'c'  :{ 
            *s++=va_arg(arg,char); 
            break; 
         } 
         default      :break; 
      } 
   } 
   *s='\0'; 
} 
/******************************************************************** 
 * 函数名   :  Printf 
 * 函数功能 ：  格式化输出
 * 形参    ：  fmt为格式字符串，定（类型不定、个数不定）形参 
 * 返回值   ：  无 
*******************************************************************/ 
void UartPrintf(char *fmt,...) 
{ 
   /******************************************************** 
    * va_list类型声明一个参数指针ap，该变量将依次引用各个参数 
    * ******************************************************/ 
   va_list ap; 
   char string[256]; 
   /******************************************************** 
    * va_start(ap,fmt),该函数将ap指针指向参数列表中的最后一个 
    * 有名参数fmt 
    * ******************************************************/ 
   va_start(ap,fmt); 
 
   /******************************************************** 
    * Vs_Printf,该函数将ap指向的无名参数按照fmt固定的格式转化 
    * 成为字符串存于string中 
    * ******************************************************/ 
		Vs_Printf(string,fmt,ap); 
		UartSendString(string); 
   /******************************************************** 
    * 最后调用va_end(ap)释放指针 
    * ******************************************************/ 
   va_end(ap); 
}

