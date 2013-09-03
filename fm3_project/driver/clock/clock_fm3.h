/*!
 ******************************************************************************
 **
 ** \file clock_fm3.h
 **
 ** \brief clock module driver
 ** 
 **
 ** \author 
 **
 ** \version V0.01
 **
 ** \date 2011-11-29
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */ 
 
#ifndef _CLOCK_FM3_H_
#define _CLOCK_FM3_H_


/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_CLOCK == OFF)
    #if (PERIPHERAL_ENABLE_CLOCK == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_CLOCK == ON) && (PERIPHERAL_AVAILABLE_CLOCK == ON)
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

/**
 ******************************************************************************
 ** System Clock Mode Control Register value definition
 **        <i>(USER SETTING)</i>
 ** 
 ** SCM_CTL
 **
 ** Bit#7-5 : RCS[2:0]
 ** - 0 = Internal high-speed CR oscillation (default)
 ** - 1 = Main oscillation clock
 ** - 2 = PLL oscillation clock
 ** - 3 = (not allowed)
 ** - 4 = Internal low-speed CR oscillation
 ** - 5 = Sub clock oscillation
 ** - 6 = (not allowed)
 ** - 7 = (not allowed)
 **
 ** Bit#4 : PLLE
 ** - 0 = Disable PLL (default)
 ** - 1 = Enable PLL
 **
 ** Bit#3 : SOSCE
 ** - 0 = Disable sub oscillation (default)
 ** - 1 = Enable sub oscillation
 **
 ** Bit#2 : (reserved)
 **
 ** Bit#1 : MOSCE
 ** - 0 = Disable main oscillation (default)
 ** - 1 = Enable main oscillation  
 **
 ** Bit#0 : (reserved)  
 ******************************************************************************/ 

/* Clock mode */
/*! \brief Internal high-speed CR oscillation */
#define CLOCK_MODE_HIGH_CR         (0)
/*! \brief Main oscillation clock */
#define CLOCK_MODE_MAIN_OSC        (1)
/*! \brief PLL oscillation clock */
#define CLOCK_MODE_MAIN_PLL        (2)
/*! \brief Internal low-speed CR oscillation clock */
#define CLOCK_MODE_LOW_CR          (4) 
/*! \brief Sub oscillation clock */
#define CLOCK_MODE_SUB_OSC         (5)

/* Sub oscillator on/off definition*/
/*! \brief Sub oscillator off */
#define CLOCK_SUB_OSC_OFF     (0)
/*! \brief Sub oscillator on */
#define CLOCK_SUB_OSC_ON      (1)

/* Main oscillator on/off definition*/
/*! \brief Main oscillator off */
#define CLOCK_MAIN_OSC_OFF     (0)
/*! \brief Main oscillator on */
#define CLOCK_MAIN_OSC_ON      (1)

/* Low speed CR on/off definition*/
/*! \brief Low speed CR off */
#define CLOCK_LOW_CR_OFF     (0)
/*! \brief Low speed CR on */
#define CLOCK_LOW_CR_ON      (1)

/* High speed CR on/off definition*/
/*! \brief High speed CR off */
#define CLOCK_HIGH_CR_OFF     (0)
/*! \brief High speed CR on */
#define CLOCK_HIGH_CR_ON      (1)

/* PLL on/off definition*/
/*! \brief PLL off */
#define CLOCK_MAINPLL_OFF     (0)
/*! \brief PLL on */
#define CLOCK_MAINPLL_ON      (1)

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* External variables                                                        */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void SwitchToMainClk(uint8_t SubOscMode, uint8_t MainPLLMode);
void SwitchToMainPLL(uint8_t SubOscMode);
void SwitchToHighCRPLL(uint8_t MainOscMode, uint8_t SubOscMode);
void SwitchToSubClk(uint8_t MainPLLMode, uint8_t MainOscMode);
void SwitchToHighCR(uint8_t MainPLLMode, uint8_t MainOscMode, 
                    uint8_t SubOscMode);
void SwitchToLowCR(uint8_t MainPLLMode, uint8_t MainOscMode, 
                   uint8_t SubOscMode);

#ifdef __cplusplus
}
#endif

#endif

#endif /* _CLOCK_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
