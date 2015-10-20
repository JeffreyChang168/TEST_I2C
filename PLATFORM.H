/* **********************************************************************

         Copyright (c) 2002-2014 Beyond Innovation Technology Co., Ltd

        All rights are reserved. Reproduction in whole or in parts is
    prohibited without the prior written consent of the copyright owner.

   ----------------------------------------------------------------------
    Software License Agreement

    The software supplied herewith by Beyond Innovation Technology Co., Ltd
    (the "Company") is intended and supplied to you, the Company's
    customer, for use solely and exclusively on BiTEK products.

    The software is owned by the Company and/or its supplier, and is
    protected under applicable copyright laws. All rights are reserved.
    Any use in violation of the foregoing restrictions may subject the
    user to criminal sanctions under applicable laws, as well as to
    civil liability for the breach of the terms and conditions of this
    license.

    THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
    TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
    IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

   ----------------------------------------------------------------------

    Module: PLATFORM.H -

    Purpose: Interface of PLATFORM.

    Version: 0.05                                   06:22PM  2014/05/29

    Compiler: Keil 8051 C Compiler v9.52

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 02:26PM  2012/10/01 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ----------------------------------------------------------------------
    v0.02 03:49PM  2013/04/15 Jeffrey Chang
    Reason:
        1. To revise for VID1232_100_128 platform.
        2. To revise for AT070TN94 panel.
    Solution:

   ----------------------------------------------------------------------
    v0.03 11:17AM  2013/05/10 Jeffrey Chang
    Reason:
        1. To fix Y/C separate modes issue.
        2. To reivse for BIT1618C_AT070TN94_CVBS2_AOSD_20130507_01.bin.
        3. To update BIT1618C.H file.
    Solution:

   ----------------------------------------------------------------------
    v0.04 01:40PM  2014/05/20 Jeffrey Chang
    Reason:
        1. To revise for VID1319_101_128 platform.
    Solution:

   ----------------------------------------------------------------------
    v0.05 06:21PM  2014/05/29 Jeffrey Chang
    Reason:
        1. To verify Hardware I2C SLAVE and Software SPI @RIN0/RIN1 pins.
    Solution:

   ********************************************************************** */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_


/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "common.h"


/* ------------------------------------
    Macro Definitions
   ------------------------------------ */
#undef EXTERN

#ifdef _PLATFORM_C_
    #define EXTERN
#else
    #define EXTERN  extern
#endif


#define PLATFORM_VID1124_102_174    1       // VID1124_102_174 + BIT1618C
#define PLATFORM_VID1232_100_128    2       // VID1232_100_128 + BIT1618C
#define PLATFORM_VID1319_101_128    3       // VID1319_101_128 + BIT1618C

// [A] PLATFORM_CFG is defined in UV2 or uvproj file !
//#define PLATFORM_CFG                PLATFORM_VID1124_102_174

#define TFT_T070SWV056H             1       // TRUST T070SWV056H 7" (800 (H) x RGB x 480 (V))
#define TFT_HSD070IDW1A             2       // HannStar HSD070IDW1-A
#define TFT_HSD062IDW1A01           3       // HannStar HSD062IDW1-A01
#define TFT_AT070TN94               4       // InnoLux AT070TN94 7" (800 (H) x RGB x 480 (V))
#define TFT_R4N007124A072           5       // InnoLux AT070TN94+TOC R4N0-0712-4A072
#define TFT_AT070TN92               6       // InnoLux AT070TN92
#define TFT_TM070RDH11              7       // TianMa TM070RDH11
#define TFT_AT043TN25               8       // InnoLux AT043TN25
#define TFT_P070V4A0V0TP            9       // PROMATE P070V4A0V0TP (800 (H) x RGB x 480 (V))

// [B] TFT_CFG is defined in UV2 or uvproj file !
//#define TFT_CFG                   TFT_AT043TN25



#define VERSION_DATE        __DATE2__
#define VERSION_TIME        __TIME__
#define VERSION_CODE        "CI2C v0.10"
#define VERSION_ID          0x0A

#if (TFT_CFG == TFT_T070SWV056H)
                              //"123456789012345678"
    #define VERSION_MESSAGE     "T070SWV056H " VERSION_CODE

#elif (TFT_CFG == TFT_HSD070IDW1A)
                              //"123456789012345678"
    #define VERSION_MESSAGE     "HSD070IDW1A " VERSION_CODE

#elif (TFT_CFG == TFT_HSD062IDW1A01)
                              //"1234567890123456789
    #define VERSION_MESSAGE     "HSD062IDW1A01 " VERSION_CODE

#elif (TFT_CFG == TFT_AT070TN94)
                              //"1234567890123456789
    #define VERSION_MESSAGE     "AT070TN94 " VERSION_CODE

#elif (TFT_CFG == TFT_R4N007124A072)
                              //"12345678901234567890
    #define VERSION_MESSAGE     "R4N007124A072 " VERSION_CODE

#elif (TFT_CFG == TFT_AT070TN92)
                              //"1234567890123456789
    #define VERSION_MESSAGE     "AT070TN92 " VERSION_CODE

#elif (TFT_CFG == TFT_TM070RDH11)
                              //"1234567890123456789
    #define VERSION_MESSAGE     "TM070RDH11 " VERSION_CODE

#elif (TFT_CFG == TFT_AT043TN25)
                              //"1234567890123456789
    #define VERSION_MESSAGE     "AT043TN25 " VERSION_CODE

#elif (TFT_CFG == TFT_P070V4A0V0TP)
                              //"1234567890123456789
    #define VERSION_MESSAGE     "P070V4A0V0TP " VERSION_CODE

#else
    #error PLATFORM.H - Invalid TFT_CFG !
#endif // TFT_CFG


#define USE_EEPROM          0   // Code=EEPROM, Data=EEPROM
#define USE_FLASH           1   // Code=FLASH,  Data=FLASH
#define USE_BOTH            2   // Code=FLASH,  Data=EEPROM

// [C] USE_CFG is defined in UV2 or uvproj file !
//#define USE_CFG             USE_EEPROM

#if (USE_CFG == USE_EEPROM)
    #define PLATFORM_EEPROM     ON
    #define PLATFORM_FLASH      OFF
#elif (USE_CFG == USE_FLASH)
    #define PLATFORM_EEPROM     OFF
    #define PLATFORM_FLASH      ON
#elif (USE_CFG == USE_BOTH)
    #define PLATFORM_EEPROM     ON
    #define PLATFORM_FLASH      OFF
#endif



#define PLATFORM_FONT       ON          // User Font or 2-Layer
#define PLATFORM_GPI        OFF
#define PLATFORM_I2C        ON          // I2C
#define PLATFORM_I2C_INT0   ON          // I2C @INT0
#define PLATFORM_I2C_INT1   OFF         // I2C @INT1
#define PLATFORM_IR_IN      OFF         // IR Decoder
#define PLATFORM_IR_OUT     OFF         // IR Encoder
#define PLATFORM_MENU       ON
#define PLATFORM_OSD        ON
#define PLATFORM_PORT       OFF
#define PLATFORM_RTC        OFF
#define PLATFORM_SARADC     OFF
#define PLATFORM_SPI        OFF         // SPI
#define PLATFORM_SPI_SW     OFF         // SPI
#define PLATFORM_TIMER      ON
#define PLATFORM_UART       OFF
#define PLATFORM_VD         ON
#define PLATFORM_VP         ON

#define DEBUG_GPI           OFF
#define DEBUG_EEPROM        OFF
#define DEBUG_FLASH         OFF
#define DEBUG_FONT          ON
#define DEBUG_I2C           ON
#define DEBUG_IR_IN         OFF
#define DEBUG_IR_OUT        OFF
#define DEBUG_MCU           OFF
#define DEBUG_MCU_INT0      ON
#define DEBUG_MCU_INT1      ON
#define DEBUG_OSD           OFF
#define DEBUG_OSD_2LAYER    OFF
#define DEBUG_SARADC_AK     OFF         // SARADC for ADC Key
#define DEBUG_SARADC_RE     OFF         // SARADC for Rotary Encoder
#define DEBUG_SARADC_TS     OFF         // SARADC for Touch Screen
#define DEBUG_TIMER         OFF
#define DEBUG_UART          OFF
#define DEBUG_VD            OFF
#define DEBUG_VP            OFF


/* ------------------------------------
    Type Definitions
   ------------------------------------ */


/* ------------------------------------
    Variables Definitions/Declarations
   ------------------------------------ */


/* ------------------------------------
    Function Prototypes
   ------------------------------------ */


#endif /* _PLATFORM_H_ */


/* **********************************************************************

    Description:

   ********************************************************************** */

/* %% End Of File %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
