/* **********************************************************************

         Copyright (c) 2002-2013 Beyond Innovation Technology Co., Ltd

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

    Module: VP.C - Video Processor.

    Purpose: Implementation of VP module.

    Version: 0.02                                   10:04AM  2013/04/16

    Compiler: Keil 8051 C Compiler v9.51

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 06:37PM  2013/03/16 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ----------------------------------------------------------------------
    v0.02 10:04AM  2013/04/16 Jeffrey Chang
    Reason:
        1. To support 480i, 576i, 480p and 576p.
    Solution:

   ********************************************************************** */

#define  _VP_C_


/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "bit1618c.h"
#include "bitek.h"
#include "common.h"
#include "osd.h"
#include "platform.h"
#include "timer.h"
#include "vp.h"

#if (PLATFORM_EEPROM)
    #include "eeprom.h"
#endif

#if (PLATFORM_FLASH)
    #include "flash.h"
#endif


/* ------------------------------------
    Macro Definitions
   ------------------------------------ */


/* ------------------------------------
    Type Definitions
   ------------------------------------ */

/* ------------------------------------
    Variables Definitions
   ------------------------------------ */


#if 0
        // VID1232_100_128 + BIT1618C_AT070TN94_Y2_20130416_01.bin
        // InnoLux AT070TN94 7" (800 (H) x RGB x 480 (V))
        // VP
        UB8 CODE abVP_00A_0FE[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
// 0 */                                                   0x12,0x84,0xFC,0xA9,0xA9,0x00, // R00E[5]=0 (BCLK=XCLK)
/* 0 */                                                   0x12,0x84,0xFC,0xA9,0x89,0x00, // R00E[5]=0 (BCLK=XCLK)
/* 1 */ 0x00,0x00,0x00,0x00,0x22,0x00,0x04,0x00,0x0A,0x2D,0x4D,0x30,0xF8,0xC3,0x33,0x05, // R010=000h, R016=004h
/* 2 */ 0x08,0xE9,0x00,0x49,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x20,0x00,0x00,0x00,0x00,
/* 3 */ 0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x01,0x17,0x80,0x17,0x03,0x40,0x00,0x07,0x00,
/* 4 */ 0x00,0x3F,0x3F,0x84,0xE0,0x08,0xAA,0x7A,0x30,0x17,0x3E,0x10,0xA0,0x70,0x30,0x17,
/* 5 */ 0x17,0x10,0x40,0x58,0x88,0xC3,0xC3,0x20,0x80,0x23,0x2D,0x4D,0x30,0xA7,0x27,0x30,
/* 6 */ 0x18,0xF8,0x01,0x20,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x88,0x06,0x00,0x01,0x03,
/* 7 */ 0x6C,0xD8,0x30,0x88,0x6C,0xF1,0x30,0x6C,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0xE0,
/* 8 */ 0x8F,0xA4,0xA0,0x03,0x40,0xC0,0x77,0x2C,0x20,0x03,0x17,0x00,0x00,0x00,0x14,0xFB,
/* 9 */ 0x00,0x00,0xFB,0x00,0x00,0x00,0xA4,0x03,0x00,0x7B,0x7B,0xFF,0xAC,0xF2,0x04,0xB3,
/* A */ 0x04,0x04,0x00,0x40,0x22,0x78,0x80,0x00,0x00,0x00,0x0D,0x0D,0x00,0x88,0xA0,0x40,
/* B */ 0x19,0x8D,0x85,0xEA,0xA4,0x32,0x7F,0x31,0x80,0xFE,0x40,0x32,0x00,0x08,0x00,0x00,
/* C */ 0x4D,0xFB,0x71,0x80,0xC3,0x92,0x9B,0x01,0x9B,0x01,0x92,0x81,0x00,0xD0,0x00,0x00,
/* D */ 0x00,0x10,0x80,0x5E,0x8D,0x04,0x04,0x00,0x00,0x00,0x15,0x00,0x3F,0x8E,0xD0,0x8F,// R0D4
/* E */ 0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x88,0x01,0x00,0x00,0x03,0x00,0x00,0x00,0x00,
// F */ 0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x00,0x80,0x00,0x80,0x00,0x02,0x04,0x00      // R0FB=044h for BL OFF
/* F */ 0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x80,0x00,0x02,0x04,0x00      // R0F8=000h for BL OFF
        };


        #if 0
        UB8 CODE abVP_130_1FF[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*13 */ 0x80,0x80,0x80,0x80,0x80,0x80,0xC6,0x39,0x30,0x80,0x80,0x80,0xC0,0x02,0x40,0x40,
/*14 */ 0x04,0xAA,0x38,0xC2,0xCC,0x2B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*15 */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x06,0x00,0x00,0x02,0x00,0x6B,0x00, // 0x15C and 0x15F
/*16 */ 0x00,0x03,0x80,0x40,0x00,0x00,0x00,0x6B,0x86,0x85,0x09,0x00,0x00,0x00,0x00,0x00, // MSF_SRC(0x164[6:5]) = TOUT (00), MSF_TOUT(0x164[1])=0, 0x161=003h
/*17 */ 0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,
/*18 */ 0x11,0x47,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x00,0x00,0x7C,0x7C,0x00,
/*19 */ 0x00,0x00,0x60,0x00,0xF0,0x7C,0xE7,0x00,0x01,0x16,0x04,0x0E,0x76,0x02,0x0A,0xFC,
/*1A */ 0x00,0x12,0x12,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0xF4,0x0E,0x00,0x00,0x00,0x00,
/*1B */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*1C */ 0x00,0x00,0x44,0x59,0x07,0x06,0x31,0x00,0x00,0x00,0x1B,0xFA,0x50,0xAA,0xEA,0x87,
/*1D */ 0x0B,0x51,0xFF,0xFA,0x01,0x30,0xB1,0x58,0xFD,0x00,0xB7,0x19,0xC7,0x80,0x16,0x30,
/*1E */ 0x04,0x00,0x00,0x00,0x00,0x2F,0x8D,0x46,0x84,0x80,0x13,0x00,0x00,0x00,0x01,0x00,
/*1F */ 0x00,0xF0,0xFD,0x7F,0x04,0x00,0x00,0x00,0x89,0x00,0x00,0x00,0x00,0x4B,0xFF,0x46,
        };
        #endif


        UB8 CODE abVP_130_145[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*13 */ 0x80,0x80,0x80,0x80,0x80,0x80,0xC6,0x39,0x30,0x80,0x80,0x80,0xC0,0x02,0x40,0x40, // R138[6]=0 YUV Gamma OFF, R138[7]=0 RGB Gamma OFF
/*14 */ 0x04,0xAA,0x38,0xC2,0xCC,0x2B
        };


        UB8 CODE abM8051_160_163[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*16 */ 0x00,0x0B,0x80,0x40                                                              // IC default + 24C32, R161=0Bh
        };

        UB8 CODE abPWM_17F_183[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*17 */                                                                            0x10,
/*18 */ 0x11,0x47,0x00,0x00
        };

        UB8 CODE abPLL_198_19E[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*19 */                                         0x01,0x16,0x04,0x0E,0x76,0x02,0x0A
        };



        #if 0
        UB8 CODE abAOSD_146_15F[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*14 */                               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*15 */ 0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x66,0x00,0x00,0x02,0xF0,0x6B,0x04, // 0x15C=002h, R15E=06Bh
        };
        
        UB8 CODE abDMA_164_170[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*16 */                     0x1C,0x00,0x00,0x00,0x00,0x05,0x01,0x00,0x03,0x00,0x6B,0xF9,
/*17 */ 0x20
        };

        UB8 CODE abIR_174_17E[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*17 */                     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        };

        UB8 CODE abSPI_185_189[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*18 */                          0x00,0x00,0x00,0x00,0x00
        };

        UB8 CODE abGPI_18C_194[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*18 */                                                             0x00,0x7C,0x7C,0x00,
/*19 */ 0x00,0x00,0x60,0x00,0xF0
        };

        UB8 CODE abSARADC_19F_1A9[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*19 */                                                                            0xFE,
/*1A */ 0x00,0x09,0x09,0x20,0x00,0x00,0x00,0x00,0x00,0x00
        };

        UB8 CODE abI2C_1AE_1B5[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*1A */                                                                       0x00,0x00,
/*1B */ 0x00,0x00,0x00,0x00,0x00,0x00
        };

        // For CPU Panel !
        UB8 CODE abCPU_1B9_1BD[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*1B */                                              0x00,0x00,0x00,0x00,0x00,
        };
        #endif
#endif


        // VID1232_100_128 + BIT1618C_AT070TN94_CVBS2_AOSD_20130507_01.bin
        // InnoLux AT070TN94 7" (800 (H) x RGB x 480 (V))
        // VP
        UB8 CODE abVP_00A_0FE[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
// 0 */                                                   0x12,0x84,0xFC,0xA9,0xA9,0x00, // R00E[5]=0 (BCLK=XCLK)
/* 0 */                                                   0x12,0x84,0xFC,0xA9,0x89,0x00, // R00E[5]=0 (BCLK=XCLK)
/* 1 */ 0x00,0x00,0x00,0x00,0x22,0x00,0x04,0x00,0x0A,0x2D,0x4D,0x30,0xF8,0xC3,0x33,0x05, // R010=000h, R016=004h
/* 2 */ 0x08,0xE9,0x00,0x49,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x20,0x00,0x00,0x00,0x00,
/* 3 */ 0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x01,0x17,0x80,0x17,0x03,0x40,0x00,0x07,0x00,
/* 4 */ 0x00,0x3F,0x3F,0x84,0xE0,0x08,0xAA,0x7A,0x30,0x17,0x3E,0x10,0xA0,0x70,0x30,0x17,
/* 5 */ 0x17,0x10,0x40,0x58,0x88,0xC3,0xC3,0x20,0x80,0x23,0x2D,0x4D,0x30,0xA7,0x27,0x30,
/* 6 */ 0x18,0xF8,0x01,0x20,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x88,0x06,0x00,0x01,0x03,
/* 7 */ 0x6C,0xD8,0x30,0x88,0x6C,0xF1,0x30,0x6C,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0xE0,
/* 8 */ 0x8F,0xA4,0xA0,0x03,0x40,0xC0,0x77,0x2C,0x20,0x03,0x17,0x00,0x00,0x00,0x14,0xFB,
/* 9 */ 0x00,0x00,0xFB,0x00,0x00,0x00,0xA4,0x03,0x00,0x7B,0x7B,0xFF,0xAC,0xF2,0x04,0xB3,
/* A */ 0x04,0x04,0x00,0x40,0x22,0x78,0x80,0x00,0x00,0x00,0x0D,0x0D,0x00,0x88,0xA0,0x40,
/* B */ 0x19,0x8D,0x85,0xEA,0xA4,0x32,0x7F,0x31,0x80,0xFE,0x40,0x32,0x00,0x08,0x00,0x00,
/* C */ 0x4D,0xFB,0x71,0x80,0xC3,0x92,0x9B,0x01,0x9B,0x01,0x92,0x81,0x00,0xD0,0x00,0x00,
/* D */ 0x00,0x10,0x80,0x5E,0x8D,0x04,0x04,0x00,0x00,0x00,0x15,0x00,0x3F,0x8E,0xD0,0x8F,// R0D4
/* E */ 0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x88,0x01,0x00,0x00,0x03,0x00,0x00,0x00,0x00,
// F */ 0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x00,0x80,0x00,0x80,0x00,0x02,0x04,0x00      // R0F8=000h for BL OFF
/* F */ 0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x80,0x00,0x02,0x04,0x00      // R0F8=000h for BL OFF
        };


        #if 0
        UB8 CODE abVP_130_1FF[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*13 */ 0x80,0x80,0x80,0x80,0x80,0x80,0xC6,0x39,0x30,0x80,0x80,0x80,0xC0,0x02,0x40,0x40,
/*14 */ 0x04,0xAA,0x38,0xC2,0xCC,0x2B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*15 */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x06,0x00,0x00,0x02,0x00,0x6B,0x00, // 0x15C and 0x15F
/*16 */ 0x00,0x03,0x80,0x40,0x00,0x00,0x00,0x6B,0x86,0x85,0x09,0x00,0x00,0x00,0x00,0x00, // MSF_SRC(0x164[6:5]) = TOUT (00), MSF_TOUT(0x164[1])=0, 0x161=003h
/*17 */ 0x00,0xFF,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,
/*18 */ 0x11,0x47,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x00,0x00,0x7C,0x7C,0x00,
/*19 */ 0x00,0x00,0x60,0x00,0xF0,0x3C,0xF3,0x00,0x01,0x16,0x04,0x0E,0x76,0x02,0x0A,0xFC,
/*1A */ 0x00,0x12,0x12,0x20,0x00,0x00,0x00,0x00,0x02,0x00,0x5B,0x06,0x00,0x00,0x00,0x00,
/*1B */ 0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*1C */ 0x00,0x00,0x44,0x59,0x06,0x07,0x31,0x00,0x00,0x00,0x1E,0xFA,0x50,0xEA,0x10,0x88,
/*1D */ 0x01,0x8B,0x01,0x8B,0x00,0x01,0x8E,0x5C,0xC8,0x00,0xBA,0x19,0x3E,0xB0,0xD7,0x37,
/*1E */ 0x04,0x00,0x00,0x00,0x00,0x0F,0x8D,0x43,0x99,0x7A,0x03,0x00,0x00,0x00,0x01,0x00,
/*1F */ 0x00,0xF0,0xFD,0x7F,0x04,0x00,0x00,0x00,0x89,0x00,0x00,0x00,0x00,0x4B,0xFF,0x46,
        };
        #endif


        UB8 CODE abVP_130_145[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*13 */ 0x80,0x80,0x80,0x80,0x80,0x80,0xC6,0x39,0x30,0x80,0x80,0x80,0xC0,0x02,0x40,0x40, // R138[6]=0 YUV Gamma OFF, R138[7]=0 RGB Gamma OFF
/*14 */ 0x04,0xAA,0x38,0xC2,0xCC,0x2B
        };


        UB8 CODE abM8051_160_163[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*16 */ 0x00,0x0B,0x80,0x40                                                              // IC default + 24C32, R161=0Bh
        };

        UB8 CODE abPWM_17F_183[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*17 */                                                                            0x10,
/*18 */ 0x11,0x47,0x00,0x00
        };

        UB8 CODE abPLL_198_19E[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*19 */                                         0x01,0x16,0x04,0x0E,0x76,0x02,0x0A
        };



        #if 0
        UB8 CODE abAOSD_146_15F[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*14 */                               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*15 */ 0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x66,0x00,0x00,0x02,0xF0,0x6B,0x04, // 0x15C=002h, R15E=06Bh
        };

        UB8 CODE abDMA_164_170[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*16 */                     0x1C,0x00,0x00,0x00,0x00,0x05,0x01,0x00,0x03,0x00,0x6B,0xF9,
/*17 */ 0x20
        };

        UB8 CODE abIR_174_17E[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*17 */                     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        };

        UB8 CODE abSPI_185_189[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*18 */                          0x00,0x00,0x00,0x00,0x00
        };

        UB8 CODE abGPI_18C_194[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*18 */                                                             0x00,0x7C,0x7C,0x00,
/*19 */ 0x00,0x00,0x60,0x00,0xF0
        };

        UB8 CODE abSARADC_19F_1A9[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*19 */                                                                            0xFE,
/*1A */ 0x00,0x09,0x09,0x20,0x00,0x00,0x00,0x00,0x00,0x00
        };

        UB8 CODE abI2C_1AE_1B5[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*1A */                                                                       0x00,0x00,
/*1B */ 0x00,0x00,0x00,0x00,0x00,0x00
        };

        // For CPU Panel !
        UB8 CODE abCPU_1B9_1BD[] = {
        // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
/*1B */                                              0x00,0x00,0x00,0x00,0x00,
        };
        #endif

/* ------------------------------------
    Function Prototypes
   ------------------------------------ */


/* -------------------------------------------------------------------
    Name: VP_Init -
    Purpose: To initiate VP module.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void VP_Init (void)
{
    // PLL
    BITEK_TxBurst(BITEK_198_PLL_DM_M0, sizeof(abPLL_198_19E), abPLL_198_19E);

    // VP Default Values !
    BITEK_TxBurst(BITEK_00A_CLK_ATTR0, sizeof(abVP_00A_0FE), abVP_00A_0FE);

    // VP Default Values !
    BITEK_TxBurst(BITEK_130_BRIGHTNESS_R, sizeof(abVP_130_145), abVP_130_145);

    // M8051
    BITEK_TxBurst(BITEK_160_M8051_DUMMY, sizeof(abM8051_160_163), abM8051_160_163);

    // PWM
    BITEK_TxBurst(BITEK_17F180_PWMx_FREQ, sizeof(abPWM_17F_183), abPWM_17F_183);

} /* VP_Init */



#if (VP_SET_BRIGHTNESS)
/* -------------------------------------------------------------------
    Name: VP_SetBrightness -
    Purpose: This function sets the VP Brightness.
    Passed:
    Returns: None.
    Notes: [1]63
   ------------------------------------------------------------------- */
void VP_SetBrightness (UB8 bBrightness)
{
    BITEK_TxByte(BITEK_13A_BRIGHTNESS, BRIGHTNESS_OFFSET + BRIGHTNESS_SLOPE * bBrightness);
} // VP_SetBrightness
#endif


#if (VP_SET_COLORKILLER)
/* -------------------------------------------------------------------
    Name: VP_SetColorKiller -
    Purpose: This function sets the COLOR KILLER.
    Passed: 0x00..0x7F
    Returns: None.
    Notes: [1]63
   ------------------------------------------------------------------- */
void VP_SetColorKiller (UB8 bSaturation)
{
    UB8 bColorKiller;


    bColorKiller = BITEK_RxByte(BITEK_140_KILL_COLOR_ATTR);

    if (bSaturation)
        // Color Killer OFF (0x140[1] = 0)
        bColorKiller &= ~BITEK_MASK_KILL_COLOR;
    else
        // Color Killer ON  (0x140[1] = 1)
        bColorKiller |= BITEK_MASK_KILL_COLOR;

    BITEK_TxByte(BITEK_140_KILL_COLOR_ATTR, bColorKiller);
} // VP_SetColorKiller
#endif


#if (VP_SET_CONTRAST)
/* -------------------------------------------------------------------
    Name: VP_SetContrast -
    Purpose: This function sets the VP Contrast.
    Passed:
        0x00 = 0.0
        0x80 = 1.0
        0xFF = 1.9922
    Returns: None.
    Notes: [1]63
   ------------------------------------------------------------------- */
void VP_SetContrast (UB8 bContrast)
{
    BITEK_TxByte(BITEK_13B_CONTRAST, CONTRAST_OFFSET + CONTRAST_SLOPE * bContrast);
} // VP_SetContrast
#endif


#if (VP_SET_SATURATION)
/* -------------------------------------------------------------------
    Name: VP_SetSaturation -
    Purpose: This function sets the VP Saturation.
    Passed: 0x00..0x7F
    Returns: None.
    Notes: [1]63
   ------------------------------------------------------------------- */
void VP_SetSaturation (UB8 bSaturation)
{
    VP_SetColorKiller(bSaturation);

    bSaturation = SATURATION_OFFSET + SATURATION_SLOPE * bSaturation;
    BITEK_TxByte(BITEK_13E_SAT_U, bSaturation);
    BITEK_TxByte(BITEK_13F_SAT_V, bSaturation);
} // VP_SetSaturation
#endif


#if (VP_SET_SOURCE)
/* -------------------------------------------------------------------
    Name: VP_SetSource -
    Purpose:
    Passed:
    Returns: None.
    Notes: [1]86
   ------------------------------------------------------------------- */
void VP_SetSource (UB8  bSource)
{
    switch (bSource)
    {
        case SOURCE_CVBS2:
/*                                  Y2Pb11Pr12  CVBS2
                                    576p
|    Clock     |       0x00C      |    0xEC   |    0xFC   |            |
|    Panel     |       0x01C      |    0xFF   |    0xF8   |            |
|    Panel     |       0x01D      |    0xC6   |    0xC3   |            |
|Input Windows |       0x046      |    0xA2   |    0xAA   |            |
|Input Windows |       0x047      |    0x72   |    0x7A   |            |
|Input Windows |       0x049      |    0x15   |    0x17   |            |
|Input Windows |       0x04A      |    0x62   |    0x3E   |            |
|Input Windows |       0x04B      |    0x20   |    0x10   |            |
|Input Windows |       0x04C      |    0x9B   |    0xA0   |            |
|Input Windows |       0x04D      |    0x6B   |    0x70   |            |
|Input Windows |       0x050      |    0x09   |    0x17   |            |
|Input Windows |       0x051      |    0x20   |    0x10   |            |
|  V Scaling   |       0x07E      |    0x69   |    0x09   |            |
|  V Scaling   |       0x080      |    0x21   |    0x8F   |            |
|  V Scaling   |       0x082      |    0xB0   |    0xA0   |            |
|  V Scaling   |       0x086      |    0xF0   |    0x77   |            |
|  V Scaling   |       0x087      |    0x85   |    0x2C   |            |
|  V Scaling   |       0x088      |    0x00   |    0x20   |            |
|    Timing    |       0x08A      |    0x25   |    0x17   |            |
|    Timing    |       0x08E      |    0x22   |    0x14   |            |
|Video Decoder |       0x09C      |    0x94   |    0xAC   |            |
|Video Decoder |       0x09D      |    0xDC   |    0xF2   |            |
|Video Decoder |       0x09E      |    0x40   |    0x04   |            |
|Video Decoder |       0x09F      |    0x01   |    0xB3   |            |
|Video Decoder |       0x0A4      |    0x00   |    0x22   |            |
|Video Decoder |       0x0A5      |    0x7A   |    0x78   |            |
|Video Decoder |       0x0B3      |    0xC0   |    0xEA   |            |
|Video Decoder |       0x0B4      |    0xC0   |    0xA4   |            |
|Video Decoder |       0x0C2      |    0xC4   |    0x71   |            |
|Video Decoder |       0x0C4      |    0xF3   |    0xC3   |            |
|Video Decoder |       0x0C7      |    0x81   |    0x01   |            |
|Video Decoder |       0x0C9      |    0x81   |    0x01   |            |
|Video Decoder |       0x0DE      |    0x55   |    0xD0   |            |
|Video Decoder |       0x0DF      |    0xFF   |    0x8F   |            |
|Video Decoder |       0x0E0      |    0x83   |    0x03   |            |
|    Image     |       0x141      |    0x2A   |    0xAA   |            |
|    Image     |       0x143      |    0x42   |    0xC2   |            |
|    Image     |       0x144      |    0x4C   |    0xCC   |            |
*/
            BITEK_TxByte(0x00C, 0xFC);
            BITEK_TxByte(0x01C, 0xF8);
            BITEK_TxByte(0x01D, 0xC3);
            BITEK_TxByte(0x046, 0xAA);
            BITEK_TxByte(0x047, 0x7A);
            BITEK_TxByte(0x049, 0x17);
            BITEK_TxByte(0x04A, 0x3E);
            BITEK_TxByte(0x04B, 0x10);
            BITEK_TxByte(0x04C, 0xA0);
            BITEK_TxByte(0x04D, 0x70);
            BITEK_TxByte(0x050, 0x17);
            BITEK_TxByte(0x051, 0x10);
            BITEK_TxByte(0x07E, 0x09);
            BITEK_TxByte(0x080, 0x8F);
            BITEK_TxByte(0x082, 0xA0);
            BITEK_TxByte(0x086, 0x77);
            BITEK_TxByte(0x087, 0x2C);
            BITEK_TxByte(0x088, 0x20);
            BITEK_TxByte(0x08A, 0x17);
            BITEK_TxByte(0x08E, 0x14);
            BITEK_TxByte(0x09C, 0xAC);
            BITEK_TxByte(0x09D, 0xF2);
            BITEK_TxByte(0x09E, 0x04);
            BITEK_TxByte(0x09F, 0xB3);
            BITEK_TxByte(0x0A4, 0x22);
            BITEK_TxByte(0x0A5, 0x78);
            BITEK_TxByte(0x0B3, 0xEA);
            BITEK_TxByte(0x0B4, 0xA4);
            BITEK_TxByte(0x0C2, 0x71);
            BITEK_TxByte(0x0C4, 0xC3);
            BITEK_TxByte(0x0C7, 0x01);
            BITEK_TxByte(0x0C9, 0x01);
            BITEK_TxByte(0x0DE, 0xD0);
            BITEK_TxByte(0x0DF, 0x8F);
            BITEK_TxByte(0x0E0, 0x03);
            BITEK_TxByte(0x141, 0xAA);
            BITEK_TxByte(0x143, 0xC2);
            BITEK_TxByte(0x144, 0xCC);
            break;

        case SOURCE_Y2C12:
/*                                  CVBS2       Y2C12
|Input Windows |       0x046      |    0xAA   |    0xA6   |            |
|Input Windows |       0x047      |    0x7A   |    0x76   |            |
|Input Windows |       0x049      |    0x17   |    0x15   |            |
|Input Windows |       0x04A      |    0x3E   |    0x3C   |            |
|Input Windows |       0x04C      |    0xA0   |    0x9C   |            |
|Input Windows |       0x04D      |    0x70   |    0x6C   |            |
|Input Windows |       0x04F      |    0x17   |    0x15   |            |
|    Timing    |       0x08A      |    0x17   |    0x15   |            |
|    Timing    |       0x08E      |    0x14   |    0x12   |            |
|Video Decoder |       0x0A5      |    0x78   |    0x7A   |            |
|Video Decoder |       0x0C4      |    0xC3   |    0xF3   |            |
|Video Decoder |       0x0C7      |    0x01   |    0x81   |            |
|Video Decoder |       0x0C9      |    0x01   |    0x81   |            |
|Video Decoder |       0x0DF      |    0x8F   |    0x9F   |            |
*/
            BITEK_TxByte(0x046, 0xA6);
            BITEK_TxByte(0x047, 0x76);
            BITEK_TxByte(0x049, 0x15);
            BITEK_TxByte(0x04A, 0x3C);
            BITEK_TxByte(0x04C, 0x9C);
            BITEK_TxByte(0x04D, 0x6C);
            BITEK_TxByte(0x04F, 0x15);
            BITEK_TxByte(0x08A, 0x15);
            BITEK_TxByte(0x08E, 0x12);
            BITEK_TxByte(0x0A5, 0x7A);
            BITEK_TxByte(0x0C4, 0xF3);
            BITEK_TxByte(0x0C7, 0x81);
            BITEK_TxByte(0x0C9, 0x81);
            BITEK_TxByte(0x0DF, 0x9F);
            break;

        case SOURCE_Y2CB11CR12:
/*                                  CVBS2       Y2Cb11Cr12 (576i)
|Input Windows |       0x046      |    0xAA   |    0x94   |            |
|Input Windows |       0x047      |    0x7A   |    0x64   |            |
|Input Windows |       0x049      |    0x17   |    0x15   |            |
|Input Windows |       0x04A      |    0x3E   |    0x3C   |            |
|Input Windows |       0x04C      |    0xA0   |    0x8A   |            |
|Input Windows |       0x04D      |    0x70   |    0x5A   |            |
|Input Windows |       0x04F      |    0x17   |    0x15   |            |
|    Timing    |       0x08A      |    0x17   |    0x15   |            |
|    Timing    |       0x08E      |    0x14   |    0x12   |            |
|Video Decoder |       0x09C      |    0xAC   |    0x94   |            |
|Video Decoder |       0x09D      |    0xF2   |    0xDC   |            |
|Video Decoder |       0x09E      |    0x04   |    0x40   |            |
|Video Decoder |       0x09F      |    0xB3   |    0x01   |            |
|Video Decoder |       0x0A5      |    0x78   |    0x79   |            |
|Video Decoder |       0x0B3      |    0xEA   |    0xFF   |            |
|Video Decoder |       0x0B4      |    0xA4   |    0xFF   |            |
|Video Decoder |       0x0C4      |    0xC3   |    0xF3   |            |
|Video Decoder |       0x0C7      |    0x01   |    0x81   |            |
|Video Decoder |       0x0C9      |    0x01   |    0x81   |            |
|Video Decoder |       0x0DE      |    0xD0   |    0x55   |            |
|Video Decoder |       0x0DF      |    0x8F   |    0xBF   |            |
|Video Decoder |       0x0E0      |    0x03   |    0x83   |            |
|    Image     |       0x141      |    0xAA   |    0x2A   |            |
|    Image     |       0x143      |    0xC2   |    0x42   |            |

                                    Y2C12       Y2Cb11Cr12 (576i)
|Input Windows |       0x046      |    0xA6   |    0x94   |            |
|Input Windows |       0x047      |    0x76   |    0x64   |            |
|Input Windows |       0x04C      |    0x9C   |    0x8A   |            |
|Input Windows |       0x04D      |    0x6C   |    0x5A   |            |
|Video Decoder |       0x09C      |    0xAC   |    0x94   |            |
|Video Decoder |       0x09D      |    0xF2   |    0xDC   |            |
|Video Decoder |       0x09E      |    0x04   |    0x40   |            |
|Video Decoder |       0x09F      |    0xB3   |    0x01   |            |
|Video Decoder |       0x0A5      |    0x7A   |    0x79   |            |
|Video Decoder |       0x0B3      |    0xEA   |    0xFF   |            |
|Video Decoder |       0x0B4      |    0xA4   |    0xFF   |            |
|Video Decoder |       0x0DE      |    0xD0   |    0x55   |            |
|Video Decoder |       0x0DF      |    0x9F   |    0xBF   |            |
|Video Decoder |       0x0E0      |    0x03   |    0x83   |            |
|    Image     |       0x141      |    0xAA   |    0x2A   |            |
|    Image     |       0x143      |    0xC2   |    0x42   |            |
*/
            BITEK_TxByte(0x046, 0x94);
            BITEK_TxByte(0x047, 0x64);
            BITEK_TxByte(0x049, 0x15);
            BITEK_TxByte(0x04A, 0x3C);
            BITEK_TxByte(0x04C, 0x8A);
            BITEK_TxByte(0x04D, 0x5A);
            BITEK_TxByte(0x04F, 0x15);
            BITEK_TxByte(0x08A, 0x15);
            BITEK_TxByte(0x08E, 0x12);
            BITEK_TxByte(0x09C, 0x94);
            BITEK_TxByte(0x09D, 0xDC);
            BITEK_TxByte(0x09E, 0x40);
            BITEK_TxByte(0x09F, 0x01);
            BITEK_TxByte(0x0A5, 0x79);
            BITEK_TxByte(0x0B3, 0xFF);
            BITEK_TxByte(0x0B4, 0xFF);
            BITEK_TxByte(0x0C4, 0xF3);
            BITEK_TxByte(0x0C7, 0x81);
            BITEK_TxByte(0x0C9, 0x81);
            BITEK_TxByte(0x0DE, 0x55);
            BITEK_TxByte(0x0DF, 0xBF);
            BITEK_TxByte(0x0E0, 0x83);
            BITEK_TxByte(0x141, 0x2A);
            BITEK_TxByte(0x143, 0x42);
            break;

        case SOURCE_Y2PB11PR12:
/*                                  CVBS2       Y2Pb11Pr12 (576p)
|    Clock     |       0x00C      |    0xFC   |    0xEC   |            |
|    Panel     |       0x01C      |    0xF8   |    0xFF   |            |
|    Panel     |       0x01D      |    0xC3   |    0xC6   |            |
|Input Windows |       0x046      |    0xAA   |    0xA2   |            |
|Input Windows |       0x047      |    0x7A   |    0x72   |            |
|Input Windows |       0x049      |    0x17   |    0x15   |            |
|Input Windows |       0x04A      |    0x3E   |    0x62   |            |
|Input Windows |       0x04B      |    0x10   |    0x20   |            |
|Input Windows |       0x04C      |    0xA0   |    0x9B   |            |
|Input Windows |       0x04D      |    0x70   |    0x6B   |            |
|Input Windows |       0x050      |    0x17   |    0x09   |            |
|Input Windows |       0x051      |    0x10   |    0x20   |            |
|  V Scaling   |       0x07E      |    0x09   |    0x69   |            |
|  V Scaling   |       0x080      |    0x8F   |    0x21   |            |
|  V Scaling   |       0x082      |    0xA0   |    0xB0   |            |
|  V Scaling   |       0x086      |    0x77   |    0xF0   |            |
|  V Scaling   |       0x087      |    0x2C   |    0x85   |            |
|  V Scaling   |       0x088      |    0x20   |    0x00   |            |
|    Timing    |       0x08A      |    0x17   |    0x25   |            |
|    Timing    |       0x08E      |    0x14   |    0x22   |            |
|Video Decoder |       0x09C      |    0xAC   |    0x94   |            |
|Video Decoder |       0x09D      |    0xF2   |    0xDC   |            |
|Video Decoder |       0x09E      |    0x04   |    0x40   |            |
|Video Decoder |       0x09F      |    0xB3   |    0x01   |            |
|Video Decoder |       0x0A4      |    0x22   |    0x00   |            |
|Video Decoder |       0x0A5      |    0x78   |    0x7A   |            |
|Video Decoder |       0x0B3      |    0xEA   |    0xC0   |            |
|Video Decoder |       0x0B4      |    0xA4   |    0xC0   |            |
|Video Decoder |       0x0C2      |    0x71   |    0xC4   |            |
|Video Decoder |       0x0C4      |    0xC3   |    0xF3   |            |
|Video Decoder |       0x0C7      |    0x01   |    0x81   |            |
|Video Decoder |       0x0C9      |    0x01   |    0x81   |            |
|Video Decoder |       0x0DE      |    0xD0   |    0x55   |            |
|Video Decoder |       0x0DF      |    0x8F   |    0xFF   |            |
|Video Decoder |       0x0E0      |    0x03   |    0x83   |            |
|    Image     |       0x141      |    0xAA   |    0x2A   |            |
|    Image     |       0x143      |    0xC2   |    0x42   |            |
|    Image     |       0x144      |    0xCC   |    0x4C   |            |

                                    Y2Cb11Cr12  Y2Pb11Pr12
                                    576i        576p
|    Clock     |       0x00C      |    0xFC   |    0xEC   |            |
|    Panel     |       0x01C      |    0xF8   |    0xFF   |            |
|    Panel     |       0x01D      |    0xC3   |    0xC6   |            |
|Input Windows |       0x046      |    0x94   |    0xA2   |            |
|Input Windows |       0x047      |    0x64   |    0x72   |            |
|Input Windows |       0x04A      |    0x3C   |    0x62   |            |
|Input Windows |       0x04B      |    0x10   |    0x20   |            |
|Input Windows |       0x04C      |    0x8A   |    0x9B   |            |
|Input Windows |       0x04D      |    0x5A   |    0x6B   |            |
|Input Windows |       0x04F      |    0x15   |    0x17   |            |
|Input Windows |       0x050      |    0x17   |    0x09   |            |
|Input Windows |       0x051      |    0x10   |    0x20   |            |
|  V Scaling   |       0x07E      |    0x09   |    0x69   |            |
|  V Scaling   |       0x080      |    0x8F   |    0x21   |            |
|  V Scaling   |       0x082      |    0xA0   |    0xB0   |            |
|  V Scaling   |       0x086      |    0x77   |    0xF0   |            |
|  V Scaling   |       0x087      |    0x2C   |    0x85   |            |
|  V Scaling   |       0x088      |    0x20   |    0x00   |            |
|    Timing    |       0x08A      |    0x15   |    0x25   |            |
|    Timing    |       0x08E      |    0x12   |    0x22   |            |
|Video Decoder |       0x0A4      |    0x22   |    0x00   |            |
|Video Decoder |       0x0A5      |    0x79   |    0x7A   |            |
|Video Decoder |       0x0B3      |    0xFF   |    0xC0   |            |
|Video Decoder |       0x0B4      |    0xFF   |    0xC0   |            |
|Video Decoder |       0x0C2      |    0x71   |    0xC4   |            |
|Video Decoder |       0x0DF      |    0xBF   |    0xFF   |            |
|    Image     |       0x144      |    0xCC   |    0x4C   |            |
*/
            BITEK_TxByte(0x00C, 0xEC);
            BITEK_TxByte(0x01C, 0xFF);
            BITEK_TxByte(0x01D, 0xC6);
            BITEK_TxByte(0x046, 0xA2);
            BITEK_TxByte(0x047, 0x72);
            BITEK_TxByte(0x049, 0x15);
            BITEK_TxByte(0x04A, 0x62);
            BITEK_TxByte(0x04B, 0x20);
            BITEK_TxByte(0x04C, 0x9B);
            BITEK_TxByte(0x04D, 0x6B);
            BITEK_TxByte(0x050, 0x09);
            BITEK_TxByte(0x051, 0x20);
            BITEK_TxByte(0x07E, 0x69);
            BITEK_TxByte(0x080, 0x21);
            BITEK_TxByte(0x082, 0xB0);
            BITEK_TxByte(0x086, 0xF0);
            BITEK_TxByte(0x087, 0x85);
            BITEK_TxByte(0x088, 0x00);
            BITEK_TxByte(0x08A, 0x25);
            BITEK_TxByte(0x08E, 0x22);
            BITEK_TxByte(0x09C, 0x94);
            BITEK_TxByte(0x09D, 0xDC);
            BITEK_TxByte(0x09E, 0x40);
            BITEK_TxByte(0x09F, 0x01);
            BITEK_TxByte(0x0A4, 0x00);
            BITEK_TxByte(0x0A5, 0x7A);
            BITEK_TxByte(0x0B3, 0xC0);
            BITEK_TxByte(0x0B4, 0xC0);
            BITEK_TxByte(0x0C2, 0xC4);
            BITEK_TxByte(0x0C4, 0xF3);
            BITEK_TxByte(0x0C7, 0x81);
            BITEK_TxByte(0x0C9, 0x81);
            BITEK_TxByte(0x0DE, 0x55);
            BITEK_TxByte(0x0DF, 0xFF);
            BITEK_TxByte(0x0E0, 0x83);
            BITEK_TxByte(0x141, 0x2A);
            BITEK_TxByte(0x143, 0x42);
            BITEK_TxByte(0x144, 0x4C);

            BITEK_TxByte(0x04F, 0x17);
            break;


        #if 0
        case SOURCE_CVBS11:
            BITEK_TxByte(BITEK_0EE_AFE_ATTR,   b0EE | 0x04);
            BITEK_TxByte(BITEK_0D4_AAGC_ATTR3, b0D4       );
            break;

        case SOURCE_CVBS12:
            BITEK_TxByte(BITEK_0EE_AFE_ATTR,   b0EE | 0x44);
            BITEK_TxByte(BITEK_0D4_AAGC_ATTR3, b0D4       );
            break;
        #endif
    } // switch
} // VP_SetSource
#endif


#if (VP_STD_DETECT)
/* -------------------------------------------------------------------
    Name: VP_STD_Detect -
    Purpose: To detect/show color standard.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void VP_STD_Detect (void)
{
    #define MSG_COLOR_STD_SIZE      10

    UB8 CODE * CODE apbCOLOR_STD[] =
    {
       //1234567890
        "       PAL",   // 0
        "     PAL-N",   // 1
        "     SECAM",   // 2
        "     PAL-M",   // 3
        "NTSC443 50",   // 4
        "    NTSC-M",   // 5
        "NTSC443 60",   // 6
        "BlackWhite",   // 7
        "    PAL-60",   // 8
        " No Signal",   // 9
        "      480i",   // 10
        "      576i",   // 11
        "      480p",   // 12
        "      576p",   // 13
    };


    UB8 bSTD;


    // H-LOCK (0x1E5[1]) = 1
    bSTD = BITEK_RxByte(BITEK_1E5_VD_INFO_O) & 0x0E;
    if (bSTD == 0x0E)
    {
        // VD STD mode
        bSTD = BITEK_RxByte(BITEK_1E6_STD_INFO_O) & 0x0F;
      //OSD_ShowHex(VP_SHOW_VD_STATUS + 6, bSTD);          // ROW5: Debug Only


        if ((tsGD.bSource == SOURCE_Y2CB11CR12) ||
            (tsGD.bSource == SOURCE_Y2PB11PR12)     )
        {
/*
                                    Y2Cb11Cr12  Y2Cb11Cr12
                                        480i        576i
|Video Decoder |       0x09D      |    0xED   |    0xDC   |            |
|Video Decoder |       0x09F      |    0xB3   |    0x01   |            |

                                    Y2Pb11Pr12  Y2Pb11Pr12
                                        480p        576p
|Video Decoder |       0x09D      |    0xED   |    0xDC   |            |
|Video Decoder |       0x09F      |    0xB3   |    0x01   |            |
| AOSD Control |       0x15D      |    0x68   |    0x00   |            |
| AOSD Control |       0x15E      |    0x03   |    0x6B   |            |
*/

            if (bSTD & BITEK_MASK_FIDT_O)
            {
                // 60Hz
                BITEK_TxByte(0x09D, 0xED);
                BITEK_TxByte(0x09F, 0xB3);

                if (tsGD.bSource == SOURCE_Y2CB11CR12)
                    bSTD = 10;
                else
                    bSTD = 12;
            }
            else
            {
                // 50Hz
                BITEK_TxByte(0x09D, 0xDC);
                BITEK_TxByte(0x09F, 0x01);

                if (tsGD.bSource == SOURCE_Y2CB11CR12)
                    bSTD = 11;
                else
                    bSTD = 13;
            }
        }
        else
        {
            switch (bSTD)
            {
                case BITEK_MASK_STD_PAL_M:              // 3 + 8
                case BITEK_MASK_STD_NTSC_M:             // 5 + 8
                case BITEK_MASK_STD_NTSC443_60:         // 6 + 8
                case BITEK_MASK_STD_BLACK_WHITE60:      // 7 + 8
                    bSTD -= 8;

                case BITEK_MASK_STD_PAL:                // 0
                case BITEK_MASK_STD_PAL_N:              // 1
                case BITEK_MASK_STD_SECAM:              // 2
                case BITEK_MASK_STD_NTSC443_50:         // 4
                case BITEK_MASK_STD_BLACK_WHITE50:      // 7
                case BITEK_MASK_STD_PAL_60:             // 8
                default:
                    break;
            } // switch bSTD
        }
    }
    else
        bSTD = 9;

    // Show Color Standard string
    BITEK_TxBurst(VP_SHOW_COLOR_STD, MSG_COLOR_STD_SIZE, apbCOLOR_STD[ bSTD ]);
    BITEK_TxByte(BITEK_10D_OSD0_ATTR2, OSD0_ON);

    bOSD_Cnt = OSD_AUTO_START;
} /* VP_STD_Detect */
#endif


#ifdef NOT_JUNK

#if (VP_SET_SHARPNESS)
/* -------------------------------------------------------------------
    Name: VP_SetSharpness -
    Purpose: To set Sharpness of VP.
    Passed:
    Returns:
    Notes: [1]67
   ------------------------------------------------------------------- */
void VP_SetSharpness (UB8 bSharpness)
{
    bSharpness = SHARPNESS_OFFSET + bSharpness;

    VP_TxByte(VP_MAD, VP_08A_SHARPNESS, bSharpness);
} /* VP_SetSharpness */
#endif



#if (VP_SET_TCON_MODE)
/* -------------------------------------------------------------------
    Name: VP_SetTCON_Mode -
    Purpose: To set the video processor TCON Mode.
    Passed:
        UB8 bTCON_Mode  TCON_MODE_RU    or
                        TCON_MODE_LU    or
                        TCON_MODE_LD    or
                        TCON_MODE_RD
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void VP_SetTCON_Mode (UB8 bTCON_Mode)
{
    #if (PROJECT == DMO012006200)    ||  \
        (PROJECT == DMO012007900)

    switch (bTCON_Mode)
    {
        case TCON_MODE_RU:
            VP_TxByte(VP_MAD, VP_00A_GPO_REG, 0x20);
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x44);
            break;

        case TCON_MODE_LU:
            VP_TxByte(VP_MAD, VP_00A_GPO_REG, 0x00);
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x46);
            break;

        case TCON_MODE_LD:
            VP_TxByte(VP_MAD, VP_00A_GPO_REG, 0x10);
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x47);
            break;

        case TCON_MODE_RD:
            VP_TxByte(VP_MAD, VP_00A_GPO_REG, 0x30);
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x45);
            break;
    } // switch

    #elif (PROJECT == DMO025000100)     ||  \
          (PROJECT == DMO025006500)     ||  \
          (PROJECT == DMO049000100)

    switch (bTCON_Mode)
    {
        case TCON_MODE_RU:
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x0B);
            break;

        case TCON_MODE_LU:
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x01);
            break;

        case TCON_MODE_LD:
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x04);
            break;

        case TCON_MODE_RD:
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x0E);
            break;
    } // switch

    #elif (PROJECT == DMO032006200)

    switch (bTCON_Mode)
    {
        case TCON_MODE_RU:
            VP_TxByte(VP_MAD, VP_00A_GPO_REG, 0x20);
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x44);
            break;

        case TCON_MODE_LU:
            VP_TxByte(VP_MAD, VP_00A_GPO_REG, 0x00);
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x46);
            break;

        case TCON_MODE_LD:
            VP_TxByte(VP_MAD, VP_00A_GPO_REG, 0x10);
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x47);
            break;

        case TCON_MODE_RD:
            VP_TxByte(VP_MAD, VP_00A_GPO_REG, 0x30);
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x45);
            break;
    } // switch

    #elif (PROJECT == DMO032006400)     ||  \
          (PROJECT == DMO032006401)     ||  \
          (PROJECT == DMO032006402)

    switch (bTCON_Mode)
    {
        case TCON_MODE_RU:
            VP_TxByte(VP_MAD, VP_00A_GPO_REG, 0x20);
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x44);
            break;

        case TCON_MODE_LU:
            VP_TxByte(VP_MAD, VP_00A_GPO_REG, 0x00);
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x46);
            break;

        case TCON_MODE_LD:
            VP_TxByte(VP_MAD, VP_00A_GPO_REG, 0x10);
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x47);
            break;

        case TCON_MODE_RD:
            VP_TxByte(VP_MAD, VP_00A_GPO_REG, 0x30);
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x45);
            break;
    } // switch

    #elif (PROJECT == DMO059000100)     ||  \
          (PROJECT == DMO059000101)     ||  \
          (PROJECT == DMO059000102)     ||  \
          (PROJECT == DMO069000100)     ||  \
          (PROJECT == DMO069000101)     ||  \
          (PROJECT == DMO069000102)     ||  \
          (PROJECT == DMO069000103)     ||  \
          (PROJECT == DMO069000104)     ||  \
          (PROJECT == DMO069000105)

    switch (bTCON_Mode)
    {
        case TCON_MODE_RU:
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x0A);
            break;

        case TCON_MODE_LU:
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x00);
            break;

        case TCON_MODE_LD:
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x05);
            break;

        case TCON_MODE_RD:
            VP_TxByte(VP_MAD, VP_032_TCON_ATTR1, 0x0F);
            break;
    } // switch

    #else
        #error VP.C - Invalid PROJECT !
    #endif
} // VP_SetTCON_Mode
#endif


#if (VP_SET_TESTPATTERN)
/* -------------------------------------------------------------------
    Name: VP_SetTestPattern -
    Purpose: VP Background and Test Pattern.
    Passed:
    Returns: None
    Notes: [1]40
   ------------------------------------------------------------------- */
void VP_SetTestPattern (
UB8 bColorR,
UB8 bColorG,
UB8 bColorB,
UB8 bPatternType,   // Pattern Type
UB8 bMode           // Freerun/Background
)
{
    UB8     b42;


    b42 = VP_RxByte(VP_MAD, BITEK_042_TESTPAT_ATTR)                         &
          (VP_MASK_PATTERN_HV | VP_MASK_PATTERN_DIR | VP_MASK_PATTERN_TYPE);

    VP_TxByte(VP_MAD, BITEK_03F_R_ATTR, bColorR);
    VP_TxByte(VP_MAD, BITEK_040_G_ATTR, bColorG);
    VP_TxByte(VP_MAD, BITEK_041_B_ATTR, bColorB);

    VP_TxByte(VP_MAD, BITEK_042_TESTPAT_ATTR, b42 | bMode | bPatternType);
} /* VP_SetTestPattern */
#endif

#endif


#if (VP_SETUP)
/* -------------------------------------------------------------------
    Name: VP_Setup -
    Purpose: To setup VP.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void VP_Setup (void)
{
    VP_SetBrightness(tsGD.bBrightness);
    VP_SetContrast(tsGD.bContrast);
    VP_SetSaturation(tsGD.bSaturation);

    VP_SetSource(tsGD.bSource);
} // VP_Setup
#endif


/* **********************************************************************

    Description:

   ********************************************************************** */

/* %% End Of File %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
