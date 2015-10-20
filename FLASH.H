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

    Module: FLASH.C -

    Purpose: Interface of FLASH.

    Version: 0.02                                   10:48AM  2013/01/09

    Compiler: Keil 8051 C Compiler v9.51

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 01:27PM  2012/07/09 Paul Lee
    Reason:
        1. Original.
    Solution:

   ----------------------------------------------------------------------
    v0.02 10:49AM  2013/01/09 Jeffrey Chang
    Reason:
        1. To add VERSION_ID for Validation.
    Solution:

   ********************************************************************** */

#ifndef _FLASH_H_
#define _FLASH_H_

/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "vp.h"

/* ------------------------------------
    Macro Definitions
   ------------------------------------ */
#undef EXTERN

#ifdef _FLASH_C_
    #define EXTERN
#else
    #define EXTERN	extern
#endif



#define ENABLE_FLASH_DEUBG  0

// Flash Type
#define FLASH_AT25F512A     1
#define FLASH_AT25F512B     2
#define FLASH_N25S16        3
#define FLASH_W25X40B       4
#define FLASH_CFG           FLASH_AT25F512B

// software write-protect
#define FLASH_SOFT_WP       OFF

#define FLASH_CHECK_STATUS  ON
#define FLASH_DELAY         ON
#define FLASH_ERASE_SECTOR  ON
#define FLASH_INIT          ON
#define FLASH_READ_ALL      ON
#define FLASH_READ_BURST    OFF
#define FLASH_READ_BYTE     ON
#define FLASH_SET_ADDR_CNT  ON
#define FLASH_SET_COMMAND   ON
#define FLASH_WRITE_ALL     ON
#define FLASH_WRITE_BURST   OFF
#define FLASH_WRITE_BYTE    ON
#define FLASH_WRITE_STATUS  OFF



#define FLASH_ERASE_TIME    10


/* ::::::::::::::::::::::::::::::::::::
    tsGD structure starting address
   :::::::::::::::::::::::::::::::::::: */

#if (FLASH_CFG ==  FLASH_AT25F512A)
    #define FGD_ADDR_BASE        (0x008000UL)
#else
    #define FGD_ADDR_BASE        (0x00F000UL)
  //#define FGD_ADDR_BASE        (0x008000UL)
  //#define FGD_ADDR_BASE        (0x001000UL)
#endif



/* ::::::::::::::::::::::::::::::::::::
    tsGD in FLASH starting address
   :::::::::::::::::::::::::::::::::::: */

/*
    1 Page = 4096 Bytes

    a) If RECORD SIZE <= 16, Index Range: 0..255
        4096 / 16 = 256

    b) If RECORD SIZE > 16, Index Range: 0..(4096/RECORD SIZE)

*/

#define FGD_PAGE_SIZE			    (4096)                  // 1 Page = 4096 Bytes
  #define FGD_RECORD_SIZE			(16)                    // If RECORD SIZE <= 16
//#define FGD_RECORD_SIZE			(sizeof(tFGD))          // If RECORD SIZE > 16

#define FGD_ADDR_OFFSET			    (FGD_RECORD_SIZE * tsGD.bIndex)

#define FGD_ADDR_INDEX              (FGD_ADDR_BASE          + FGD_ADDR_OFFSET                )

#define FGD_ADDR_BRIGTNESS          (FGD_ADDR_INDEX         + sizeof(tsGD.bIndex            ))
#define FGD_ADDR_CONTRAST           (FGD_ADDR_BRIGTNESS     + sizeof(tsGD.bBrightness       ))
#define FGD_ADDR_SATURATION         (FGD_ADDR_CONTRAST      + sizeof(tsGD.bContrast         ))
#define FGD_ADDR_SOURCE             (FGD_ADDR_SATURATION    + sizeof(tsGD.bSaturation       ))

#if 0
#define FGD_ADDR_SHARPNESS          (FGD_ADDR_SOURCE        + sizeof(tsGD.bSource           ))
#define FGD_ADDR_ASPECT_RATIO       (FGD_ADDR_SHARPNESS     + sizeof(tsGD.bSharpness        ))
#define FGD_ADDR_MIRROR_FLIP        (FGD_ADDR_ASPECT_RATIO  + sizeof(tsGD.bAspectRatio      ))
#define FGD_ADDR_SARADC			    (FGD_ADDR_MIRROR_FLIP   + sizeof(tsGD.bMirrorFlip       ))
#define FGD_ADDR_FONT  			    (FGD_ADDR_SARADC        + sizeof(tsGD.bSARADC           ))
#endif

#define FGD_ADDR_VERSION_ID         (FGD_ADDR_BASE          + FGD_PAGE_SIZE - 1             )

#define FLASH_INDEX_SIZE            (FGD_PAGE_SIZE / FGD_RECORD_SIZE)
#define FLASH_INDEX_MIN             (0                      )
#define FLASH_INDEX_MAX             (FLASH_INDEX_SIZE - 1   )

#define FLASH_EMPTY_MARK            (0xFF                   )
#define FLASH_INDEX_DEFAULT         (FLASH_EMPTY_MARK - 1   )


/* ------------------------------------
    Type Definitions
   ------------------------------------ */
/* ::::::::::::::::::::::::::::::::::::
    tFGD: FLASH General Descriptor
   :::::::::::::::::::::::::::::::::::: */
typedef struct FGD_TAG {
	UB8     bIndex      ;       // RECORD Index (0..FLASH_INDEX_MAX)

	UB8     bBrightness ;
    UB8     bContrast   ;
    UB8     bSaturation ;
    UB8     bSource     ;

    #if 0
    UB8     bSharpness  ;
    UB8     bAspectRatio;
    UB8     bMirrorFlip ;
    UB8     bSARADC     ;
    UB8     bFont       ;
    #endif

} tFGD;


/* ------------------------------------
    Variables Definitions/Declarations
   ------------------------------------ */
#ifdef _FLASH_C_

    EXTERN tFGD CODE    tsGD_Default =
    {
        FLASH_INDEX_DEFAULT     ,

        BRIGHTNESS_DEFAULT      ,
        CONTRAST_DEFAULT        ,
        SATURATION_DEFAULT      ,
        SOURCE_DEFAULT          ,

        #if 0
        SHARPNESS_DEFAULT       ,
        ASPECT_RATIO_DEFAULT    ,
        MIRROR_FLIP_DEFAULT     ,
        SARADC_DEFAULT          ,
        FONT_DEFAULT            ,
        #endif
    };


#else

    EXTERN tFGD CODE    tsGD_Default;

#endif


#if (FLASH_READ_BURST)
    EXTERN XDATA tFGD   tsGD    _at_    0x7C00;
#else
    EXTERN XDATA tFGD   tsGD;
#endif


/* ------------------------------------
    Function Prototypes
   ------------------------------------ */
#if (FLASH_CHECK_STATUS)
void FLASH_CheckStatus(void);
#endif

#if (FLASH_DELAY)
void FLASH_Delay(UW16 wTime);
#endif

#if (FLASH_ERASE_SECTOR)
void FLASH_EraseSector(UL32 dwAddr);
#endif

#if (FLASH_INIT)
void FLASH_Init(void);
#endif

#if (FLASH_READ_ALL)
void FLASH_ReadAll(void);
#endif

#if (FLASH_READ_BYTE)
UB8  FLASH_ReadByte(UL32 dwAddr);
#endif

#if (FLASH_READ_BURST)
void FLASH_ReadBurst(UL32 dwAddr, UW16 wCnt, UW16 pbData);
#endif

#if (FLASH_SET_ADDR_CNT)
void FLASH_SetAddrCnt(UL32 dwAddress, UW16 wCnt);
#endif

#if (FLASH_SET_COMMAND)
void FLASH_SetCommand(UB8 bOPCode, UB8 bCmdType, UB8 bCtrl1, UB8 bCtrl2);
#endif

#if (FLASH_WRITE_ALL)
void FLASH_WriteAll(void);
#endif

#if (FLASH_WRITE_BYTE)
void FLASH_WriteByte(UL32 dwAddr, UB8 bData);
#endif

#if (FLASH_WRITE_BURST)
void FLASH_WriteBurst(UL32 dwAddr, UB8 bCNT, UB8 *pbData  );
#endif

#if (FLASH_WRITE_STATUS)
void FLASH_WriteStatus(void);
#endif


#endif /* _FLASH_H_ */


/* **********************************************************************

    Description:

   ********************************************************************** */

/* %% End Of File %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
