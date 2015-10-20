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

    Module: MAIN.C -

    Purpose: Implementation of MAIN module.

    Version: 0.01                                   03:20PM  2014/05/23

    Compiler: Keil 8051 C Compiler v9.52

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 03:20PM  2014/05/23 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

    ********************************************************************** */

#define _MAIN_C_


/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "bit1618c.h"
#include "bitek.h"
#include "dispatch.h"
#include "gpi.h"
#include "i2c.h"
#include "main.h"
#include "mcu.h"
#include "osd.h"
#include "por.h"
#include "spi.h"
#include "timer.h"
#include "vp.h"


/* ------------------------------------
    Macro Definitions
   ------------------------------------ */
//; //////////////////////////////////////////////////////////////////////////////
//;24C16 = 07h,24C32 = 87h        OVER 400KHz
//;24C16 = 08h,24C32 = 88h
#define SPEED_24C16     0x08
#define SPEED_24C32     0x88
#define EEPROM_SPEED    SPEED_24C32


/* ------------------------------------
    Type Definitions
   ------------------------------------ */

/* ------------------------------------
    Variables Definitions
   ------------------------------------ */

/* ------------------------------------
    Function Prototypes
   ------------------------------------ */


/* -------------------------------------------------------------------
    Name: main -
    Purpose:
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void main (void)
{
    /* Power-On Reset */
    POR_Init();

    #if (PLATFORM_SPI_SW)
    // SPI Mode
    SPI_Init();
    {
        UB8 bIdx;

        for (bIdx = 0; bIdx < 7; bIdx++)
           	OSD_ShowHex(VP_SHOW_SPI + 3 * bIdx, SPI_GetAT25F512AN(0x50 + bIdx));
    }
    #endif

    // I2C Mode
    I2C_Init();

    #if (I2C_MASTER)

#if 1
    // I2C Master
    {
        UB8 bIdx;


        for (bIdx = 0; bIdx < 7; bIdx++)
        {
            // I2C Master RxByte
            OSD_ShowHex(VP_SHOW_I2C + 3 * bIdx, I2C_MASTER_RxByte(I2C_M_MAD, bIdx));

            // I2C Master RxBurst
            #if 0
            {
                UB8 bData;

                I2C_MASTER_RxBurst(I2C_M_MAD, bIdx, 1, &bData);
                OSD_ShowHex(VP_SHOW_I2C + 3 * bIdx, bData);
            }
            #endif
        }
    }
#endif

#if 0
    I2C_MASTER_TxByte(I2C_M_MAD, 0x55, 0xAA);
#endif

    #endif


    for (;;)
    {
        if (bMCU_BL_Cnt)
        {
            if (bMCU_BL_Cnt <= BL_STOP)
            {
                DISPATCH_Task_BL_Cnt();

                bMCU_BL_Cnt = 0;
            }
        }
    } /* for endless loop */
} /* main */


/* -------------------------------------------------------------------
    Name:  -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */

/* **********************************************************************

    Description:


   ********************************************************************** */

/* %% End Of File %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
