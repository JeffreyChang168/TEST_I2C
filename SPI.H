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

    Module: SPI.H -

    Purpose: Interface of SPI.

    Version: 0.02                                   03:10PM  2014/05/27

    Compiler: Keil 8051 C Compiler v9.52

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 03:55PM  2013/03/17 Chang
    Reason:
        1. Original.
    Solution:

   ----------------------------------------------------------------------
    v0.02 03:09PM  2014/05/27 Chang
    Reason:
        1. To add Software SPI.
    Solution:

   ********************************************************************** */

#ifndef _SPI_H_
#define _SPI_H_


/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "common.h"


/* ------------------------------------
    Macro Definitions
   ------------------------------------ */
#undef EXTERN

#ifdef _SPI_C_
    #define EXTERN
#else
    #define EXTERN  extern
#endif


#define SPI_TXPACKET        OFF
#define SPI_GETHT1380       OFF
#define SPI_SETHT1380       OFF
#define SPI_SETUPHT1380     OFF


/* ::::::::::::::::::::::::::::::::::::
    SPI  Configuration
   :::::::::::::::::::::::::::::::::::: */
/*
    CS      SDA     SCL
    RIN2    RIN3    RIN4    OK 05:38PM  2013/03/17
    GIN2    GIN3    GIN4    OK 05:43PM  2013/03/17
    BIN2    BIN3    BIN4    05:53PM  2013/03/17

    TOUT5   TOUT6   TOUT7
*/

#define SPI_RIN234          0       // CS=RIN2,  SDA=RIN3,  SCL=RIN4    R_MSF_SRC(0x0E3[3:2]) != RIN Port (01) Write ONLY
#define SPI_GIN234          1       // CS=GIN2,  SDA=GIN3,  SCL=GIN4    R_MSF_SRC(0x0E3[3:2]) != GIN Port (10) Write ONLY
#define SPI_BIN234          2       // CS=BIN2,  SDA=BIN3,  SCL=BIN4    R_MSF_SRC(0x0E3[3:2]) != BIN Port (11) Write ONLY
#define SPI_TOUT567         3       // CS=TOUT5, SDA=TOUT6, SCL=TOUT7                                          Write ONLY
#define SPI_RIN012_SW       4       // CS=RIN0,  SDA=RIN1,  SCL=RIN2    M8051 RIN GPIO (0xE8[7]=1)
#define SPI_RIN234_SW       5       // CS=RIN2,  SDA=RIN3,  SCL=RIN4    M8051 RIN GPIO (0xE8[7]=1)
#define SPI_GIN234_SW       6       // CS=GIN2,  SDA=GIN3,  SCL=GIN4    M8051 GIN GPIO (0xEB[7]=1)
#define SPI_BIN234_SW       7       // CS=BIN2,  SDA=BIN3,  SCL=BIN4
#define SPI_TOUT567_SW      8       // CS=TOUT5, SDA=TOUT6, SCL=TOUT7

                                            //  CS      SDA     SCL
//#define SPI_CFG           SPI_RIN234      //  RIN2    RIN3    RIN4    OK 05:37PM  2013/03/17
//#define SPI_CFG           SPI_GIN234      //  GIN2    GIN3    GIN4    OK 05:43PM  2013/03/17
//#define SPI_CFG           SPI_BIN234      //  BIN2    BIN3    BIN4    OK 05:53PM  2013/03/17
//#define SPI_CFG           SPI_TOUT567     //  TOUT5   TOUT6   TOUT7
  #define SPI_CFG           SPI_RIN012_SW   //  RIN0    RIN1    RIN2    OK
//#define SPI_CFG           SPI_GIN234_SW

/*
;   SPI_ATTR    (0x185)
; 7 6 5 4 3 2 1 0
; | | | | | | | |_ SPI_SCL (0=Normal, 1=Invert)
; | | | | | | |___ SPI_SDA (0=Normal, 1=Invert)
; | | | | | |_____ SPI_CS  (0=Normal, 1=Invert)
; | | | | |_______ SPI_SDAM (0=Low level, 1=High Level)
; | | |_|_________ SPI_SCLM (00=Rising  edge/Low  level) SPI Mode 0
; | |                       (01=Rising  edge/High Level) SPI Mode 3
; | |                       (10=Falling edge/Low  level) SPI Mode 1
; | |                       (11=Falling edge/High Level) SPI Mode 2
; |_|_____________ SPI_SPEED (0=XCLK/8, 1=XCLK/16, 2=XCLK/32, 3=XCLK/64)

; 1 1 0 1 1 0 0 0  0xD8   SPEED=XCLK/64, SCLM=01=Rising edge/High Level, SDAM=HIGH
;SPI_CFG                     EQU     0D8h    ; TM040KFH01 (ILI8961)
*/
#define SPI_ATTR             0xD8

/*
;   SPI_CTRL    (0x189)
; 7 6 5 4 3 2 1 0
; | | | |_|_|_|_|_ SPI_BIT (0 to 24)
; | | |___________ SPI_BUSY (0=Free, 1=Busy)
; | |_____________ SPI_SYNC (0=Disable, 1=Enable)
; |_______________ SPI_EN   (0=Disable, 1=Rising edge to send)
; 0 0 0 1 1 0 0 0   0x18 SPI_EN=Disable, SPI_BIT=24
; 0 0 0 1 0 0 0 0   0x10 SPI_EN=Disable, SPI_BIT=16, SYNC Disable
; 0 1 0 1 0 0 0 0   0x50 SPI_EN=Disable, SPI_BIT=16, SYNC Enable
;SPI_CTRL                    EQU     010h
;SPI_CTRL                    EQU     018h
*/

//#define SPI_CTRL_OFF         0x50
//#define SPI_CTRL_ON          0xD0

// 16-bit
//#define SPI_CTRL_OFF         0x10       // OK 05:01PM  2013/01/09
//#define SPI_CTRL_ON          0x90

// 24-bit
  #define SPI_CTRL_OFF         0x18
  #define SPI_CTRL_ON          0x98


/* ------------------------------------
    Type Definitions
   ------------------------------------ */


/* ------------------------------------
    Variables Definitions/Declarations
   ------------------------------------ */

/* ------------------------------------
    Function Prototypes
   ------------------------------------ */
void SPI_Init(void);

UB8 SPI_GetAT25F512AN(UL32 ulAddr);

#if (SPI_TXPACKET)
void SPI_TxPacket(UL32 ulPacket);
#endif

#if (SPI_GETHT1380)
UB8 SPI_GetHT1380(UB8 bReg);
#endif

#if (SPI_SETHT1380)
void SPI_SetHT1380(UB8 bReg, UB8 bData);
#endif

#if (SPI_SETUPHT1380)
void SPI_SetupHT1380(void);
#endif


#if 0
void SPI_FlashWriteEnable(void);
void SPI_FlashChipErase(void);
void SPI_FlashEraseSector(void);
#endif


#endif /* _SPI_H_ */


/* **********************************************************************

    Description:

   ********************************************************************** */

/* %% End Of File %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
