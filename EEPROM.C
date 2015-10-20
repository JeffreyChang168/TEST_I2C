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

    Module: EEPROM.C -

    Purpose: Implementation of EEPROM.

    Version: 0.01                                   06:33PM  2013/03/28

    Compiler: Keil 8051 C Compiler v9.51

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 06:33PM  2013/03/28 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ********************************************************************** */


#define _EEPROM_C_

/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "bit1618c.h"
#include "bitek.h"
#include "platform.h"
#include "osd.h"
#include "timer.h"

#if (PLATFORM_EEPROM)
    #include "eeprom.h"
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

/* ------------------------------------
    Function Prototypes
   ------------------------------------ */


#if (EEPROM_INIT)
/* -------------------------------------------------------------------
    Name: EEPROM_Init -
    Purpose: To initialize EEPROM module.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void EEPROM_Init (void)
{
    /* ::::::::::::::::::::::::::::::::::::
        To recall data structure (EGD) from EEPROM !
       :::::::::::::::::::::::::::::::::::: */
	BITEK_TxByte(BITEK_162_M8051_ATTR4, BITEK_MASK_EEPROM_24C32 + 1);
	BITEK_TxByte(BITEK_165_DMA_SPEED,   0x08);

    // To read tsGD from EEPROM !
    EEPROM_ReadBurst(EGD_ADDR_BASE, sizeof(tsGD), (UB8 *)&tsGD);

    // To initiate EEPROM with default-values
    //  when
    //      PROJECT CODE doesn't match !
    if (tsGD.bProjectCode != PROJECT_CODE)
    {
        // To reset tsGD with default values
        EEPROM_WriteBurst(EGD_ADDR_BASE, sizeof(tsGD_Default), (UB8 *)&tsGD_Default);

        // To read tsGD !
      //EEPROM_ReadBurst(EGD_ADDR_BASE, sizeof(tsGD), (UB8 *)&tsGD);
        tsGD = tsGD_Default;
    }

} /* EEPROM_Init */
#endif


#if (EEPROM_READ_BURST)
/* -------------------------------------------------------------------
    Name: EEPROM_ReadBurst -
    Purpose: To read bulk data from EEPROM.
    Passed:
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void EEPROM_ReadBurst (
UW16    wAddr,
UB8     bCnt,       /* The number of data which will be transmitted */
UB8     *pbDst      /* Point to the destination data */
)
{
    //////////////////////////////////////////////////////////////////
    // CODE SIZE: 0x31
    for (; bCnt; bCnt--)
    {
        *pbDst++ = EEPROM_ReadByte(wAddr++);
    }
    //////////////////////////////////////////////////////////////////
} /* EEPROM_ReadBurst */
#endif


#if (EEPROM_READ_BYTE)
/* -------------------------------------------------------------------
    Name: EEPROM_ReadByte -
    Purpose: To receive bulk CODE data.
    Passed:
    Returns: None.
    Notes:
        The EEPROM_MAD should be the slave device's 'WRITE' module addres,
        not 'READ' one.
   ------------------------------------------------------------------- */
UB8 EEPROM_ReadByte (UW16 wAddr)
{
	BITEK_TxByte(BITEK_166_DMA_COUNT07_00, 0x00);
	BITEK_TxByte(BITEK_167_DMA_COUNT15_08, 0x00);

    BITEK_TxByte(BITEK_168_DMA_EADDR07_00,    wAddr & 0xFF);
    BITEK_TxByte(BITEK_169_DMA_EADDR15_08,    (wAddr >> 8) & 0xFF);
    BITEK_TxByte(BITEK_16A_DMA_EADDR23_16,     0x00);


    BITEK_TxByte(BITEK_16F_DMA_ATTR,    0x05);

    // To enable I2C Single Read Mode
    BITEK_TxByte(BITEK_170_DMA_ATTR1,    DMA_EEPROM_READ_ON);

    while (DMA_BUSY)
        ;

    // To disable I2C Single Read Mode
    BITEK_TxByte(BITEK_170_DMA_ATTR1,    DMA_EEPROM_READ_OFF);

    return( BITEK_RxByte(BITEK_171_DMA_OUT) );
} /* EEPROM_ReadByte */
#endif


#if (EEPROM_WRITE_BURST)
/* -------------------------------------------------------------------
    Name: EEPROM_WriteBurst -
    Purpose: To write bulk data into EEPROM.
    Passed:
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void EEPROM_WriteBurst (
UW16    wAddr,
UB8     bCnt,       /* The number of data which will be transmitted */
UB8     *pbSrc      /* Point to the source data */
)
{
    //////////////////////////////////////////////////////////////////
    // CODE SIZE: 0x31
    for (; bCnt; bCnt--)
    {
        EEPROM_WriteByte(wAddr++, *pbSrc++);
    }
    //////////////////////////////////////////////////////////////////

} /* EEPROM_WriteBurst */
#endif



#define EEPROM_SET_WP_ON    {   BITEK_TxByte(BITEK_0FC_RTS1_ATTR, EEPROM_WP_ON);    }
#define EEPROM_SET_WP_OFF   {   BITEK_TxByte(BITEK_0FC_RTS1_ATTR, EEPROM_WP_OFF);   }



#if (EEPROM_WRITE_BYTE)
/* -------------------------------------------------------------------
    Name: EEPROM_WriteByte -
    Purpose:
        To transmit one byte data to I2C slave device.

    Passed:
    Returns: None.
    Notes:
        The bSLA should be the slave device's 'WRITE' module addres,
        not 'READ' one.
   ------------------------------------------------------------------- */
void EEPROM_WriteByte (
UW16    wAddr,         /* EEPROM address */
UB8     bData          /* Data  */
)
{
    // WP OFF
  //EEPROM_SET_WP_OFF;

	BITEK_TxByte(BITEK_16B_DMA_RADDR07_00, bData);

	BITEK_TxByte(BITEK_166_DMA_COUNT07_00, 0x00);
	BITEK_TxByte(BITEK_167_DMA_COUNT15_08, 0x00);

    BITEK_TxByte(BITEK_168_DMA_EADDR07_00, wAddr & 0xFF);
    BITEK_TxByte(BITEK_169_DMA_EADDR15_08, (wAddr >> 8) & 0xFF);
    BITEK_TxByte(BITEK_16A_DMA_EADDR23_16, 0x00);

	BITEK_TxByte(BITEK_16F_DMA_ATTR, 0x04);

    // To enable I2C Single Write Mode
    BITEK_TxByte(BITEK_170_DMA_ATTR1,    DMA_EEPROM_WRITE_ON);	  //0x17

    while (DMA_BUSY)
        ;

    // To disable I2C Single Write Mode
    BITEK_TxByte(BITEK_170_DMA_ATTR1,    DMA_EEPROM_WRITE_OFF);	   //0x16

    {
        // To wait the EEPROM write cycle time (about 10ms) according to EEPROM type
        UW16 wIdx;

        for (wIdx = 0xFFF; wIdx; wIdx--)
            ;
    }


    // WP ON
  //EEPROM_SET_WP_ON;
} /* EEPROM_WriteByte */
#endif


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
