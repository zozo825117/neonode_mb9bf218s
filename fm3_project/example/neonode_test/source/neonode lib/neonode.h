//---------------------------------------------------------------------------//
//
//! @file 		neonode.h
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

#ifndef __NEONODE_H__ 
#define __NEONODE_H__


//****************************************************************************
// Global Variables  
//****************************************************************************/

/*
	neonode command struct
*/
typedef struct { 
	uint8_t Size;
	uint8_t ID;
} commands_t; 

/*
	see neonode software guide : command Notifications  TouchNotification 
*/
typedef struct { 
	uint8_t State:   													4;/* Down = 0, Move = 1, Up = 2, 
																							  Invalid = 3,  Ghost detected = 4 */
	uint8_t id:   														4;
}touch_state_t;

typedef union { 
	uint8_t All;
	touch_state_t Bit;
} touch_state_u; 

typedef struct { 
	uint8_t X_lbyte;
	uint8_t X_hbyte;
	uint8_t Y_lbyte;
	uint8_t Y_hbyte;
	touch_state_u State ;   
	uint8_t Width;
	uint8_t Height; 
	uint8_t Pressure;
	uint8_t Probability;
} touch_notification_t;
/*
	Command Touch Notification struct
*/
typedef struct { 
	uint8_t Count;
	touch_notification_t tNotices[5];
} touch_notification_message_t; 


/*
	Touch finger motion information struct
*/
typedef struct { 
	uint16_t down                            				:1;  // 0 
	uint16_t up                              				:1;  // 1 
	uint16_t move                            				:1;  // 2 
	uint16_t pressed                         				:1;  // 3 
	uint16_t one_touch                       				:1;  // 4 
	uint16_t two_touch                       				:1;  // 5 
	uint16_t touch_move                      				:1;  // 6 
	uint16_t reserve                         				:1;  // 7 
	uint16_t move_down                       				:1;  // 8 
	uint16_t move_up                         				:1;  // 9 
	uint16_t move_right                      				:1;  // 10
	uint16_t move_left                       				:1;  // 11
	uint16_t move_down_right                 				:1;  // 12
	uint16_t move_down_left                  				:1;  // 13
	uint16_t move_up_right                   				:1;  // 14
	uint16_t move_up_left                    				:1;  // 15
}touch_motion_t;                               
/*
	Touch finger motion information union
*/
typedef union{
 uint16_t All;
 touch_motion_t Bit;
}touch_motion_u;
/*
	Touch finger information struct
	Singlefingre and multifinger delay conut and motion message
*/
typedef struct{
	uint8_t fCnt;
	uint16_t Last_X;
	uint16_t Last_Y;
	touch_motion_u fMotion;
}touch_finger_info_t;

/*
	Touch finger struct
	fType : 1 single finger >1 multifinger
	fss : finger state  
					0x80=detect down   
					0x40=single finger down  
					0x20=multi-finger down
					0x10=multi-finger move
	fCount : finger pressed time count
*/
typedef struct{
	uint8_t fType;
	uint8_t fss;            
	uint16_t fCount;
	touch_notification_t *ptNotices[5];
	touch_finger_info_t sfInfo[5];
	touch_finger_info_t mfInfo;
	//touch_finger_info_t mfInfo;
}touch_finger_message_t; 










//****************************************************************************
// 宏定义 
//****************************************************************************/
#define NUMBEROFAXISSIGNALS 								28
#define NUMBEROFAXISLEDS										28
#define NUMBEROFAXISPDS 										26
#define COUNT																5
#define ID_SIZE														  1

/*
Configuration commands
*/
#define DEACTIVATE_ID  											0x00                          
#define INITIALIZE_ID                       0x01
#define SETRESOLUTION_ID                    0x02
#define SETCONFIGURATION_ID                 0x03
#define STARTTOUCHDETECTION_ID              0x04
#define SETSCANNINGFREQUENCY_ID             0x08
#define SETTOUCHSIZELIMITS_ID               0x09
#define SETACTIVELEDS_ID                    0x1D
#define STARTPROXIMITYDETECTION_ID          0x2C
#define STOPPROXIMITYDETECTION_ID           0x2D
#define SETPROXIMITYSIZELIMITS_ID           0x2E

#define DEACTIVATE_SIZE 										0x01
#define INITIALIZE_SIZE                     0x01
#define SETRESOLUTION_SIZE                  0x05
#define SETCONFIGURATION_SIZE               0x05
#define STARTTOUCHDETECTION_SIZE            0x01
#define SETSCANNINGFREQUENCY_SIZE           0x09
#define SETTOUCHSIZELIMITS_SIZE             0x05
#define SETACTIVELEDS_SIZE                  0x05
#define STARTPROXIMITYDETECTION_SIZE        0x01
#define STOPPROXIMITYDETECTION_SIZE         0x01
#define SETPROXIMITYSIZELIMITS_SIZE         0x05

/*
Debug commands 
*/  
#define LOWSIGNALSREQUEST_ID 								0x0D
#define FIXEDSIGNALSTRENGTH_ID 							0x0F
#define LEDLEVELSREQUEST_ID						 			0x1C
#define FORCEDLEDLEVELSREQUEST_ID 					0x20
#define STATUSREQUEST_ID 										0x1E
#define OPENSHORTREQUEST_ID 								0x21
#define PRODUCTIDREQUEST_ID 								0x29	
                               
#define LOWSIGNALSREQUEST_SIZE 							0x02
#define FIXEDSIGNALSTRENGTH_SIZE  					0x03 
#define LEDLEVELSREQUEST_SIZE 						  0x02
#define FORCEDLEDLEVELSREQUEST_SIZE  				0x02 
#define STATUSREQUEST_SIZE  								0x01
#define OPENSHORTREQUEST_SIZE  							0x02
#define PRODUCTIDREQUEST_SIZE  							0x01

/*
Responses 
*/  
#define DEACTIVATERESPONSE_ID 							0x00
#define INITIALIZERESPONSE_ID 							0x01
#define SETRESOLUTIONRESPONSE_ID						0x02
#define SETCONFIGURATIONRESPONSE_ID 			 	0x03
#define SETSCANNINGFREQUENCYRESPONSE_ID 		0x08    				
#define SETTOUCHSIZELIMITSRESPONSE_ID				0x09
#define LOWSIGNALSRESPONSE_ID 							0x0D
#define FIXEDSIGNALSTRENGTHRESPONSE_ID 			0x0F
#define LEDLEVELSRESPONSE_ID 								0x1C
#define SETACTIVELEDSRESPONSE_ID 						0x1D
#define STATUSRESPONSE_ID  									0x1E
#define OPENSHORTRESPONSE_ID 								0x21
#define PRODUCTIDRESPONSE_ID    						0x29
#define STOPPROXIMITYDETECTIONRESPONSE_ID 	0x2D
#define SETPROXIMITYSIZELIMITSRESPONSE_ID 	0x2F
#define INVALIDCOMMANDRESPONSE_ID  					0xFE

#define DEACTIVATERESPONSE_SIZE  						0x02
#define INITIALIZERESPONSE_SIZE  						0x02 
#define SETRESOLUTIONRESPONSE_SIZE  				0x02
#define SETCONFIGURATIONRESPONSE_SIZE  			0x02 
#define SETSCANNINGFREQUENCYRESPONSE_SIZE  	0x02
#define SETTOUCHSIZELIMITSRESPONSE_SIZE  		0x02 
#define LOWSIGNALSRESPONSE_SIZE  						(3 + (NUMBEROFAXISSIGNALS + 7) / 8)
#define FIXEDSIGNALSTRENGTHRESPONSE_SIZE  	(3 + NUMBEROFAXISSIGNALS)
#define LEDLEVELSRESPONSE_SIZE  						(3 + (1.5 * NUMBEROFAXISSIGNALS))
#define SETACTIVELEDSRESPONSE_SIZE  				0x02
#define STATUSRESPONSE_SIZE  								0x81 
#define OPENSHORTRESPONSE_SIZE 							(2 + (2* NUMBEROFAXISPDS + 2* NUMBEROFAXISLEDS + 7) / 8)
#define PRODUCTIDRESPONSE_SIZE    					(1 + (ID_SIZE)) 
#define STOPPROXIMITYDETECTIONRESPONSE_SIZE 0x02 
#define SETPROXIMITYSIZELIMITSRESPONSE_SIZE 0x02 
#define INVALIDCOMMANDRESPONSE_SIZE  				0x02 




/*
Notifications
*/
#define TOUCHNOTIFICATION_ID 								0x04
#define BOOTCOMPLETENOTIFICATION_ID 				0x07
#define OVERRUNNOTIFICATION_ID 							0x25
#define PROXIMITYNOTIFICATION_ID 						0x2C

#define TOUCHNOTIFICATION_SIZE         			(2 + (9 * COUNT))
#define BOOTCOMPLETENOTIFICATION_SIZE				0x02
#define OVERRUNNOTIFICATION_SIZE 						0x01
#define PROXIMITYNOTIFICATION_SIZE  				(2 + (7 * COUNT)) 

/*
Internal Commands 
   	0x05 
    0x06 
   	0x0A 
   	0x22 
    0x30 
    0xEF 
*/

/*
 device address
*/
#define	ADDR_NEONODE   									0x50
#define READ		       									0x01	// I2C READ bit
#define WRITE		       									0x00	// I2C WRITE bit
/*
	Frame define
*/
#define FRAMESTART										 	0xEE
/*
	Touch State
*/
#define TOUCH_STATE_DOWN  							0x00
#define TOUCH_STATE_MOVE 								0x01 
#define TOUCH_STATE_UP 									0x02
#define TOUCH_STATE_INVALID							0x03  
#define TOUCH_STATE_GHOST_DETECTED			0x08 


/*
	Touch Message set count 
	unit is scan cycle 20ms
*/
#define CNT_DOWN										     2
#define CNT_SINGLE_TOUCH_MIN                 6
#define CNT_SINGLE_TOUCH_MAX                 25
/*
	move hysteresis threshold	
*/
#define MOVE_HYSTERE_THRES							 3
/*
	check some times to see whether the message direction continously
*/
#define CNT_CONTINU_MOVE								 2


/*
	IIC define
*/
#define IIC_COMM_INIT										I2cGpioInit();
//#define IIC_NO_BUSY                 	i2cWaitBusy()
#define IIC_PUT_BYTE(x)                	i2cPutbyte(x)
#define IIC_GET_BYTE(x)                	i2cGetbyte(x)
#define IIC_START_WRT             		 	i2cWriteStart()
#define IIC_START_READ            		 	i2cReadStart()

#define IIC_RDY_IN											IO_ReadGPIOPin(IIC_IO_PORT, IIC_RDY_IO_PIN)


//****************************************************************************
// 函数申明 
//****************************************************************************/
void CommsNeonodeInit(void);
uint8_t CommsNeonodeCmdRead(uint8_t *data);
uint8_t CommsNeonodeCmdWrite(uint8_t cmd_size,uint8_t *data);
uint8_t NeonodeSetting(void);
uint8_t NeonodeNewConvertion(void);
uint8_t CommsNeonodeCmdReadForce(uint8_t *data);

//****************************************************************************
// 变量申明 
//****************************************************************************/
extern touch_notification_message_t t_CTNs;

extern touch_finger_message_t t_TFM;


#endif // __NEONODE_H__