/*!
 ******************************************************************************
 **
 ** \file main.c
 **
 ** \brief 
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
#include "neonode.h"
#include "debug_printf.h"

/*---------------------------------------------------------------------------*/
/* Macro definition                                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* IO definition                                                             */
/*---------------------------------------------------------------------------*/

/* IIC transfer result */
#define   I2C_RET_OK             0
#define   I2C_RET_ERROR          1

/*---------------------------------------------------------------------------*/
/* Local functions prototypes                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Global data                                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Local data                                                               */
/*---------------------------------------------------------------------------*/
uint8_t buf[30];

//*****************************************************************************
//
// The most recent raw ADC reading for the X position on the screen.  This
// value is not affected by the selected screen orientation.
//
//*****************************************************************************
volatile short g_sTouchX;

//*****************************************************************************
//
// The most recent raw ADC reading for the Y position on the screen.  This
// value is not affected by the selected screen orientation.
//
//*****************************************************************************
volatile short g_sTouchY;
//*****************************************************************************
//
// The queue of debounced pen positions.  This is used to slightly delay the
// returned pen positions, so that the pen positions that occur while the pen
// is being raised are not send to the application.
//
//*****************************************************************************
static short g_psSamples[8];

//*****************************************************************************
//
// The count of pen positions in g_psSamples.  When negative, the buffer is
// being pre-filled as a result of a detected pen down event.
//
//*****************************************************************************
static signed char g_cIndex = 0;

/*
 
*/



//---------------------------------------------------------------------------//
//
//! @brief 			ÑÓÊ±DelayUs
//! @author 		zozo
//! @note
//! @param 			cycles
//! @return 		void
//£¡@deprecated  # of CPU cycles delayed is similar to "cycles". Specifically,\n 
//! 						it's ((cycles-15) % 6) + 15.  Not exact, but gives a sense of \n
//! 						the real-time delay.  Also, if MCLK ~1MHz, "cycles" is similar\n 
//! 						to # of useconds delayed.
//! @since 			2010.07.01
//! @see   
//---------------------------------------------------------------------------//
void DELAY_US (uint32_t cycles){ 
  while(cycles>15)                          // 15 cycles consumed by overhead
    cycles = cycles - 4;                    // 6 cycles consumed each iteration
}
//---------------------------------------------------------------------------//
//
//! @brief 			ÑÓÊ±º¯Êý 
//! @author 		zozo
//! @note
//! @param 			Uint16 cycles
//! @return 		void
//!
//!	@deprecated none
//! @since 2010.10.10
//! @see   
//---------------------------------------------------------------------------//
void DELAY_MS (uint32_t cycles){ 
	uint32_t cnt;
	while(cycles)
	{
		cycles = cycles - 1;
		cnt = MCLK;
		while(cnt)
		{
			DELAY_US(1000);
			cnt--;
		}
	}
}

//*****************************************************************************
//
//! Debounces presses of the touch screen.
//!
//! This function is called when a new X/Y sample pair has been captured in
//! order to perform debouncing of the touch screen.
//!
//! \return None.
//
//*****************************************************************************
static void TouchScreenDebouncer(void)
{
    long lX, lY;


    lX = g_sTouchX;
    lY = g_sTouchY;

    //
    // See if the touch screen is being touched.
    //
		/* up */
		if(t_TFM.sfInfo[0].fMotion.Bit.up)
			{
        //
        // See if there is a touch screen event handler.
        //
        //if(g_pfnTSHandler)
        {
            //
            // Send the pen up message to the touch screen event
            // handler.
            //
            //g_pfnTSHandler(WIDGET_MSG_PTR_UP, g_psSamples[g_cIndex],
                           //g_psSamples[g_cIndex + 1]);
             UartPrintf("1_UP,x=%d,y=%d\n",g_psSamples[g_cIndex],g_psSamples[g_cIndex + 1]);
        }
			}
	  /* down */
	  else if(t_TFM.sfInfo[0].fMotion.Bit.down)
			{
				g_psSamples[0] = lX;
				g_psSamples[1] = lY;
				g_cIndex = 2;
				//if(g_pfnTSHandler)
          {
              //
              // Send the pen down message to the touch screen event
              // handler.
              //
              //g_pfnTSHandler(WIDGET_MSG_PTR_DOWN, g_psSamples[0],
                             //g_psSamples[1]);
							UartPrintf("1_DOWN,x=%d,y=%d\n",g_psSamples[0],g_psSamples[1]);
          }
			}
		/* move */
	  else if(t_TFM.sfInfo[0].fMotion.Bit.move)
			{
        //
        // Store this sample into the sample buffer.
        //
        g_psSamples[g_cIndex] = lX;
        g_psSamples[g_cIndex + 1] = lY;

			
       //if(g_pfnTSHandler)
        {
            //
            // Send the pen move message to the touch screen event
            // handler.
            //
            //g_pfnTSHandler(WIDGET_MSG_PTR_MOVE, g_psSamples[g_cIndex],
                           //g_psSamples[g_cIndex + 1]);
            UartPrintf("1_MOVE,x=%d,y=%d\n",g_psSamples[g_cIndex],g_psSamples[g_cIndex + 1]);

        }
			 //
			 // Increment the index.
			 //
			 g_cIndex = (g_cIndex + 2) & 7;


			}
		else if(t_TFM.mfInfo.fMotion.Bit.move_down)
			{
					UartPrintf("multi-finger move down \n");
			}
		else if(t_TFM.mfInfo.fMotion.Bit.move_up)
			{
					UartPrintf("multi-finger move up \n");
			}
		else if(t_TFM.mfInfo.fMotion.Bit.one_touch)
			{
					UartPrintf("multi-finger one touch \n");
			}


}

/*
 ******************************************************************************
 ** \brief RT underflow interrupt handler
 **
 ** \param Sec Requested secondes
 ******************************************************************************
 */
static void RTUnderflowIntHandler(void)
{
	static uint8_t noKeyDly = 0;
	
	uint8_t updata;
	
	updata = NeonodeNewConvertion();
	
	if( updata )
		{
		  if(updata == 0x01)
		  	{
					g_sTouchX = t_TFM.sfInfo[0].Last_X;
					g_sTouchY = t_TFM.sfInfo[0].Last_Y;
		  	}
			noKeyDly = 0;
		}
		else
		{
			/* bug, if no response,then reinitial  */
						if(noKeyDly<50)
							{
								noKeyDly++;
							}
						else
							{
								if(CommsNeonodeCmdReadForce(buf))
									{
										if(buf[0] != 0xEE)
										{
											/* neonode init*/
											CommsNeonodeInit(); 
											delay_ms(10);//wait fot zForec DR high
											NeonodeSetting();
											UartPrintf("reset neonode! \n");
                      
										}
			
									}
                noKeyDly = 0;
							} 
		}
	

	TouchScreenDebouncer();
			
}

/*
 ******************************************************************************
 ** \brief RT trigger interrupt handler
 **
 ** \param Sec Requested secondes
 ******************************************************************************
 */
static void RTTrigIntHandler(void)
{
    //

}
/*!
******************************************************************************
** \brief  Main function of project 
**
** \param  none
**
** \retval uint32_t return value, if needed
******************************************************************************
*/
void base_timer_init()
{
    BT_RTInitRegT reg;   
    uint8_t ch, iomode;
    /* Set requested channel,If 32-bit mode used, use CH 0,2,4,6 */
    ch = USER_BT_CH ; 
    
    /* Set requested I/O mode */
    iomode = BT_IO_MODE_0;
    
    if(USER_BT_CH <= BT_CH_7)
    {
        /* Enable Bastimner Interrupt */
        NVIC_EnableIRQ(BTIM0_7_IRQn);
    }
    else
    {
#if (MCU_TYPE == DEVICE_TYPE2) || (MCU_TYPE == DEVICE_TYPE8)       
        /* Enable Bastimner Interrupt */
        NVIC_EnableIRQ(BTIM8_15_IRQn);
#endif  
    }
    
    /* Set initialization data */
    reg.Cycle = 720000;   //Cycle = T*(m+1)=20ms @ PCLK = 36MHz
    reg.Clock = BT_CLK_DIV_1;   //BT_CLK_DIV_4: T =   us @ PCLK = 36MHz
    reg.TimerMode = USER_TIMER_MODE;
    reg.InputEdge = BT_TRG_DISABLE;
    reg.Polarity = BT_POLARITY_NORMAL;
    reg.Mode = BT_MODE_CONTINUOUS; 
        
    /* Set IO Mode */
    BT_SetIOMode(ch,iomode);
    
    /* RT register initialization */
    BT_RTInit(ch,&reg); 
       
    /* Enable Interrupt */
    BT_RTEnableUnderflowInt(ch,RTUnderflowIntHandler);
    BT_RTEnableTrigInt(ch, RTTrigIntHandler);
    
    /* Enable count operatoin */
    BT_RTEnableCount(ch);  
    
    /* Start triggered by software */
    BT_RTStartSoftTrig(ch);   
		
}

/*!
 ******************************************************************************
 ** \brief  Main function of project 
 **
 ** \param  none
 **
 ** \retval uint32_t return value, if needed
 ******************************************************************************
 */
int32_t main(void)
{

   /* neonode init*/
	CommsNeonodeInit(); 
	delay_ms(10);//wait fot zForec DR high
	NeonodeSetting();

	/*base timer inital */
	base_timer_init();
  
  /* debug uart*/
  DebugUartInit();
	UartPrintf("hellow\n");
  //enable_int();

	while(1) 
		{ 

		} 
}

/*****************************************************************************/
/* END OF FILE */
