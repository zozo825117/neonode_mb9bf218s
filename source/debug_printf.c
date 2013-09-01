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
//! @brief 			�򴮿��ն˷���һ���ַ� 
//! @author 		zozo
//! @note
//! @param 			TxdataΪ�����͵��ַ� 
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
//! @brief 			�򴮿��ն˷����ַ��� 
//! @author 		zozo
//! @note
//! @param 			ptΪ�ַ���ָ�롣 
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
 * ������   :  IntToString 
 * �������� ��  ��һ�����ε���ת����Ϊһ��������ʽһ�����ַ��� 
 * ˵��    ��  ����˵�����ܣ�����һ������Ϊ65535���ú������þ��ǽ���ת��Ϊ�ַ��� 
*           ��65535�� 
 * �β�    ��  tempΪ��װ����������pcharΪת���ɵ��ַ������洢�ĵ�ַ 
 * ����ֵ   ��  �� 
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
 * ������   :  Vs_Printf 
 * �������� ��  ������ָ��arg��ָ�Ĳ����������ո�ʽ�ַ���format�е�һһ��Ӧ�ĸ� 
*           ʽת��������һ���µ��ַ������ڵ�ַs�С� 
 * �β�    ��  sΪ�����γɵ��ַ����洢��ַ��formatΪ��ʽ�ַ�����argΪ��������ָ 
*           �롣 
 * ����ֵ   ��  �� 
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
 * ������   :  Printf 
 * �������� ��  ��ʽ�����
 * �β�    ��  fmtΪ��ʽ�ַ������������Ͳ����������������β� 
 * ����ֵ   ��  �� 
*******************************************************************/ 
void UartPrintf(char *fmt,...) 
{ 
   /******************************************************** 
    * va_list��������һ������ָ��ap���ñ������������ø������� 
    * ******************************************************/ 
   va_list ap; 
   char string[256]; 
   /******************************************************** 
    * va_start(ap,fmt),�ú�����apָ��ָ������б��е����һ�� 
    * ��������fmt 
    * ******************************************************/ 
   va_start(ap,fmt); 
 
   /******************************************************** 
    * Vs_Printf,�ú�����apָ���������������fmt�̶��ĸ�ʽת�� 
    * ��Ϊ�ַ�������string�� 
    * ******************************************************/ 
		Vs_Printf(string,fmt,ap); 
		UartSendString(string); 
   /******************************************************** 
    * ������va_end(ap)�ͷ�ָ�� 
    * ******************************************************/ 
   va_end(ap); 
}

