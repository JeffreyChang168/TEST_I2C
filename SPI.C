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

    Module: SPI.C -

    Purpose: Implementation of SPI.

    Version: 0.02                                   03:13PM  2014/05/27

    Compiler: Keil 8051 C Compiler v9.52

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 03:56PM  2013/03/17 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ----------------------------------------------------------------------
    v0.02 03:13PM  2014/05/27 Jeffrey Chang
    Reason:
        1. To add Software SPI.
    Solution:

   ********************************************************************** */

#define _SPI_C_

/* ------------------------------------
    Header Files
   ------------------------------------ */
#include <absacc.h>
#include <intrins.h>

#include "bit1618c.h"
#include "bitek.h"
#include "common.h"
#include "spi.h"

/* ------------------------------------
    Macro Definitions
   ------------------------------------ */
#define NOP_1           { _nop_(); }
#define NOP_2           { NOP_1; NOP_1; }
#define NOP_4           { NOP_2; NOP_2; }

#define SPI_T_H0        NOP_4   /* Min. 150 ns                          */
#define SPI_T_H1        NOP_4   /* Min. 150 ns                          */

#define SPI_TXBYTE      OFF

/* ------------------------------------
    Type Definitions
   ------------------------------------ */


/* ------------------------------------
    Variables Definitions
   ------------------------------------ */
#if (SPI_CFG == SPI_RIN012_SW)
sbit SPI_SCL        = P0 ^ 2;       /* SPI clock            (SCL)       */
sbit SPI_SDA        = P0 ^ 1;       /* Master Out Slave In  (SDA)       */
sbit SPI_CS         = P0 ^ 0;       /* Slave Select         (CS)        */
#elif (SPI_CFG == SPI_RIN234_SW)
sbit SPI_SCL        = P0 ^ 4;       /* SPI clock            (SCL)       */
sbit SPI_SDA        = P0 ^ 3;       /* Master Out Slave In  (SDA)       */
sbit SPI_CS         = P0 ^ 2;       /* Slave Select         (CS)        */
#elif (SPI_CFG == SPI_GIN234_SW)
sbit SPI_SCL        = P1 ^ 4;       /* SPI clock            (SCL)       */
sbit SPI_SDA        = P1 ^ 3;       /* Master Out Slave In  (SDA)       */
sbit SPI_CS         = P1 ^ 2;       /* Slave Select         (CS)        */
#elif (SPI_CFG == SPI_BIN234_SW)
sbit SPI_SCL        = P2 ^ 4;       /* SPI clock            (SCL)       */
sbit SPI_SDA        = P2 ^ 3;       /* Master Out Slave In  (SDA)       */
sbit SPI_CS         = P2 ^ 2;       /* Slave Select         (CS)        */
#endif


/* ------------------------------------
    Function Prototypes
   ------------------------------------ */

/* -------------------------------------------------------------------
    Name: SPI_Init -
    Purpose: To initialize SPI module.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void SPI_Init (void)
{
    // Setup SPI pins
    #if (SPI_CFG == SPI_RIN234)
    BITEK_TxByte(BITEK_0E8_RIN_ATTR, BITEK_MASK_RIN_SPI_EN);
    BITEK_TxByte(BITEK_185_SPI_ATTR, SPI_ATTR);
    #elif (SPI_CFG == SPI_GIN234)
    BITEK_TxByte(BITEK_0EB_GIN_ATTR, BITEK_MASK_GIN_SPI_EN);
    BITEK_TxByte(BITEK_185_SPI_ATTR, SPI_ATTR);
    #elif (SPI_CFG == SPI_BIN234)
    BITEK_TxByte(BITEK_0EE_BIN_ATTR, BITEK_MASK_BIN_SPI_EN);
    BITEK_TxByte(BITEK_185_SPI_ATTR, SPI_ATTR);
    #elif (SPI_CFG == SPI_TOUT567)
    BITEK_TxByte(BITEK_0F3_TOUT_ATTR, BITEK_MASK_TOUT_SPI_EN);
    BITEK_TxByte(BITEK_185_SPI_ATTR, SPI_ATTR);

    #elif (SPI_CFG == SPI_RIN012_SW)
    P0MOD = 0xFF;

    // RIN (P0) as M8051 GPIO (0x0E8[7]=1)
    XBYTE[ BITEK_0E8_RIN_ATTR ] |= BITEK_MASK_RIN_GPIO;

    // R_RIN10_MFP_EN (R0E8[0]) = 0
    // R_RIN_SPI_EN   (R0E8[3]) = 0
    XBYTE[ BITEK_0E8_RIN_ATTR ] &= ~(BITEK_MASK_RIN_SPI_EN | BITEK_MASK_RIN10_MFP_EN);

    // R_MSF_SRC (R164[6:5]) != 01,
    XBYTE[ BITEK_164_MSF_ATTR ] &= ~BITEK_MASK_MSF_SRC;

    SPI_CS  = 1;

    #elif (SPI_CFG == SPI_GIN234_SW)
    // GIN (P1) as M8051 GPIO (0x0EB[7]=1)
    BITEK_TxByte(BITEK_0EB_GIN_ATTR, BITEK_RxByte(BITEK_0EB_GIN_ATTR) | BITEK_MASK_GIN_GPIO);
    #endif
} /* SPI_Init */


/* -------------------------------------------------------------------
    Name: SPI_GetAT25F512AN -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
UB8 SPI_GetAT25F512AN (UL32 ulAddr)
{
    UL32    ulMask;
    UB8     bMask;
    UB8     bValue;


    // OpCode = 0x03
    ulAddr |= 0x03000000UL;

    SPI_SCL = 0;
    SPI_CS  = 0;

    // From MSB to LSB
    for (ulMask = 0x80000000UL; ulMask; ulMask >>= 1)
    {
        SPI_SDA = ulAddr & ulMask;

        // SDA is loaded one bit a time at the rising edge of SCL
        SPI_SCL = 1;
        SPI_T_H1;
        SPI_SCL = 0;
    }

    bValue = 0;
    // From MSB to LSB
    for (bMask = 0x80; bMask; bMask >>= 1)
    {
        // SDA is loaded one bit a time at the rising edge of SCL
        SPI_SCL = 1;
        SPI_T_H1;

        if (SPI_SDA)
            bValue |= bMask;
        SPI_T_H1;

        SPI_SCL = 0;
    }

    SPI_T_H0;

    SPI_CS = 1;

    return(bValue);
}


#if (SPI_TXPACKET)
/* -------------------------------------------------------------------
    Name: SPI_TxPacket -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void SPI_TxPacket (UL32 ulPacket)
{
    UW16    wIdx;


    // 0x186
    BITEK_TxByte(BITEK_186_SPI_DATA07_00, (ulPacket >>  0));

    // 0x187
    BITEK_TxByte(BITEK_187_SPI_DATA15_08, (ulPacket >>  8));

    // 0x188
    BITEK_TxByte(BITEK_188_SPI_DATA23_16, (ulPacket >> 16));

    // 0x189
    // SPI OFF
    BITEK_TxByte(BITEK_189_SPI_ATTR1, SPI_CTRL_OFF);

    // SPI ON
    BITEK_TxByte(BITEK_189_SPI_ATTR1, SPI_CTRL_ON);


    for (wIdx = 0xFFFF; wIdx; wIdx--)
    {
        if ((BITEK_RxByte(BITEK_189_SPI_ATTR1) & BITEK_MASK_SPI_BUSY_O) == 0)
            break;
    }

    // 0x189
    // SPI OFF
    BITEK_TxByte(BITEK_189_SPI_ATTR1, SPI_CTRL_OFF);
}
#endif


#if (SPI_GETHT1380)
/* -------------------------------------------------------------------
    Name: SPI_GetHT1380 -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
UB8 SPI_GetHT1380 (UB8 bReg)
{
    UB8 bMask;
    UB8 bValue;


    SPI_SCL = 0;
    SPI_CS  = 1;

    // From LSB to MSB
    for (bMask = 0x01; bMask; bMask <<= 1)
    {
        SPI_SDA = bReg & bMask;

        // SDA is loaded one bit a time at the rising edge of SCL
        SPI_SCL = 1;
        SPI_T_H1;
        SPI_SCL = 0;
    }

    bValue = 0;
    // From LSB to MSB
    for (bMask = 0x01; bMask; bMask <<= 1)
    {
        // SDA is loaded one bit a time at the rising edge of SCL
        SPI_SCL = 1;
        SPI_T_H1;

        if (SPI_SDA)
            bValue |= bMask;
        SPI_T_H1;

        SPI_SCL = 0;
    }

    SPI_T_H0;

    SPI_CS = 0;

    return( bValue );
}
#endif


#if (SPI_SETHT1380)
/* -------------------------------------------------------------------
    Name: SPI_SetHT1380 -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void SPI_SetHT1380 (
UB8 bReg,
UB8 bData
)
{
    #if (SPI_CFG == SPI_RIN234)     ||  \
        (SPI_CFG == SPI_GIN234)     ||  \
        (SPI_CFG == SPI_BIN234)     ||  \
        (SPI_CFG == SPI_TOUT567)

    UB8 bMask;
    UB8 bValue;


    bValue = 0;
    for (bMask = 0x80; bMask; bMask >>= 1)
    {
        bValue >>= 1;
        if (bData & bMask)
        {
            bValue |= 0x80;
        }
    }

    BITEK_TxByte(BITEK_189_SPI_ATTR1,       SPI_CTRL_OFF);
    BITEK_TxByte(BITEK_186_SPI_DATA07_00,   bValue);
    BITEK_TxByte(BITEK_187_SPI_DATA15_08,   bReg);
    BITEK_TxByte(BITEK_189_SPI_ATTR1,       SPI_CTRL_ON);

    #else

    UB8 bMask;


    SPI_SCL = 0;
    SPI_CS  = 1;

    // From LSB to MSB
    for (bMask = 0x01; bMask; bMask <<= 1)
    {
        SPI_SDA = bReg & bMask;

        // SDA is loaded one bit a time at the rising edge of SCL
        SPI_SCL = 1;
        SPI_T_H1;
        SPI_SCL = 0;
    }

    // From LSB to MSB
    for (bMask = 0x01; bMask; bMask <<= 1)
    {
        SPI_SDA = bData & bMask;

        // SDA is loaded one bit a time at the rising edge of SCL
        SPI_SCL = 1;
        SPI_T_H1;
        SPI_SCL = 0;
    }

    SPI_T_H0;

    SPI_CS = 0;

    #endif
}
#endif


#if (SPI_TXBYTE)
/* -------------------------------------------------------------------
    Name: SPI_TxByte -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void SPI_TxByte (UB8 bByte)
{
    UB8 bMask;


    // From LSB to MSB
    for (bMask = 0x01; bMask; bMask <<= 1)
    {
        SPI_SDA = bByte & bMask;

        // SDA is loaded one bit a time at the rising edge of SCL
        SPI_T_H1;
        SPI_SCL = 1;
        SPI_T_H1;
        SPI_SCL = 0;
    }
}
#endif


#if (SPI_SETUPHT1380)
/* -------------------------------------------------------------------
    Name: SPI_SetupHT1380 -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void SPI_SetupHT1380 (void)
{
    UB8 bHT1380_Default[] =
	{
        0x00,       // Seconds: 00
        0x59,       // Minutes: 59
        0x23,       // Hours:   23

        0x02,       // Day:     02
        0x01,       // Month:   01
        0x03,       // Weekday: 03 Wed
        0x13,       // Year:    13
	};
    UB8 bIdx;


    SPI_SCL = 0;
    SPI_CS  = 1;


  //SPI_TxByte(HT1380_W_BURST);
    SPI_TxByte(0xBE);

    for (bIdx = 0; bIdx < sizeof(bHT1380_Default); bIdx++)
        SPI_TxByte(bHT1380_Default[bIdx]);


    SPI_T_H0;
    SPI_CS = 0;
}
#endif





#if 0
void SPI_FlashWriteEnable(void)
{
    BITEK_TxByte(BITEK_189_SPI_ATTR1, 0x08);
	BITEK_TxByte(BITEK_186_SPI_DATA07_00, 0x06);
    BITEK_TxByte(BITEK_189_SPI_ATTR1, 0x88);
    while (BITEK_RxByte(BITEK_189_SPI_ATTR1) & BITEK_MASK_SPI_BUSY);
    BITEK_TxByte(BITEK_189_SPI_ATTR1, 0x08);
}


void SPI_FlashChipErase(void)	//ok
{

   SPI_FlashWriteEnable();

   BITEK_TxByte(BITEK_186_SPI_DATA07_00, 0x60);
   BITEK_TxByte(BITEK_189_SPI_ATTR1, 0x8);
   while (BITEK_RxByte(BITEK_189_SPI_ATTR1) & BITEK_MASK_SPI_BUSY);
   BITEK_TxByte(BITEK_189_SPI_ATTR1, 0x88);
   BITEK_TxByte(BITEK_189_SPI_ATTR1, 0x8);
}

void SPI_FlashEraseSector(void)	 //Fail ,cs can't alawsy low
{

    SPI_FlashWriteEnable();


    BITEK_TxByte(BITEK_189_SPI_ATTR1, 0x08);
	BITEK_TxByte(BITEK_186_SPI_DATA07_00, 0x20);
    BITEK_TxByte(BITEK_189_SPI_ATTR1, 0x88);
    while (BITEK_RxByte(BITEK_189_SPI_ATTR1) & BITEK_MASK_SPI_BUSY);

	BITEK_TxByte(BITEK_189_SPI_ATTR1, 0x08);

	BITEK_TxByte(BITEK_189_SPI_ATTR1, 0x18);
	BITEK_TxByte(BITEK_186_SPI_DATA07_00, 0x00);
	BITEK_TxByte(BITEK_187_SPI_DATA15_08, 0x10);
	BITEK_TxByte(BITEK_188_SPI_DATA23_16, 0x00);

    BITEK_TxByte(BITEK_189_SPI_ATTR1, 0x98);
    while (BITEK_RxByte(BITEK_189_SPI_ATTR1) & BITEK_MASK_SPI_BUSY);
    BITEK_TxByte(BITEK_189_SPI_ATTR1, 0x18);

}

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
