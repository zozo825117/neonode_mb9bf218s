//---------------------------------------------------------------------------//
//
//! @file 		neonode.c
//! @brief    neonode specific functions for I2C Firmware library
//
//! @author 	zozo 
//! @version 	v0.1
//! @note
//!     
//! 
//! 修改日志  :
//! 2013.7.31    创建初试 zozo
//
//
//---------------------------------------------------------------------------//



//****************************************************************************
// 头文件
//****************************************************************************/
#include "fm3_common.h"
#include "Pdl_header.h"
#include "resource.h"
#include "neonode.h"
#include "iic.h"
#include "debug_printf.h"
#include "string.h"
  
//****************************************************************************
// 变量定义
//****************************************************************************/
commands_t NeonodeCmdLine[]={
	{ DEACTIVATE_SIZE 						     ,DEACTIVATE_ID  						       },   //0 
	{ INITIALIZE_SIZE                  ,INITIALIZE_ID                    },   //1 
	{ SETRESOLUTION_SIZE               ,SETRESOLUTION_ID                 },   //2 
	{ SETCONFIGURATION_SIZE            ,SETCONFIGURATION_ID              },   //3 
	{ STARTTOUCHDETECTION_SIZE         ,STARTTOUCHDETECTION_ID           },   //4 
	{ SETSCANNINGFREQUENCY_SIZE        ,SETSCANNINGFREQUENCY_ID          },   //5 
	{ SETTOUCHSIZELIMITS_SIZE          ,SETTOUCHSIZELIMITS_ID            },   //6 
	{ SETACTIVELEDS_SIZE               ,SETACTIVELEDS_ID                 },   //7 
	{ STARTPROXIMITYDETECTION_SIZE     ,STARTPROXIMITYDETECTION_ID       },   //8 
	{ STOPPROXIMITYDETECTION_SIZE      ,STOPPROXIMITYDETECTION_ID        },   //9 
	{ SETPROXIMITYSIZELIMITS_SIZE      ,SETPROXIMITYSIZELIMITS_ID        },   //10
	{ LOWSIGNALSREQUEST_SIZE 			     ,LOWSIGNALSREQUEST_ID 			       },   //11
	{ FIXEDSIGNALSTRENGTH_SIZE  	     ,FIXEDSIGNALSTRENGTH_ID 		       },   //12
	{ LEDLEVELSREQUEST_SIZE 			     ,LEDLEVELSREQUEST_ID				       },   //13
	{ FORCEDLEDLEVELSREQUEST_SIZE      ,FORCEDLEDLEVELSREQUEST_ID        },   //14
	{ STATUSREQUEST_SIZE  				     ,STATUSREQUEST_ID 					       },   //15
	{ OPENSHORTREQUEST_SIZE  			     ,OPENSHORTREQUEST_ID 			       },   //16
	{ PRODUCTIDREQUEST_SIZE  			     ,PRODUCTIDREQUEST_ID 			       },   //17
	{ DEACTIVATERESPONSE_SIZE  				 ,DEACTIVATERESPONSE_ID 					 },   //18
	{ INITIALIZERESPONSE_SIZE  				 ,INITIALIZERESPONSE_ID 					 },   //19
	{ SETRESOLUTIONRESPONSE_SIZE  		 ,SETRESOLUTIONRESPONSE_ID				 },   //20
	{ SETCONFIGURATIONRESPONSE_SIZE  	 ,SETCONFIGURATIONRESPONSE_ID 		 },   //21
	{ SETSCANNINGFREQUENCYRESPONSE_SIZE,SETSCANNINGFREQUENCYRESPONSE_ID  },   //22
	{ SETTOUCHSIZELIMITSRESPONSE_SIZE  ,SETTOUCHSIZELIMITSRESPONSE_ID		 },   //23
	{ LOWSIGNALSRESPONSE_SIZE  				 ,LOWSIGNALSRESPONSE_ID 					 },   //24
	{ FIXEDSIGNALSTRENGTHRESPONSE_SIZE ,FIXEDSIGNALSTRENGTHRESPONSE_ID 	 },   //25
	{ LEDLEVELSRESPONSE_SIZE  				 ,LEDLEVELSRESPONSE_ID 						 },   //26
	{ SETACTIVELEDSRESPONSE_SIZE  		 ,SETACTIVELEDSRESPONSE_ID 		     },   //27
	{ STATUSRESPONSE_ID  							 ,STATUSRESPONSE_SIZE  						 },		//28  
	{ OPENSHORTRESPONSE_ID 						 ,OPENSHORTRESPONSE_SIZE 					 },		//29
	{ PRODUCTIDRESPONSE_ID    				 ,PRODUCTIDRESPONSE_SIZE    			 },		//30
	{ STOPPROXIMITYDETECTIONRESPONSE_ID,STOPPROXIMITYDETECTIONRESPONSE_SIZE}, //31
	{ SETPROXIMITYSIZELIMITSRESPONSE_ID,SETPROXIMITYSIZELIMITSRESPONSE_SIZE}, //32
	{ INVALIDCOMMANDRESPONSE_ID  			 ,INVALIDCOMMANDRESPONSE_SIZE  			 }, //33
	{ TOUCHNOTIFICATION_SIZE        	 ,TOUCHNOTIFICATION_ID 				     },   //34
	{ BOOTCOMPLETENOTIFICATION_SIZE	   ,BOOTCOMPLETENOTIFICATION_ID      },   //35
	{ OVERRUNNOTIFICATION_SIZE 			   ,OVERRUNNOTIFICATION_ID 			     },   //36
	{ PROXIMITYNOTIFICATION_SIZE  	   ,PROXIMITYNOTIFICATION_ID 		     }    //37
};                                                                            

touch_notification_message_t t_CTNs;

touch_finger_message_t t_TFM;



/*
	Function name:	CommsNeonodeInit
	Parameters:			none
	Return:					none
	Description:		communication io inital  
*/

void CommsNeonodeInit(void)
{
	IIC_COMM_INIT;

	IO_DisableFunc(IIC_IO_PORT,IIC_RDY_IO_PIN);
	IO_ConfigGPIOPin(IIC_IO_PORT,IIC_RDY_IO_PIN,
									 IO_DIR_INPUT,IO_PULLUP_ON);


	
  /**/
	IO_DisableFunc(IIC_RST_PORT,IIC_RST_IO_PIN);
	IO_WriteGPIOPin(IIC_RST_PORT,IIC_RST_IO_PIN,IO_BIT_CLR);
	IO_ConfigGPIOPin(IIC_RST_PORT,IIC_RST_IO_PIN,
									 IO_DIR_OUTPUT,IO_PULLUP_ON);
	delay_us(100);
	IO_WriteGPIOPin(IIC_RST_PORT,IIC_RST_IO_PIN,IO_BIT_SET);
	
}
/*
	Function name:	CommsNeonodeCmdRead
	Parameters:			uint8_t *data
	Return:					TRUE or FALSE 
	Description:	
*/
uint8_t CommsNeonodeCmdRead(uint8_t *data)
{
	uint8_t size;
  uint16_t cnt = 1000; //wait limit : 1s
	while( IO_ReadGPIOPin(IIC_IO_PORT,IIC_RDY_IO_PIN) && cnt--)
		{
			delay_us(1000);
		};// wait data ready Low
	if(cnt==0)
		{
			goto read_error;
		}
	
	/* 发送读地址*/
	if(IIC_START_READ==FALSE)
		{
      __nop();
			goto read_error;
		}
	delay_us(3 );
	/* read FrameStart */
	*data++ = IIC_GET_BYTE(0);
	delay_us(3 );
	*data++ = size = IIC_GET_BYTE(1);
	
	if(size >= 0xff)
		{
		*data++ = IIC_GET_BYTE(1);
		  goto read_too_long;
		}
	IIC_SEND_STOP;
	delay_us(100);

	/* 发送读地址*/
	if(IIC_START_READ==FALSE)
		{
      goto read_error;
		}
	while (size>1)
		{
			*data++ = IIC_GET_BYTE(0);//读 ack
			delay_us(2 );
			size--;
		}
	*data = IIC_GET_BYTE(1);//读 nack
  IIC_SEND_STOP;											// 发送停止位
  return TRUE;
	
read_error:
read_too_long:		
	IIC_SEND_STOP;

	return FALSE;


}
/*
	Function name:	CommsNeonodeCmdReadForce
	Parameters:			uint8_t *data
	Return:					TRUE or FALSE 
	Description:	
*/
uint8_t CommsNeonodeCmdReadForce(uint8_t *data)
{
	uint8_t size;
	
	/* 发送读地址*/
	if(IIC_START_READ==FALSE)
		{
      __nop();
			goto read_error;
		}
	delay_us(3 );
	/* read FrameStart */
	*data++ = IIC_GET_BYTE(0);
	delay_us(3 );
	*data++ = size = IIC_GET_BYTE(1);
	
	if(size >= 0xff)
		{
		*data++ = IIC_GET_BYTE(1);
		  goto read_too_long;
		}
	IIC_SEND_STOP;
	delay_us(100);

	/* 发送读地址*/
	if(IIC_START_READ==FALSE)
		{
      goto read_error;
		}
	while (size>1)
		{
			*data++ = IIC_GET_BYTE(0);//读 ack
			delay_us(2 );
			size--;
		}
	*data = IIC_GET_BYTE(1);//读 nack
  IIC_SEND_STOP;											// 发送停止位
  return TRUE;
	
read_error:
read_too_long:		
	IIC_SEND_STOP;

	return FALSE;


}

/*
	Function name:	CommsNeonodeCmdWrite
	Parameters:			uint8_t cmd_size,Uint8 *data
	Return:					TRUE or FALSE 
	Description:	
*/
uint8_t CommsNeonodeCmdWrite(uint8_t cmd_size,uint8_t *data)
{

	/* 确定总线空闲
	if(IIC_NO_BUSY == FALSE)							 
		{
			continue;
		}
		*/
	
	/* 发送写地址 */
	if(IIC_START_WRT==FALSE)
		{
			goto write_error;
		}
	delay_us(3 );
	/* 发送 FrameStart*/
	if(IIC_PUT_BYTE(FRAMESTART)==FALSE)
		{
		 	goto write_error;
		}
	delay_us(3);
	/* 发送数据长度 */
	if(IIC_PUT_BYTE(cmd_size)==FALSE)
		{
		 goto write_error;
		}					
	delay_us(3);
	while (cmd_size--)
		{
			if(IIC_PUT_BYTE(*data++)==FALSE)
				{
				 goto write_error;
				}
			delay_us(3 );
		}
	IIC_SEND_STOP;											// 发送停止位
	return TRUE;
		
		
write_error:
	IIC_SEND_STOP;											// 发送停止位
	return FALSE;	
}

/*
  Function name:  	InSlider
  Parameters:    		touch_finger_info_t *infofinger ,int16_t x,int16_t y,int16_t last_x,int16_t last_y
  Return:        	 	rc: 1 updata 0 no
  Description:  		determine the direction of movement  of the last and now coodinate 
*/

uint8_t InSlider(touch_finger_info_t *infofinger ,int16_t x,int16_t y,int16_t last_x,int16_t last_y)
{
  int16_t delta_x,delta_y,rc = SET;; 


	/* 
		 finger move distance  must above move hysteresis threshold	
	*/
	
	delta_x = x - last_x;
	delta_y = y - last_y;

	if(delta_y > MOVE_HYSTERE_THRES)
		{
			/*up left*/
			if(delta_x < -MOVE_HYSTERE_THRES )
				{
					 infofinger->fMotion.Bit.move_up_left = SET;
				}
	
			/*up right*/
			else if(delta_x > MOVE_HYSTERE_THRES)
				{
					infofinger->fMotion.Bit.move_up_right = SET;
				}
	
			/*up */
			else
				{
					infofinger->fMotion.Bit.move_up = SET;
				}
		}
	else if(delta_y < -MOVE_HYSTERE_THRES)
		{
			/*down left*/
			if(delta_x < -MOVE_HYSTERE_THRES )
				{
					 infofinger->fMotion.Bit.move_down_left = SET;
				}
			
			/*down right*/
			else if(delta_x > MOVE_HYSTERE_THRES)
				{
					infofinger->fMotion.Bit.move_down_right = SET;
				}
			
			/*down */
			else
				{
					infofinger->fMotion.Bit.move_down = SET;
				}
	
		}
	else
		{
			/*down left*/
			if(delta_x < -MOVE_HYSTERE_THRES )
				{
					 infofinger->fMotion.Bit.move_left = SET;
				}
			
			/*up right*/
			else if(delta_x > MOVE_HYSTERE_THRES)
				{
					infofinger->fMotion.Bit.move_right = SET;
				}
			else
				{
				  rc = RESET;
				}
		}
	
    return rc;
}
/*
	Function name:	multiFingerProc
	Parameters: 		uint8_t updata : have touch notification
								touch_finger_message_t *t_finger
	Return: 				None
	Description:	
*/

void multiFingerProc(uint8_t updata,touch_finger_message_t *t_finger)
{
	static union{
			uint8_t all;
			struct{
				uint8_t one_finger_move_down							 :1;
				uint8_t two_finger_move_down							 :1;
				uint8_t one_finger_move_up								 :1;
				uint8_t two_finger_move_up								 :1;
			}bit;
	
		}mulit_buf;

	/*multi-finger up down*/
	t_finger->mfInfo.fMotion.All &=~ 0xff77;	
	
	if(updata)
	{
	 if(updata & 0x01)
		{
		 mulit_buf.bit.one_finger_move_down = t_finger->sfInfo[0].fMotion.Bit.move_down|
																					t_finger->sfInfo[0].fMotion.Bit.move_down_right|
																					t_finger->sfInfo[0].fMotion.Bit.move_down_left; 
		 mulit_buf.bit.one_finger_move_up = t_finger->sfInfo[0].fMotion.Bit.move_up|
																				t_finger->sfInfo[0].fMotion.Bit.move_up_right|
																				t_finger->sfInfo[0].fMotion.Bit.move_up_left; 
	
		}
	 if(updata & 0x02)
		{
		 mulit_buf.bit.two_finger_move_down = t_finger->sfInfo[1].fMotion.Bit.move_down|
																					t_finger->sfInfo[1].fMotion.Bit.move_down_right|
																					t_finger->sfInfo[1].fMotion.Bit.move_down_left; 
		 mulit_buf.bit.two_finger_move_up = t_finger->sfInfo[1].fMotion.Bit.move_up|
																				t_finger->sfInfo[1].fMotion.Bit.move_up_right|
																				t_finger->sfInfo[1].fMotion.Bit.move_up_left; 

		}
	
		if((mulit_buf.all & 0x03) == 0x03)
			{
				t_finger->mfInfo.fMotion.Bit.move_down = SET;
			}
		else if((mulit_buf.all & 0x0c) == 0x0c)
			{
				t_finger->mfInfo.fMotion.Bit.move_up = SET;
			}
		else if(t_finger->sfInfo[1].fMotion.Bit.one_touch)
			{
				t_finger->mfInfo.fMotion.Bit.one_touch= SET;
			} 		 
	}

}

/*
  Function name:  twoFingerProc
  Parameters:    	uint8_t updata : have touch notification
  							uint8_t no: which finger   0 or 1 
							touch_finger_message_t *t_finger
  Return:         None
  Description:  
*/

void twoFingerProc(uint8_t updata,uint8_t no,touch_finger_message_t *t_finger)
{

//  static int16_t cnt=0; 
  int16_t now_x,now_y; 
//  static touch_motion_u  last_motion;

  /*up/down/move/one_thouch/two_touch/all direction   message reset before set*/
  t_finger->sfInfo[no].fMotion.All &= ~0xff77;

  if(updata & 0x02)
    {
			now_x =(t_finger->ptNotices[no]->X_lbyte|t_finger->ptNotices[no]->X_hbyte<<8);
			now_y =(t_finger->ptNotices[no]->Y_lbyte|t_finger->ptNotices[no]->Y_hbyte<<8);
		

      switch(t_finger->ptNotices[no]->State.Bit.State & 0x03)
        {
          case TOUCH_STATE_DOWN:
              /*
                 if two finger down set reset one finger down message
     
              */
               t_finger->sfInfo[no].fMotion.Bit.down= SET;
               t_finger->sfInfo[no].fMotion.Bit.pressed= SET;  
               t_finger->sfInfo[0].fMotion.Bit.up = RESET;   
               t_finger->fss = 0x20;
							 t_finger->fType= 2;
							 //UartPrintf("finger %d down x=%d y=%d \n",no+1,now_x,now_y);
            break;
          case TOUCH_STATE_UP:
                t_finger->sfInfo[no].fMotion.Bit.up = SET;
                t_finger->sfInfo[no].fMotion.Bit.pressed= RESET;
								/* no move and the right time is considered to be within the click action */
								if((t_finger->fCount >= CNT_SINGLE_TOUCH_MIN) && (t_finger->fCount <= CNT_SINGLE_TOUCH_MAX))									 
                  {
                  	
                    if(!(t_finger->fss & 0x10))
                    	{
                    	  t_finger->sfInfo[no].fMotion.Bit.one_touch= SET;
                    	}
                    
                  }
								t_finger->fCount = 0;
                t_finger->fss &=~ 0x30;
								t_finger->fType= 0;
								now_x = now_y = 0;
								//UartPrintf("finger %d up x=%d y=%d \n",no+1,now_x,now_y);
            break;
          case TOUCH_STATE_MOVE:

						 t_finger->sfInfo[no].fMotion.Bit.move = SET;
						 
            if(t_finger->sfInfo[no].Last_X!=0 && t_finger->sfInfo[no].Last_Y!=0)
            {
              if(InSlider(&t_finger->sfInfo[no],now_x,now_y,t_finger->sfInfo[no].Last_X,t_finger->sfInfo[no].Last_Y))
              	{
              		t_finger->fss |= 0x10;
									//UartPrintf("finger %d motion=%d x=%d y=%d \n",no+1,t_finger->sfInfo[no].fMotion.All,now_x,now_y);
              	}
							else
								{

								}
            }
						
												
						/* check some times to see whether the message direction continously  
            if((last_motion.All&0xFF00)!=(t_finger->sfInfo[no].fMotion.All & 0xff00))
              {
                last_motion.All = (t_finger->sfInfo[no].fMotion.All & 0xff00);
                if(cnt <= CNT_CONTINU_MOVE)
                	{
                    cnt++;
										t_finger->sfInfo[no].fMotion.All &= ~0xff00;
                  }
              }
            else
              {
                cnt = 0;
              }*/
          	//UartPrintf("finger %d move x=%d y=%d \n",no+1,now_x,now_y);

            break;  

          default:
            break;
            
        }
			t_finger->sfInfo[no].Last_X = now_x;
			t_finger->sfInfo[no].Last_Y = now_y;
    }
  else
    {
      /* no notification */

    }

}

/*
  Function name: oneFingerProc
  Parameters:    	uint8_t updata : have touch notification
  							uint8_t no: which finger   0 or 1 
                 			 touch_finger_message_t *t_finger
  Return:         None
  Description:  
*/

void oneFingerProc(uint8_t updata,uint8_t no,touch_finger_message_t *t_finger)
{

// static int16_t cnt=0; 
  int16_t now_x,now_y; 
// static touch_motion_u  last_motion;

   /*up/down/move/one_thouch/two_touch/all direction   message reset before set*/
  t_finger->sfInfo[no].fMotion.All &= ~0xff77;

  if(updata&0x01)
    {
      now_x =(t_finger->ptNotices[no]->X_lbyte|t_finger->ptNotices[no]->X_hbyte<<8);
      now_y =(t_finger->ptNotices[no]->Y_lbyte|t_finger->ptNotices[no]->Y_hbyte<<8);

      /*
       	 	if one finger down add some delay to set any flag 
     		 */
      switch(t_finger->ptNotices[no]->State.Bit.State & 0x03)
        {
          case TOUCH_STATE_DOWN:
								if(!(t_finger->fss&0x20))
									{
										 t_finger->fss = 0x80;
									}
               
            break;
          case TOUCH_STATE_UP:
            if(t_finger->fss & 0x40)
              {
                t_finger->sfInfo[no].fMotion.Bit.up = SET;
                if(t_finger->fCount >= CNT_SINGLE_TOUCH_MIN && t_finger->fCount <= CNT_SINGLE_TOUCH_MAX)                   
                  {
                    t_finger->sfInfo[no].fMotion.Bit.one_touch= SET;
                  }
								t_finger->fCount = 0;
								t_finger->fType= 0;
								//UartPrintf("finger %d up x=%d y=%d \n",no+1,now_x,now_y);
              }
            t_finger->sfInfo[no].fMotion.Bit.pressed= RESET;  
						t_finger->fss &=~ 0xC8;
						now_x = now_y = 0;
            break;
          case TOUCH_STATE_MOVE:
              if(t_finger->fss & 0x40)
                {
                  t_finger->sfInfo[no].fMotion.Bit.move = SET;
									//UartPrintf("finger %d move x=%d y=%d \n",no+1,now_x,now_y);
								}	
							if(t_finger->sfInfo[no].Last_X!=0 && t_finger->sfInfo[no].Last_Y!=0)
							{
								if(InSlider(&t_finger->sfInfo[no],now_x,now_y,t_finger->sfInfo[no].Last_X,t_finger->sfInfo[no].Last_Y))
									{
										t_finger->fss |= 0x08;
										//UartPrintf("finger %d motion=%d x=%d y=%d \n",no+1,t_finger->sfInfo[no].fMotion.All,now_x,now_y);
									}
								else
									{
							
									}
							}

								/* check some times to see whether the message direction continously	
								if((last_motion.All&0xFF00)!=(t_finger->sfInfo[no].fMotion.All & 0xff00))
									{
                    last_motion.All = (t_finger->sfInfo[no].fMotion.All & 0xff00);
										if(cnt <= CNT_CONTINU_MOVE)
											{
												cnt++;
												t_finger->sfInfo[no].fMotion.All &= ~0xff00;
											}
									}
								else
									{
										cnt = 0;
									}*/
								

               
            break;  

          default:
            break;
            
        }
			t_finger->sfInfo[no].Last_X = now_x;
			t_finger->sfInfo[no].Last_Y = now_y;

    }
  else
    {
      /* no notification */

    }
    /* one finger detected wait some times to check  */
    if(t_finger->fss&0x80)
      {
        t_finger->fss++;
        if((t_finger->fss&0x87) == (0x80+CNT_DOWN))
          {
            t_finger->fss = 0x40;
						t_finger->fType= 1;
            t_finger->sfInfo[no].fMotion.Bit.down = SET;		
            t_finger->sfInfo[no].fMotion.Bit.pressed= SET;
            //UartPrintf("finger %d down x=%d y=%d \n",no+1,now_x,now_y);

          }
      }
}
/*
	Function name:	TouchMovetionProcess
	Parameters:			None
	Return:					None
	Description:	
*/
uint8_t TouchMotionProcess(uint8_t updata,touch_finger_message_t *t_finger)
{
	uint8_t rc=SET;
	
  /*one finger*/
  oneFingerProc(updata,0,t_finger);
 


  if(!(t_finger->fss & 0x40) )
    {
    	/*two finger*/
      twoFingerProc(updata,1,t_finger);
			
			/*multi finger*/

			multiFingerProc(updata, t_finger);
    }

  /*pressed time*/
  if(t_finger->sfInfo[0].fMotion.Bit.pressed || t_finger->sfInfo[1].fMotion.Bit.pressed )
    {
       t_finger->fCount++;
    }
  else
    { 
      t_finger->fCount=0;
    }
  return rc;
}
/*
	Function name:	whichFingerIs
	Parameters:			touch_notification_message_t *touch_notice
									touch_finger_message_t *t_finger
	Return:					updata: updata 0x01 finger 1  ; 0x02 finger 2  ;0 none
	Description:	  find and move the notification is the right finger has. Now only suppost two finger
*/
uint8_t whichFingerIs(touch_notification_message_t *touch_notice,
										touch_finger_message_t *t_finger)
{
	uint8_t updata=0;
	if(touch_notice->Count == 1)
		{
      if(touch_notice->tNotices[0].State.Bit.State & TOUCH_STATE_GHOST_DETECTED)
      	{
      		t_finger->ptNotices[1] = &touch_notice->tNotices[0];
					updata |= 0x02;
      	}
			else 
				{
					t_finger->ptNotices[0] = &touch_notice->tNotices[0];
					updata |= 0x01;
				}
			

			 
		}
	else if(touch_notice->Count == 2)
		{
			/* 
			  if ghost detected,means two finger is,now only support two finger. 
			*/
			if(touch_notice->tNotices[0].State.Bit.State & TOUCH_STATE_GHOST_DETECTED)
				{
					t_finger->ptNotices[1]= &touch_notice->tNotices[0];
					t_finger->ptNotices[0] = &touch_notice->tNotices[1];
				}
			else if(touch_notice->tNotices[1].State.Bit.State & TOUCH_STATE_GHOST_DETECTED)
				{
					t_finger->ptNotices[1] = &touch_notice->tNotices[1];
					t_finger->ptNotices[0] = &touch_notice->tNotices[0];
				}	
			updata |= 0x01|0x02;
		}
	return updata;
}
/*
	Function name:	NeonodeSetting
	Parameters: 		None
	Return: 				None
	Description:	
*/
uint8_t NeonodeSetting(void)
{
	uint8_t buf[20];
	/* wait for boot */
	do
		{
			if(! CommsNeonodeCmdRead(buf))
			{
				/* Initialize */
				//buf[0] = NeonodeCmdLine[1].ID;
				//CommsNeonodeCmdWrite(NeonodeCmdLine[0].Size,buf);
				//CommsNeonodeCmdRead(buf);
				IO_WriteGPIOPin(IIC_RST_PORT,IIC_RST_IO_PIN,IO_BIT_CLR);
				delay_us(100);
				IO_WriteGPIOPin(IIC_RST_PORT,IIC_RST_IO_PIN,IO_BIT_SET);
				delay_us(100);
			}
			
			;
		}
	while((buf[2] != BOOTCOMPLETENOTIFICATION_ID) && (buf[3]!=0));//ready to receive data
	
	/* setting */
	/* Deactivate */
	buf[0] = NeonodeCmdLine[0].ID;
	CommsNeonodeCmdWrite(NeonodeCmdLine[0].Size,buf);
	CommsNeonodeCmdRead(buf);
	if(buf[buf[1]+1] == 0)
		{
			//ok
		}
	/*step 2 Initialize */
	buf[0] = NeonodeCmdLine[1].ID;
	CommsNeonodeCmdWrite(NeonodeCmdLine[1].Size,buf);
	CommsNeonodeCmdRead(buf);
	if(buf[buf[1]+1] == 0)
		{
			//ok
		}
	/*step 3 SetResolution */
	buf[0] = NeonodeCmdLine[2].ID;
	buf[1] = 0x20;
	buf[2] = 0x03;
	buf[3] = 0xE0;
	buf[4] = 0x01;
	CommsNeonodeCmdWrite(NeonodeCmdLine[2].Size,buf);
	CommsNeonodeCmdRead(buf);
	if(buf[buf[1]+1] == 0)
		{
			//ok
		}
	/*step 4 SetConfiguration */
	buf[0] = NeonodeCmdLine[3].ID;
	buf[1] = 0x01; // Dual touch	 // 0 = off, 1 = on 
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = 0x00;
	CommsNeonodeCmdWrite(NeonodeCmdLine[3].Size,buf);
	CommsNeonodeCmdRead(buf);
	if(buf[buf[1]+1] == 0)
		{
			//ok
		}
	/*step 9 SetTouchSizeLimits */
	buf[0] = NeonodeCmdLine[6].ID;
	buf[1] = 0x01; // 
	buf[2] = 0xFF;
	buf[3] = 0x01;
	buf[4] = 0x00;
	CommsNeonodeCmdWrite(NeonodeCmdLine[6].Size,buf);
	CommsNeonodeCmdRead(buf);
	if(buf[buf[1]+1] == 0) 
		{
			//ok
			__nop();//0xfe fe
		}
	/*step 10 SetScanningFrequency */
	buf[0] = NeonodeCmdLine[5].ID;
	buf[1] = 40; // Idle Frequency	 // Low Byte 
	buf[2] = 0x00; // Idle Frequency	 // High Byte 
	buf[3] = 40; // Finger Frequency	 // Low Byte 
	buf[4] = 0x00; // Finger Frequency	 // High Byte 
	buf[5] = 40; // Stylus Frequency	 // Low Byte 
	buf[6] = 0x00; // Stylus Frequency	 // High Byte 
	//buf[7] = 0xFA; // Proximity Frequency 	// Low Byte 
	//buf[8] = 0x00; // Proximity Frequency 	// High Byte	
	CommsNeonodeCmdWrite(NeonodeCmdLine[5].Size-2,buf);
	CommsNeonodeCmdRead(buf);
	if(buf[buf[1]+1] == 0) 
		{
			//ok
			__nop();//0xfe fe
		}


	/*step 11 	write 0x1A read return 0x1A=0 delay is 40.7ms */
	buf[0] = 0x1A;
	CommsNeonodeCmdWrite(1,buf);
	CommsNeonodeCmdRead(buf);
	if(buf[2] == 0x1A) 
		{
			//ok
			__nop();//0xfe fe
		}
	/*step 12 StartTouchDetection */
	buf[0] = NeonodeCmdLine[4].ID;
	CommsNeonodeCmdWrite(NeonodeCmdLine[4].Size,buf);
  
  /*Initial finger state */
  t_TFM.fss = 0;

	return TRUE;
	
}

/*
	Function name:	NeonodeNewConvertion
	Parameters:			None
	Return:					updata: updata 0x01 finger 1  ; 0x02 finger 2  ;0 none
	Description:	
*/
uint8_t NeonodeNewConvertion(void)
{
	uint8_t buf[30],len,updata = 0;
	if(!(IIC_RDY_IN))
		{
			if(CommsNeonodeCmdRead(buf))
				{
					/* touch_notification */
					if(buf[2]==NeonodeCmdLine[34].ID)
						{
						  len = buf[1]-1;
						  memcpy((uint8_t *)&t_CTNs.Count,&buf[3],len);
							updata = whichFingerIs(&t_CTNs,&t_TFM);
						}
				}
		}
	
	TouchMotionProcess(updata,&t_TFM);

	return updata;
}


