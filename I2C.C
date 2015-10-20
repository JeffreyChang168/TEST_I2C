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

    Module: I2C.C -

    Purpose: Implementation of I2C.

    Version: 0.01                                   01:48PM  2014/05/20

    Compiler: Keil 8051 C Compiler v9.52

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 01:48PM  2014/05/20 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ********************************************************************** */

#define _I2C_C_

/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "bit1618c.h"
#include "common.h"
#include "bitek.h"
#include "vp.h"
#include "osd.h"
#include "platform.h"
#include "i2c.h"



/* ------------------------------------
    Macro Definitions
   ------------------------------------ */
#define I2C_CMD_MAD_DIS			(BITEK_MASK_I2C_M_START + BITEK_MASK_I2C_M_WDATA_EN + BITEK_MASK_I2C_M_RACK_EN)
#define I2C_CMD_MAD_EN			(BITEK_MASK_I2C_M_START + BITEK_MASK_I2C_M_WDATA_EN + BITEK_MASK_I2C_M_RACK_EN + BITEK_MASK_I2C_M_EN)

#define I2C_CMD_REG_DIS			(BITEK_MASK_I2C_M_WDATA_EN + BITEK_MASK_I2C_M_RACK_EN)
#define I2C_CMD_REG_EN			(BITEK_MASK_I2C_M_WDATA_EN + BITEK_MASK_I2C_M_RACK_EN + BITEK_MASK_I2C_M_EN)

#define I2C_CMD_REG_END_DIS		(BITEK_MASK_I2C_M_WDATA_EN + BITEK_MASK_I2C_M_RACK_EN + BITEK_MASK_I2C_M_STOP)
#define I2C_CMD_REG_END_EN		(BITEK_MASK_I2C_M_WDATA_EN + BITEK_MASK_I2C_M_RACK_EN + BITEK_MASK_I2C_M_STOP + BITEK_MASK_I2C_M_EN)

#define I2C_CMD_WDATA_DIS		(BITEK_MASK_I2C_M_WDATA_EN + BITEK_MASK_I2C_M_RACK_EN)
#define I2C_CMD_WDATA_EN		(BITEK_MASK_I2C_M_WDATA_EN + BITEK_MASK_I2C_M_RACK_EN + BITEK_MASK_I2C_M_EN)

#define I2C_CMD_WDATA_END_DIS	(BITEK_MASK_I2C_M_WDATA_EN + BITEK_MASK_I2C_M_RACK_EN + BITEK_MASK_I2C_M_STOP)
#define I2C_CMD_WDATA_END_EN	(BITEK_MASK_I2C_M_WDATA_EN + BITEK_MASK_I2C_M_RACK_EN + BITEK_MASK_I2C_M_STOP + BITEK_MASK_I2C_M_EN)

#define I2C_CMD_RDATA_DIS		(BITEK_MASK_I2C_M_RDATA_EN + BITEK_MASK_I2C_M_WACK_EN)
#define I2C_CMD_RDATA_EN		(BITEK_MASK_I2C_M_RDATA_EN + BITEK_MASK_I2C_M_RACK_EN + BITEK_MASK_I2C_M_EN)

#define I2C_CMD_RDATA_END_DIS	(BITEK_MASK_I2C_M_RDATA_EN + BITEK_MASK_I2C_M_RACK_EN + BITEK_MASK_I2C_M_STOP)
#define I2C_CMD_RDATA_END_EN	(BITEK_MASK_I2C_M_RDATA_EN + BITEK_MASK_I2C_M_RACK_EN + BITEK_MASK_I2C_M_STOP + BITEK_MASK_I2C_M_EN)

//
#define I2C_CMD_MAD			I2C_CMD_MAD_DIS

#define I2C_CMD_REG			I2C_CMD_REG_DIS
#define I2C_CMD_REG_END		I2C_CMD_REG_END_DIS

#define I2C_CMD_WDATA		I2C_CMD_WDATA_DIS
#define I2C_CMD_WDATA_END	I2C_CMD_WDATA_END_DIS

#define I2C_CMD_RDATA		I2C_CMD_RDATA_DIS
#define I2C_CMD_RDATA_END	I2C_CMD_RDATA_END_DIS




#define I2C_M_WRITE_CMD(bCmd, bData)                                            \
    {                                                                           \
        BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, bCmd);                               \
        BITEK_TxByte(BITEK_1B4_I2C_M_WDATA, bData);                             \
        BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, bCmd + BITEK_MASK_I2C_M_EN);         \
        BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, bCmd);                               \
        while (BITEK_RxByte(BITEK_1B7_I2C_M_INFO) & BITEK_MASK_I2C_M_BUSY_O);   \
    }

#define I2C_M_READ_CMD(bCmd, bData)                                             \
    {                                                                           \
        BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, bCmd);                               \
        BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, bCmd + BITEK_MASK_I2C_M_EN);         \
        BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, bCmd);                               \
        while (BITEK_RxByte(BITEK_1B7_I2C_M_INFO) & BITEK_MASK_I2C_M_BUSY_O);   \
        bData = BITEK_RxByte(BITEK_1B6_I2C_M_RDATA_O);                          \
    }

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
    Name: I2C_Init -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_Init (void)
{
    #if (I2C_MPIN_CFG != I2C_PIN_NONE_M)
	I2C_MASTER_Init();
	#endif


    #if (I2C_SPIN_CFG != I2C_PIN_NONE_S)
	I2C_SLAVE_Init();
	#endif

  //BITEK_TxByte(BITEK_0F5_M8051_ATTR1, I2C_MSPIN_SEL);

    #if (DEBUG_I2C)
    bI2C_Idx = 0;
    #endif

}


#if (I2C_MASTER_HANDLER)
/* -------------------------------------------------------------------
    Name: I2C_MASTER_Handler -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_MASTER_Handler (void)
{
	switch (bI2C_M_State)
	{
		case eI2C_STATE_MAD:
			// Start + MAD (Module Address) + RACK
			BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, I2C_CMD_MAD_DIS);
			BITEK_TxByte(BITEK_1B4_I2C_M_WDATA, I2C_M_MAD);
			BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, I2C_CMD_MAD_EN);
			BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, I2C_CMD_MAD_DIS);

			bI2C_M_State  = eI2C_STATE_MAD_ACK;
			bI2C_M_Cnt    = 0;
			bI2C_M_CmdCnt = 0;
			break;

		case eI2C_STATE_REG:
			// SAD (Sub-Address) + RACK
			BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, I2C_CMD_REG_DIS);
			BITEK_TxByte(BITEK_1B4_I2C_M_WDATA, 0xE4);
			BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, I2C_CMD_REG_EN);
			BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, I2C_CMD_REG_DIS);

			bI2C_M_State = eI2C_STATE_REG_ACK;
		  //bI2C_M_Cnt   = 5;
			break;

		case eI2C_STATE_MAD_READ:
			BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, I2C_CMD_MAD_DIS);
			BITEK_TxByte(BITEK_1B4_I2C_M_WDATA, (I2C_M_MAD | 0x01));
			BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, I2C_CMD_MAD_EN);
			BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, I2C_CMD_MAD_DIS);

			bI2C_M_State = eI2C_STATE_MAD_READ_ACK;
			break;

		case eI2C_STATE_DATA_END:
			BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, I2C_CMD_RDATA_END_DIS);
			BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, I2C_CMD_RDATA_END_EN);
			BITEK_TxByte(BITEK_1B5_I2C_M_ATTR, I2C_CMD_RDATA_END_DIS);

			bI2C_M_State = eI2C_STATE_DATA_END_ACK;
			break;

		case eI2C_STATE_MAD_ACK:
		case eI2C_STATE_REG_ACK:
		case eI2C_STATE_MAD_READ_ACK:
		case eI2C_STATE_DATA_END_ACK:
			if (BITEK_MASK_I2C_M_BUSY_O != (BITEK_RxByte(BITEK_1B7_I2C_M_INFO) & BITEK_MASK_I2C_M_BUSY_O))
			{
				bI2C_M_State++;
			}
			break;

		case eI2C_STATE_END:
            #if (DEBUG_I2C)
			BITEK_TxByte(BITEK_001_SW_VER, BITEK_RxByte(BITEK_1B6_I2C_M_RDATA_O));
            #endif
			break;

		case eI2C_STATE_IDLE:
		default:
			break;
	}
}
#endif


#if (I2C_MASTER_INIT)
/* -------------------------------------------------------------------
    Name: I2C_MASTER_Init -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_MASTER_Init (void)
{
    // I2C Master Speed
  //BITEK_TxByte(BITEK_1B3_I2C_M_SPEED, 0x3F);
    BITEK_TxByte(BITEK_1B3_I2C_M_SPEED, 0x1F);


	#if (I2C_MPIN_CFG == I2C_PIN_RIN)
    /*
        R_RIN10_MFP_EN  (R0E8[0])  = 1;
        R_RIN10_MFP_SEL (R0E8[2:1) = 10;
        R_I2C_M_SRC (R0F5[2:0]) = 000)
    */

	// RIN as I2C Master pin
	BITEK_TxByte(BITEK_0E8_RIN_ATTR, BITEK_MASK_RIN10_MFP_EN + BITEK_MASK_RIN10_MFP_SEL_I2C_MASTER);	//05
  //BITEK_TxByte(BITEK_0F5_M8051_ATTR1, BITEK_MASK_I2C_M_RIN);

	#elif (I2C_MPIN_CFG == I2C_PIN_GIN)

	// GIN as I2C Master pin
	BITEK_TxByte(BITEK_0EB_GIN_ATTR, BITEK_MASK_GIN10_MFP_EN + BITEK_MASK_GIN10_MFP_SEL_I2C_MASTER);	//05
  //BITEK_TxByte(BITEK_0F5_M8051_ATTR1, BITEK_MASK_I2C_M_GIN);	//0x1

	#elif (I2C_MPIN_CFG == I2C_PIN_BIN)

	// BIN as I2C Master pin
	BITEK_TxByte(BITEK_0EE_BIN_ATTR, BITEK_MASK_BIN10_MFP_EN + BITEK_MASK_BIN10_MFP_SEL_I2C_MASTER);	//05
  //BITEK_TxByte(BITEK_0F5_M8051_ATTR1, BITEK_MASK_I2C_M_BIN);	//0x2

	#elif (I2C_MPIN_CFG == I2C_PIN_SYNC)

	// HS/VS Sync as I2C Master pin
	BITEK_TxByte(BITEK_0F1_VSYNC_ATTR, BITEK_MASK_VSYNC_MFP_EN + BITEK_MASK_VSYNC_MFP_SEL_I2C_MASTER); //05
	BITEK_TxByte(BITEK_0F2_HSYNC_ATTR, BITEK_MASK_HSYNC_MFP_EN + BITEK_MASK_HSYNC_MFP_SEL_I2C_MASTER); //05
  //BITEK_TxByte(BITEK_0F5_M8051_ATTR1, BITEK_MASK_I2C_M_SYNC); //0x3

	#elif (I2C_MPIN_CFG == I2C_PIN_TOUT)

	// TOUT as I2C Master pin
	BITEK_TxByte(BITEK_0F3_TOUT_ATTR, BITEK_MASK_TOUT21_MFP_EN + BITEK_MASK_TOUT21_MFP_SEL_I2C_MASTER);	//05
  //BITEK_TxByte(BITEK_0F5_M8051_ATTR1, BITEK_MASK_I2C_M_TOUT); //0x4
	#endif

    BITEK_TxByte(BITEK_0F5_M8051_ATTR1, I2C_MSPIN_SEL);

	bI2C_M_State  = eI2C_STATE_MAD;
    bI2C_M_Cnt    = 0;
	bI2C_M_CmdCnt = 0;

    #if 0
	I2C_WRITE_CMD(I2C_CMD_MAD,          I2C_MAD_ADR);
	I2C_WRITE_CMD(I2C_CMD_REG,          0xAA);
    I2C_WRITE_CMD(I2C_CMD_WDATA_END,    0x55);
    #endif

    #if (DEBUG_I2C)
	BITEK_TxByte(BITEK_001_SW_VER, 'M');
    #endif
}
#endif


#if (I2C_MASTER_RXBURST)
/* -------------------------------------------------------------------
    Name: I2C_MASTER_RxBurst -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_MASTER_RxBurst (
UB8 bMAD,       // Module Address
UB8 bReg,
UB8 bCnt,
UB8 *pbData
)
{
	UB8 bIdx;


    // For Write
	I2C_M_WRITE_CMD(I2C_CMD_MAD, bMAD);
	I2C_M_WRITE_CMD(I2C_CMD_REG_END, bReg);

    // For Read
	I2C_M_WRITE_CMD(I2C_CMD_MAD, (bMAD | 0x01));

    for (bIdx = 0; bIdx < (bCnt - 1); bIdx++)
		I2C_M_READ_CMD(I2C_CMD_RDATA, *pbData++);

	I2C_M_READ_CMD(I2C_CMD_RDATA_END, *pbData);
}
#endif


#if (I2C_MASTER_RXBYTE)
/* -------------------------------------------------------------------
    Name: I2C_MASTER_RxByte -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
UB8 I2C_MASTER_RxByte (
UB8 bMAD,       // Module Address
UB8 bReg
)
{
	UB8 bRet;


    // For Write
	I2C_M_WRITE_CMD(I2C_CMD_MAD, bMAD);
	I2C_M_WRITE_CMD(I2C_CMD_REG_END, bReg);

    // For Read
	I2C_M_WRITE_CMD(I2C_CMD_MAD, (bMAD | 0x01));
	I2C_M_READ_CMD(I2C_CMD_RDATA_END, bRet);

	return( bRet );
}
#endif


#if (I2C_MASTER_TXBURST)
/* -------------------------------------------------------------------
    Name: I2C_MASTER_TxBurst -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_MASTER_TxBurst (
UB8 bMAD,       // Module Address
UB8 bReg,
UB8 bCnt, 
UB8 *pbData
)
{
	UB8 bIdx;


    I2C_M_WRITE_CMD(I2C_CMD_MAD, bMAD);
    I2C_M_WRITE_CMD(I2C_CMD_REG, bReg);

    for (bIdx = 0; bIdx < (bCnt - 1); bIdx++)
        I2C_M_WRITE_CMD(I2C_CMD_WDATA, *pbData++);

    I2C_M_WRITE_CMD(I2C_CMD_WDATA_END, *pbData);
}
#endif


#if (I2C_MASTER_TXBYTE)
/* -------------------------------------------------------------------
    Name: I2C_MASTER_TxByte -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_MASTER_TxByte (
UB8 bMAD,       // Module Address
UB8 bReg,
UB8 bData
)
{
	I2C_M_WRITE_CMD(I2C_CMD_MAD,       bMAD);
	I2C_M_WRITE_CMD(I2C_CMD_REG,       bReg);
	I2C_M_WRITE_CMD(I2C_CMD_WDATA_END, bData);
}
#endif


#if (I2C_SLAVE_INIT)
/* -------------------------------------------------------------------
    Name: I2C_SLAVE_Init -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_SLAVE_Init (void)
{
    // Setup I2C Slave MAD
	BITEK_TxByte(BITEK_1AE_I2C_S_MAD , I2C_S_MAD);
	BITEK_TxByte(BITEK_1AF_I2C_S_MASK, I2C_S_MAD_MASK);
  //BITEK_TxByte(BITEK_1B0_I2C_S_TBUF, I2C_ACK_FREE);

    // Enable I2C Slave and Interrupt
	BITEK_TxByte(BITEK_1B1_I2C_S_ATTR, BITEK_MASK_I2C_S_EN + BITEK_MASK_I2C_S_INT);


	#if (I2C_SPIN_CFG == I2C_PIN_RIN)

	BITEK_TxByte(BITEK_0E8_RIN_ATTR, BITEK_MASK_RIN10_MFP_EN + BITEK_MASK_RIN10_MFP_SEL_I2C_SLAVE);	//07

	#elif (I2C_SPIN_CFG == I2C_PIN_GIN)

	BITEK_TxByte(BITEK_0EB_GIN_ATTR, BITEK_MASK_GIN10_MFP_EN + BITEK_MASK_GIN10_MFP_SEL_I2C_SLAVE);	//07

	#elif (I2C_SPIN_CFG == I2C_PIN_BIN)

	BITEK_TxByte(BITEK_0EE_BIN_ATTR, BITEK_MASK_BIN10_MFP_EN + BITEK_MASK_BIN10_MFP_SEL_I2C_SLAVE);	//07

	#elif (I2C_SPIN_CFG == I2C_PIN_SYNC)

	BITEK_TxByte(BITEK_0F1_VSYNC_ATTR, BITEK_MASK_VSYNC_MFP_EN + BITEK_MASK_VSYNC_MFP_SEL_I2C_SLAVE); //07
	BITEK_TxByte(BITEK_0F2_HSYNC_ATTR, BITEK_MASK_HSYNC_MFP_EN + BITEK_MASK_HSYNC_MFP_SEL_I2C_SLAVE); //07

	#elif (I2C_SPIN_CFG == I2C_PIN_TOUT)

	BITEK_TxByte(BITEK_0F3_TOUT_ATTR, BITEK_MASK_TOUT21_MFP_EN + BITEK_MASK_TOUT21_MFP_SEL_I2C_SLAVE);	//07
	
    #endif


    BITEK_TxByte(BITEK_0F5_M8051_ATTR1, I2C_MSPIN_SEL);

	//Set 100K Real= 80k
  //I2C_SLAVE_ClearBfr();
  //I2C_SLAVE_ShowBfr();

	bI2C_S_RCnt = 0;
}
#endif


#if (I2C_SLAVE_HANDLER)
/* -------------------------------------------------------------------
    Name: I2C_SLAVE_Handler -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:

    1) by Paul
        When Write 42 E5 05   ISR Will GET R1B1's Status is 0x53 0x13 0x13!  0xA3     , RBUF is 0x00 0xE5 0x55
        When Read  42 E4      ISR Will GET R1B1's Status is 0x53 0x13 0x73 0xB3! 0xA3 , RBUF is 0x00 0xE4 0xE4 0xE4

        0x53 = START+ BUSY
        0x13 = BUSY
        0x73 = START + BUS + BUSY
        0xB3 = INT   + BUS + BUSY
        0xA3 = INT   + BUS

    2) R009 = BITEK_MASK_INT1_ISR6_SWR (Start+Write+Read)
        2.1) Master -> 0x6E 0x0A -> Slave
            Result: No response

        2.2) Master -> 0x6E 0x06 0x07 -> Slave
            R1B1: 0x53 0x13 0x13
            R1B2: 0xXX 0x06 0x07

        2.3) Master -> 0x6E 0x08 0x09 -> Slave
            R1B1: 0x53 0x13 0x13
            R1B2: 0xXX 0x08 0x09

        2.4) Master -> 0x6E 0x01 0x02 0x03 0x04 -> Slave
            R1B1: 0x53 0x13 0x13 0x13 0x13
            R1B2: 0xXX 0x01 0x02 0x03 0x04

   ------------------------------------------------------------------- */
void I2C_SLAVE_Handler (void)
{
  //UB8 bStatus;


    /* R1B1
    ; 7 6 5 4 3 2 1 0
    ; | | | | | | | |_ I2C_S_EN: I2C Slave Enable (0=Disable, 1=Enable)
    ; | | | |     |___ I2C_S_SINT: I2C Slave MAD Interrupt (0=Disable, 1=Enable)
    ; | | | |
    ; | | | |_________ I2C_S_BUSY: I2C Slave Status (0=No Busy, 1=Busy)
    ; | | |___________ I2C_S_BUSY_RW: I2C R/W  Bit Status (0=Write Mode, 1=Read Mode)
    ; | |_____________ I2C_S_START: I2C Slave MAD same (0=R/W Byte, 1=Start)
    ; |_______________ I2C_S_INT_RW: I2C Slave Status (0=Write Data Byte Mode, 1=Read Data Byte Mode)
    */
  //bStatus = BITEK_RxByte(BITEK_1B1_I2C_S_ATTR);


	#if (DEBUG_I2C)
  	OSD_ShowHex(VP_SHOW_1B1 + bI2C_Idx, BITEK_RxByte(BITEK_1B1_I2C_S_ATTR));
  	OSD_ShowHex(VP_SHOW_1B2 + bI2C_Idx, BITEK_RxByte(BITEK_1B2_I2C_S_RBUF_O));
    bI2C_Idx += 3;
	#endif

}
#endif


#if (I2C_SLAVE_CLEAR_BFR)
/* -------------------------------------------------------------------
    Name: I2C_SLAVE_ClearBfr -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_SLAVE_ClearBfr (void)
{
    UB8 bIdx;


    for (bIdx = 0; bIdx < I2C_S_BUF_LEN; bIdx++)
    	abI2C_S_Bfr[ bIdx ] = 0;

	bI2C_S_WCnt = 0;
}
#endif


#if (I2C_SLAVE_SHOW_BFR)
/* -------------------------------------------------------------------
    Name: I2C_SLAVE_ShowBfr -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_SLAVE_ShowBfr (void)
{
    UB8 bIdx;


    for (bIdx = 0; bIdx < I2C_S_BUF_LEN; bIdx++)
    	OSD_ShowHex(VP_SHOW_BFR + bIdx * 3, abI2C_S_Bfr[bIdx]);
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
