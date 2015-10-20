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

    Module: EEPROM.H -

    Purpose: Interface of EEPROM.

    Version: 0.01                                   06:41PM  2013/03/28

    Compiler: Keil 8051 C Compiler v9.51

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 06:40PM  2013/03/28 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ********************************************************************** */

#ifndef _EEPROM_H_
#define _EEPROM_H_


/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "common.h"
#include "vp.h"


/* ------------------------------------
    Macro Definitions
   ------------------------------------ */
#undef EXTERN

#ifdef _EEPROM_C_
    #define EXTERN
#else
    #define EXTERN  extern
#endif


#define EEPROM_AT24C16AN    1
#define EEPROM_AT24C512N    2
#define EEPROM_FM24C32A     3
#define EEPROM_CFG          EEPROM_FM24C32A


/* ::::::::::::::::::::::::::::::::::::
     EEPROM Cinfiguration
   :::::::::::::::::::::::::::::::::::: */
#define PROJECT_CODE                0x01

#define EEPROM_INIT                 ON

#define EEPROM_READ_BURST           ON
#define EEPROM_READ_BYTE            ON
#define EEPROM_WRITE_BURST          ON
#define EEPROM_WRITE_BYTE           ON


/* ::::::::::::::::::::::::::::::::::::
    tsGD structure starting address
   :::::::::::::::::::::::::::::::::::: */
#if (EEPROM_CFG == EEPROM_AT24C16AN)
    #define EGD_ADDR_BASE           (0x07F0)
#elif (EEPROM_CFG == EEPROM_AT24C512N)
    #define EGD_ADDR_BASE           (0xFFF0)
#else
    #define EGD_ADDR_BASE           (0x0FF0)
#endif


#define EGD_ADDR_PROJECT_CODE       (EGD_ADDR_BASE                                           )

#define EGD_ADDR_BRIGHTNESS         (EGD_ADDR_PROJECT_CODE      + sizeof(tsGD.bProjectCode  ))
#define EGD_ADDR_CONTRAST           (EGD_ADDR_BRIGHTNESS        + sizeof(tsGD.bBrightness   ))
#define EGD_ADDR_SATURATION         (EGD_ADDR_CONTRAST          + sizeof(tsGD.bContrast     ))
#define EGD_ADDR_SOURCE             (EGD_ADDR_SATURATION        + sizeof(tsGD.bSaturation   ))

#if 0
#define EGD_ADDR_SHARPNESS          (EGD_ADDR_SATURATION        + sizeof(tsGD.bSaturation   ))
#define EGD_ADDR_ASPECT_RATIO       (EGD_ADDR_SHARPNESS         + sizeof(tsGD.bSharpness    ))
#define EGD_ADDR_MIRROR_FLIP        (EGD_ADDR_ASPECT_RATIO      + sizeof(tsGD.bAspectRatio  ))
#define EGD_ADDR_SARADC             (EGD_ADDR_MIRROR_FLIP       + sizeof(tsGD.bMirrorFlip   ))
#define EGD_ADDR_FONT               (EGD_ADDR_SARADC            + sizeof(tsGD.bSARADC       ))
#endif


/* ::::::::::::::::::::::::::::::::::::
    EEPROM WP control (RTS1)
    0x0FC = 01Ah (MCU_BREAK INVERT=HIGH)
    0x0FC = 00Ah (MCU_BREAK NORMAL=LOW)
   :::::::::::::::::::::::::::::::::::: */
#define EEPROM_WP_ON                0x1A
#define EEPROM_WP_OFF               0x0A

#define DMA_EEPROM_READ_ON          (BITEK_MASK_DMA_EN | BITEK_MASK_DMA_I2C_BUS | BITEK_MASK_DMA_READ_MODE  )
#define DMA_EEPROM_READ_OFF         (                    BITEK_MASK_DMA_I2C_BUS | BITEK_MASK_DMA_READ_MODE  )

//#define DMA_EEPROM_WRITE_ON         (BITEK_MASK_DMA_EN | BITEK_MASK_DMA_I2C_BUS | BITEK_MASK_DMA_WRITE_MODE | BITEK_MASK_DMA_WAITRDY_EN | BITEK_MASK_DMA_RSTATUS_EN)
//#define DMA_EEPROM_WRITE_OFF        (                    BITEK_MASK_DMA_I2C_BUS | BITEK_MASK_DMA_WRITE_MODE | BITEK_MASK_DMA_WAITRDY_EN | BITEK_MASK_DMA_RSTATUS_EN)

#define DMA_EEPROM_WRITE_ON         (BITEK_MASK_DMA_EN | BITEK_MASK_DMA_I2C_BUS | BITEK_MASK_DMA_WRITE_MODE | BITEK_MASK_DMA_WAITRDY_EN )
#define DMA_EEPROM_WRITE_OFF        (                    BITEK_MASK_DMA_I2C_BUS | BITEK_MASK_DMA_WRITE_MODE | BITEK_MASK_DMA_WAITRDY_EN )


/* ------------------------------------
    Type Definitions
   ------------------------------------ */

/* ::::::::::::::::::::::::::::::::::::
    tEGD: EEPROM General Descriptor
   :::::::::::::::::::::::::::::::::::: */
typedef struct EGD_TAG {
    UB8     bProjectCode;       // Project Code

    UB8     bBrightness ;
    UB8     bContrast   ;
    UB8     bSaturation ;       // Saturation
    UB8     bSource;            // Source

    #if 0
    UB8     bSharpness  ;
    UB8     bAspectRatio;
    UB8     bMirrorFlip ;
    UB8     bSARADC     ;
    UB8     bFont       ;
    #endif

} tEGD;


/* ------------------------------------
    Variables Definitions/Declarations
   ------------------------------------ */
#ifdef _EEPROM_C_

    EXTERN tEGD CODE    tsGD_Default =
    {
        PROJECT_CODE        ,       // Project Code for EEPROM update

        BRIGHTNESS_DEFAULT  ,
        CONTRAST_DEFAULT    ,
        SATURATION_DEFAULT  ,
        SOURCE_DEFAULT      ,       // Source

        #if 0
        SHARPNESS_DEFAULT   ,
        ASPECT_RATIO_DEFAULT,
        MIRROR_FLIP_DEFAULT ,
        SARADC_DEFAULT      ,
        FONT_DEFAULT        ,
        #endif

    };

#else

    EXTERN tEGD CODE    tsGD_Default;

#endif // _EEPROM_C_

EXTERN tEGD tsGD;


/* ------------------------------------
    Function Prototypes
   ------------------------------------ */
#if (EEPROM_INIT)
void EEPROM_Init(void);
#endif

#if (EEPROM_READ_BURST)
void EEPROM_ReadBurst(UW16 wAddr, UB8 bCnt, UB8 *pbDst);
#endif

#if (EEPROM_READ_BYTE)
UB8 EEPROM_ReadByte(UW16 wAddr);
#endif

#if (EEPROM_WRITE_BURST)
void EEPROM_WriteBurst(UW16 wAddr, UB8 bCnt, UB8 *pbSrc);
#endif

#if (EEPROM_WRITE_BYTE)
void EEPROM_WriteByte(UW16 wAddr, UB8 bData);
#endif


#endif /* _EEPROM_H_ */


/* **********************************************************************

    Description:

   ********************************************************************** */

/* %% End Of File %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
