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

    Module: I2C.H -

    Purpose: Interface of I2C.

    Version: 0.02                                   06:23PM  2014/05/29

    Compiler: Keil 8051 C Compiler v9.52

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 01:52PM  2014/05/20 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ----------------------------------------------------------------------
    v0.02 06:23PM  2014/05/29 Jeffrey Chang
    Reason:
        1. To verify Hardware I2C MASTER @RIN0/RIN1 pins.
    Solution:

   ********************************************************************** */

#ifndef _I2C_H_
#define _I2C_H_

/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "common.h"

/* ------------------------------------
    Macro Definitions
   ------------------------------------ */
#undef EXTERN

#ifdef _I2C_C_
    #define EXTERN
#else
    #define EXTERN	extern
#endif


#define I2C_MASTER              ON
#define I2C_SLAVE               OFF


// I2C Master
#define I2C_MASTER_HANDLER      OFF
#define I2C_MASTER_INIT         ON
#define I2C_MASTER_RXBURST      ON
#define I2C_MASTER_RXBYTE       ON
#define I2C_MASTER_TXBURST      ON
#define I2C_MASTER_TXBYTE       ON

// I2C Slave
#define I2C_SLAVE_CLEAR_BFR     OFF
#define I2C_SLAVE_HANDLER       OFF
#define I2C_SLAVE_INIT          OFF
#define I2C_SLAVE_SHOW_BFR      OFF



// I2C Master/Slave Module Address (MAD)
//#define I2C_M_MAD         0x42
//#define I2C_M_MAD         0x6E
  #define I2C_M_MAD         0xA0

//#define I2C_S_MAD			0x42
  #define I2C_S_MAD			0x6E
  #define I2C_S_MAD_MASK    0x00

// I2C Master/Slave Pins
// SCL          SDA
// RIN0         RIN1
// GIN0         GIN1
// BIN0         BIN1
// HSYNC        VSYNC
// TOUT1        TOUT2
#define I2C_PIN_RIN         0               // OK 07:30PM  2013/01/03
#define I2C_PIN_GIN         1               // OK 07:26PM  2013/01/03
#define I2C_PIN_BIN         2               // OK 07:32PM  2013/01/03
#define I2C_PIN_SYNC        3               // OK 07:37PM  2013/01/03
#define I2C_PIN_TOUT        4               // OK 07:52PM  2013/01/03
#define I2C_PIN_NONE_M      5               //
#define I2C_PIN_NONE_S      6               //

// I2C Master Pins
  #define I2C_MPIN_CFG      I2C_PIN_RIN     // I2C Master Pins
//#define I2C_MPIN_CFG      I2C_PIN_GIN     // I2C Master Pins
//#define I2C_MPIN_CFG      I2C_PIN_BIN     // I2C Master Pins
//#define I2C_MPIN_CFG      I2C_PIN_SYNC    // I2C Master Pins
//#define I2C_MPIN_CFG      I2C_PIN_TOUT    // I2C Master Pins
//#define I2C_MPIN_CFG      I2C_PIN_NONE_M  // I2C Master Pins

// I2C Slave Pins (INT0)
//#define I2C_SPIN_CFG      I2C_PIN_RIN     // I2C Slave Pins OK 10:22PM  2014/05/25
//#define I2C_SPIN_CFG      I2C_PIN_GIN     // I2C Slave Pins OK 09:57PM  2014/05/25
//#define I2C_SPIN_CFG      I2C_PIN_BIN     // I2C Slave Pins OK 09:55PM  2014/05/25
//#define I2C_SPIN_CFG      I2C_PIN_SYNC    // I2C Slave Pins OK 09:53PM  2014/05/25
//#define I2C_SPIN_CFG      I2C_PIN_TOUT    // I2C Slave Pins NA
  #define I2C_SPIN_CFG      I2C_PIN_NONE_S  // I2C Slave Pins

// I2C Slave Pins (INT1)
//#define I2C_SPIN_CFG      I2C_PIN_RIN     // I2C Slave Pins OK 10:21PM  2014/05/25
//#define I2C_SPIN_CFG      I2C_PIN_GIN     // I2C Slave Pins OK 09:59PM  2014/05/25
//#define I2C_SPIN_CFG      I2C_PIN_BIN     // I2C Slave Pins OK 09:28PM  2014/05/25
//#define I2C_SPIN_CFG      I2C_PIN_SYNC    // I2C Slave Pins OK 09:31PM  2014/05/25
//#define I2C_SPIN_CFG      I2C_PIN_TOUT    // I2C Slave Pins NA
//#define I2C_SPIN_CFG      I2C_PIN_NONE_S  // I2C Slave Pins


// I2C Master Pin Select (0x0F5[2:0])
#if (I2C_MPIN_CFG == I2C_PIN_RIN)
    #define I2C_MPIN_SEL	BITEK_MASK_I2C_M_RIN
#elif (I2C_MPIN_CFG == I2C_PIN_GIN)
    #define I2C_MPIN_SEL	BITEK_MASK_I2C_M_GIN
#elif (I2C_MPIN_CFG == I2C_PIN_BIN)
    #define I2C_MPIN_SEL	BITEK_MASK_I2C_M_BIN
#elif (I2C_MPIN_CFG == I2C_PIN_SYNC)
    #define I2C_MPIN_SEL	BITEK_MASK_I2C_M_SYNC
#elif (I2C_MPIN_CFG == I2C_PIN_TOUT)
    #define I2C_MPIN_SEL	BITEK_MASK_I2C_M_TOUT
#else
    #define I2C_MPIN_SEL	0
#endif

// I2C Slave Pin Select (0x0F5[5:3])
#if (I2C_SPIN_CFG == I2C_PIN_RIN)
    #define I2C_SPIN_SEL	BITEK_MASK_I2C_S_RIN
#elif (I2C_SPIN_CFG == I2C_PIN_GIN)
    #define I2C_SPIN_SEL	BITEK_MASK_I2C_S_GIN
#elif (I2C_SPIN_CFG == I2C_PIN_BIN)
    #define I2C_SPIN_SEL	BITEK_MASK_I2C_S_BIN
#elif (I2C_SPIN_CFG == I2C_PIN_SYNC)
    #define I2C_SPIN_SEL	BITEK_MASK_I2C_S_SYNC
#elif (I2C_SPIN_CFG == I2C_PIN_TOUT)
    #define I2C_SPIN_SEL	BITEK_MASK_I2C_S_TOUT
#else
    #define I2C_SPIN_SEL	0
#endif

// (0x0F5[5:0])
#define I2C_MSPIN_SEL	    (I2C_MPIN_SEL + I2C_SPIN_SEL)


#if (I2C_SPIN_CFG == I2C_MPIN_CFG)
    #error "I2C Master and Slave same pins !"
#endif



#if 0
// I2C Module Control
#define ENABLE_I2C			 1
#define ENABLE_I2C_DEBUG	 0

#define ENABLE_I2C_MASTER	 0
#define ENABLE_I2C_M_DEBUG   0

#define ENABLE_I2C_M_WTEST	 0
#define ENABLE_I2C_M_RTEST	 0

#define ENABLE_I2C_SLAVE	 1
#define ENABLE_I2C_S_DEBUG	 1
#define ENABLE_I2C_S_POLLING 0


#define ENABLE_IAP_DEBUG  	 0
#endif


/* ------------------------------------
    Type Definitions
   ------------------------------------ */
typedef enum I2C_STATE_ENUM
{
    eI2C_STATE_IDLE,
    eI2C_STATE_MAD,
    eI2C_STATE_MAD_ACK,
    eI2C_STATE_REG,
    eI2C_STATE_REG_ACK,

    eI2C_STATE_MAD_READ,
    eI2C_STATE_MAD_READ_ACK,

    eI2C_STATE_DATA         = eI2C_STATE_MAD_READ,
    eI2C_STATE_DATA_ACK     = eI2C_STATE_MAD_READ_ACK,

    eI2C_STATE_DATA_END,
    eI2C_STATE_DATA_END_ACK,
    eI2C_STATE_END,
    eI2C_STATE_SEND,
    eI2C_STATE_ACK,
} enumI2C_STATE;



typedef enum I2C_COMMAND_FORMAT_ENUM
{
	eI2C_TAG,
	eI2C_CMD,
	eI2C_LEN,
	eI2C_DATA0,
	eI2C_DATA1,
	eI2C_CHKSUM,
	eI2C_END,
	eI2C_B_TAG,
	eI2C_B_CMD,
	eI2C_B_LEN,
	eI2C_B_DATA0,
	eI2C_B_DATA1,
	eI2C_B_CHKSUM,
	eI2C_B_END,
	eI2C_FMT_SIZE,
} enumI2C_COMMAND_FORMAT;




#if 1
//
#define I2C_MAD_ADR_MASK	0x00

//
#define I2C_PKT_LEN         0x07
#define I2C_CMD_END         0x11
#define I2C_CMD_LEN         0x03
#define I2C_ACK_FREE        0x80
#define I2C_ACK_BUSY        0x88
#define I2C_CMD_TAG         0xE5
#define I2C_END_TAG         0xE4
#define I2C_INT_ACK         0x40
#define I2C_USE_INT         0x00
#define I2C_CMD_TIMER       0x2E

#endif

#define I2C_S_BUF_LEN           eI2C_FMT_SIZE


/* ------------------------------------
    Variables Definitions/Declarations
   ------------------------------------ */
#if (I2C_MPIN_CFG != I2C_PIN_NONE_M)
EXTERN UB8  bI2C_M_State;
EXTERN UB8	bI2C_M_Cnt;
EXTERN UB8	bI2C_M_CmdCnt;
#endif

#if (I2C_SPIN_CFG != I2C_PIN_NONE_S)
EXTERN UB8 	bI2C_S_WCnt;
EXTERN UB8 	bI2C_S_RCnt;
EXTERN UB8  abI2C_S_Bfr[I2C_S_BUF_LEN];
#endif

#if (DEBUG_I2C)
EXTERN UB8 	bI2C_Idx;
#endif

/* ------------------------------------
    Function Prototypes
   ------------------------------------ */
void I2C_Init(void);

#if (I2C_MASTER_HANDLER)
void I2C_MASTER_Handler(void);
#endif

#if (I2C_MASTER_INIT)
void I2C_MASTER_Init(void);
#endif

#if (I2C_MASTER_RXBURST)
void I2C_MASTER_RxBurst(UB8 bMAD, UB8 bReg, UB8 bCnt, UB8 *pbData);
#endif

#if (I2C_MASTER_RXBYTE)
UB8 I2C_MASTER_RxByte(UB8 bMAD, UB8 bReg);
#endif

#if (I2C_MASTER_TXBURST)
void I2C_MASTER_TxBurst(UB8 bMAD, UB8 bReg, UB8 bCnt, UB8 *pbData);
#endif

#if (I2C_MASTER_TXBYTE)
void I2C_MASTER_TxByte(UB8 bMAD, UB8 bReg, UB8 bData);
#endif

#if (I2C_SLAVE_INIT)
void I2C_SLAVE_Init(void);
#endif

#if (I2C_SLAVE_HANDLER)
void I2C_SLAVE_Handler(void);
#endif

#if (DEBUG_I2C)
void I2C_SLAVE_ShowBfr(void);
#endif

void I2C_SLAVE_ClearBfr(void);

#endif /* _I2C_H_ */

/* **********************************************************************

    Description:

   ********************************************************************** */

/* %% End Of File %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
