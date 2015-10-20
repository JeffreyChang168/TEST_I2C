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

    Module: UART.H

    Purpose: Interface of UART module.

    Version: 0.01                                   10:26AM  2013/03/17

    Compiler: Keil 8051 C Compiler v9.51

    Reference:
    [1] Keil C51 Compiler User's Guide 01.97, Keil Software, Inc.
    [2] P89C738; P89C739 8-bit UART microcontrollers, 1998 Apr 07
        Philips Semiconductors
    [3] 80C51-Based 8-Bit Microcontrllers, 1998 Philips Semiconductors
    [4] Application Notes and Development Tools for 80C51 Microcontrllers,
        1997 Philips Semiconductors
    [5] P89C51RC+/P89C51RD+ 80C51 8-bit Flash microcontrollers family
        32K/64K ISP FLASH with 512-1K RAM, 1999 Oct 27 Philips Semiconductors
    [6] P89C51RB2/P89C51RC2/P89C51RD2 80C51 8-bit Flash microcontrollers family
        16KB/32KB/64KB ISP/IAP Flash with 512B/512B/1KB RAM, 2001 Jan 11
        Philips Semiconductors
    [7] P89C660/P89C662/P89C664 80C51 8-bit Flash microcontrollers family
        16KB/32KB/64KB ISP/IAP Flash with 512B/1KB/2KB RAM, 2001 Jan 11
        Philips Semiconductors
    [8] P89C738; P89C739 8-bit UART microcontrollers, 1998 Apr 07
        Philips Semiconductors
    [9] W78E52B 8-bit MPT Microcontroller, Dec. 1998,
        Winbond Electronics Corp.
    [10] P89C138; P89C238; P89C838; P83C138; P83C238 Single-chip 8-bit
        microcontrollers, Mar 20, 1998 Philips Semiconductors
    [11] P89C60X2/61X2 80C51 8-bit Flash microcontrollers family
        64KB Flash 512B/1024B RAM, 2002 Apr 15, Philips Semiconductors

   ----------------------------------------------------------------------
    Modification:

    v0.01 10:26AM  2013/03/17 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ********************************************************************** */

#ifndef _UART_H_
#define _UART_H_

/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "bit1618c.h"
#include "bitek.h"
#include "common.h"
#include "mcu.h"
#include "osd.h"


/* ------------------------------------
    Macro Definitions
   ------------------------------------ */
#undef EXTERN

#ifdef _UART_C_
    #define EXTERN
#else
    #define EXTERN  extern
#endif


/*
    BIT1618C + T070SWV056H

    TxD     RxD     Timer
    RIN1    RIN0    TIMER1      OK 10:35AM  2013/03/17
    RIN1    RIN0    TIMER2      OK 10:41AM  2013/03/17
    GIN1    GIN0    TIMER2      OK 11:01AM  2013/03/17
    BIN1    BIN0    TIMER2      OK 11:37AM  2013/03/17
    VSYNC   HSYNC   TIMER2      OK 11:44AM  2013/03/17

*/
/* :::::::::::::::::::::::::
    UART Timer Configuration
   ::::::::::::::::::::::::: */
#define UART_TXD_NONE       0
#define UART_TXD_RIN1       1
#define UART_TXD_GIN1       2
#define UART_TXD_BIN1       3
#define UART_TXD_VSYNC      4
#define UART_TXD_TOUT2      5
  #define UART_TXD_CFG      UART_TXD_RIN1       // OK 10:34AM  2013/03/17
//#define UART_TXD_CFG      UART_TXD_GIN1       // OK 11:01AM  2013/03/17
//#define UART_TXD_CFG      UART_TXD_BIN1       // OK 11:36AM  2013/03/17
//#define UART_TXD_CFG      UART_TXD_VSYNC      // OK 11:43AM  2013/03/17
//#define UART_TXD_CFG      UART_TXD_TOUT2      // T070SWV056H used it as U/D control

#define UART_RXD_NONE       0
#define UART_RXD_RIN0       1
#define UART_RXD_GIN0       2
#define UART_RXD_BIN0       3
#define UART_RXD_HSYNC      4
#define UART_RXD_TOUT1      5
  #define UART_RXD_CFG      UART_RXD_RIN0       // OK 10:34AM  2013/03/17
//#define UART_RXD_CFG      UART_RXD_GIN0       // OK 11:01AM  2013/03/17
//#define UART_RXD_CFG      UART_RXD_BIN0       // OK 11:36AM  2013/03/17
//#define UART_RXD_CFG      UART_RXD_HSYNC      // OK 11:43AM  2013/03/17
//#define UART_RXD_CFG      UART_RXD_TOUT1      // OK 11:51AM  2013/03/17


#define UART_NONE           0
#define UART_TIMER1         1
#define UART_TIMER2         2
//#define UART_TIMER_CFG    UART_NONE           //
  #define UART_TIMER_CFG    UART_TIMER1         // OK 10:34AM  2013/03/17
//#define UART_TIMER_CFG    UART_TIMER2         // OK 10:41AM  2013/03/17


/* :::::::::::::::::::::::::
    UART Interrupt Configuration
   ::::::::::::::::::::::::: */
#define UART_INT            4       /* [2]92 Serial Port Interrupt Number */


/* The received/transmitted buffer size allows to be not 2 power. */
#define RX_BFR_SIZE         10

#define UART_INIT           ON



/* ------------------------------------
    Type Definitions
   ------------------------------------ */


/* ------------------------------------
    Variables Definitions/Declarations
   ------------------------------------ */
EXTERN UB8  bTxData;
EXTERN UB8  bRxData;


/* ------------------------------------
    Function Prototypes
   ------------------------------------ */

#if (UART_INIT)
EXTERN void UART_Init(void);
#endif


#endif /* _UART_H_ */


/* **********************************************************************

    Description:

   ********************************************************************** */

/* %% End Of File %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
