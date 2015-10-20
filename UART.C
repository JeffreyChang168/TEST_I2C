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

    Module: UART.C

    Purpose: Implementation of UART module.

    Version: 0.02                                   10:37AM  2013/09/24

    Compiler: Keil 8051 C Compiler v9.52

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

    v0.01 10:25AM  2013/03/17 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ----------------------------------------------------------------------
    v0.02 10:37AM  2013/09/24 Jeffrey Chang
    Reason:
        1. To fix the TOUT setting bug.
    Solution:

   ********************************************************************** */

#define _UART_C_

/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "common.h"
#include "mcu.h"
#include "timer.h"
#include "uart.h"


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

#if (UART_INIT)
/* -------------------------------------------------------------------
    Name: UART_Init -
    Purpose: To initialize UART module.
    Passed: None.
    Returns: None.
    Notes:

   ------------------------------------------------------------------- */
void UART_Init (void)
{
    #if (UART_TXD_CFG == UART_TXD_RIN1)
    {
        UB8 b0E8;


        b0E8 = BITEK_RxByte(BITEK_0E8_RIN_ATTR) & ~BITEK_MASK_RIN10_MFP_SEL;

        BITEK_TxByte(BITEK_0E8_RIN_ATTR, b0E8 | BITEK_MASK_RIN10_MFP_SEL_UART_TXD | BITEK_MASK_RIN10_MFP_EN);
    }
    #elif (UART_TXD_CFG == UART_TXD_GIN1)
    // GIN1 as UART_TxD
    // 1) GIN10_MFP_EN (0xEB[0]) = 1
    // 2) GIN10_MFP_SEL(0xEB[2:1]) = 01
    {
        UB8 b0EB;


        b0EB = BITEK_RxByte(BITEK_0EB_GIN_ATTR) & ~BITEK_MASK_GIN10_MFP_SEL;

        BITEK_TxByte(BITEK_0EB_GIN_ATTR, b0EB | BITEK_MASK_GIN10_MFP_SEL_UART_TXD | BITEK_MASK_GIN10_MFP_EN);
    }
    #elif (UART_TXD_CFG == UART_TXD_BIN1)
    {
        UB8 b0EE;


        b0EE = BITEK_RxByte(BITEK_0EE_BIN_ATTR) & ~BITEK_MASK_BIN10_MFP_SEL;

        BITEK_TxByte(BITEK_0EE_BIN_ATTR, b0EE | BITEK_MASK_BIN10_MFP_SEL_UART_TXD | BITEK_MASK_BIN10_MFP_EN);
    }
    #elif (UART_TXD_CFG == UART_TXD_VSYNC)
    {
        UB8 b0F1;


        b0F1 = BITEK_RxByte(BITEK_0F1_VSYNC_ATTR) & ~BITEK_MASK_VSYNC_MFP_SEL_UART_TXD;

        BITEK_TxByte(BITEK_0F1_VSYNC_ATTR, b0F1 | BITEK_MASK_VSYNC_MFP_SEL_UART_TXD | BITEK_MASK_VSYNC_MFP_EN);
    }
    #elif (UART_TXD_CFG == UART_TXD_TOUT2)
    {
        UB8 b0F3;


        b0F3 = BITEK_RxByte(BITEK_0F3_TOUT_ATTR) & ~BITEK_MASK_TOUT21_MFP_SEL;

        BITEK_TxByte(BITEK_0F3_TOUT_ATTR, b0F3 | BITEK_MASK_TOUT21_MFP_SEL_UART_TXD | BITEK_MASK_TOUT21_MFP_EN);
    }
    #endif


    // UART_RxD (0x0F6[5:3]): 000=RIN0, 001=GIN0, 010=BIN0, 011=HSYNC, 100=TOUT1
    // BIN0 as UART_RxD
    // 1) UART_SRC (0x0F6[5:3]) = 010
    // 2) BIN10_MFP_EN (0xEE[0]) = 0
    // 3) BIN_GPIO (0xEE[7]) = 0

    #if (UART_RXD_CFG == UART_RXD_RIN0)
    {
        UB8 b0F6;
      //UB8 b0E8;


        // UART_SRC (0x0F6[5:3]) = 000 = RIN0
        b0F6 = BITEK_RxByte(BITEK_0F6_M8051_ATTR2) & ~BITEK_MASK_UART_SRC;
        BITEK_TxByte(BITEK_0F6_M8051_ATTR2, b0F6 | BITEK_MASK_UART_SRC_RIN0);

        // RIN10_MFP_EN (0xE8[0]) = 0
        // RIN_GPIO (0xE8[7]) = 0
      //b0E8 = BITEK_RxByte(BITEK_0E8_RIN_ATTR) & ~(BITEK_MASK_RIN_GPIO | BITEK_MASK_RIN10_MFP_EN);
      //BITEK_TxByte(BITEK_0E8_RIN_ATTR, b0E8);
    }
    #elif (UART_RXD_CFG == UART_RXD_GIN0)
    {
        UB8 b0F6;
      //UB8 b0EB;


        // UART_SRC (0x0F6[5:3]) = 001 = GIN0
        b0F6 = BITEK_RxByte(BITEK_0F6_M8051_ATTR2) & ~BITEK_MASK_UART_SRC;
        BITEK_TxByte(BITEK_0F6_M8051_ATTR2, b0F6 | BITEK_MASK_UART_SRC_GIN0);

        // GIN10_MFP_EN (0xEB[0]) = 0
        // GIN_GPIO (0xEB[7]) = 0
      //b0EB = BITEK_RxByte(BITEK_0EB_GIN_ATTR) & ~(BITEK_MASK_GIN_GPIO | BITEK_MASK_GIN10_MFP_EN);
      //BITEK_TxByte(BITEK_0EB_GIN_ATTR, b0EB);
    }
    #elif (UART_RXD_CFG == UART_RXD_BIN0)
    {
        UB8 b0F6;
      //UB8 b0EE;


        // UART_SRC (0x0F6[5:3]) = 010 = BIN0
        b0F6 = BITEK_RxByte(BITEK_0F6_M8051_ATTR2) & ~BITEK_MASK_UART_SRC;
        BITEK_TxByte(BITEK_0F6_M8051_ATTR2, b0F6 | BITEK_MASK_UART_SRC_BIN0);

        // BIN10_MFP_EN (0xEE[0]) = 0
        // BIN_GPIO (0xEE[7]) = 0
      //b0EE = BITEK_RxByte(BITEK_0EE_BIN_ATTR) & ~(BITEK_MASK_BIN_GPIO | BITEK_MASK_BIN10_MFP_EN);
      //BITEK_TxByte(BITEK_0EE_BIN_ATTR, b0EE);
    }
    #elif (UART_RXD_CFG == UART_RXD_HSYNC)
    {
        UB8 b0F6;
        UB8 b0F2;


        // UART_SRC (0x0F6[5:3]) = 011 = HSYNC
        b0F6 = BITEK_RxByte(BITEK_0F6_M8051_ATTR2) & ~BITEK_MASK_UART_SRC;
        BITEK_TxByte(BITEK_0F6_M8051_ATTR2, b0F6 | BITEK_MASK_UART_SRC_HSYNC);

        // HSYNC_MFP_EN (0xF2[0]) = 0
        // HSYNC_GPIO (0xF2[3]) = 0
        b0F2 = BITEK_RxByte(BITEK_0F2_HSYNC_ATTR) & ~(BITEK_MASK_HSYNC_GPIO | BITEK_MASK_HSYNC_MFP_EN);
        BITEK_TxByte(BITEK_0F2_HSYNC_ATTR, b0F2);
    }
    #elif (UART_RXD_CFG == UART_RXD_TOUT1)
    {
        UB8 b0F6;
        UB8 b0F3;
        UB8 b0F4;
        UB8 b0FE;


        // UART_SRC (0x0F6[5:3]) = 100 = TOUT1
        b0F6 = BITEK_RxByte(BITEK_0F6_M8051_ATTR2) & ~BITEK_MASK_UART_SRC;
        BITEK_TxByte(BITEK_0F6_M8051_ATTR2, b0F6 | BITEK_MASK_UART_SRC_TOUT1);

        // TOUT21_MFP_EN (0xF3[0]) = 0
      //b0F3 = BITEK_RxByte(BITEK_0F3_TOUT_ATTR) & ~BITEK_MASK_TOUT21_MFP_EN;
      //BITEK_TxByte(BITEK_0F3_TOUT_ATTR, b0F3);
        b0F3 = BITEK_RxByte(BITEK_0F3_TOUT_ATTR) & ~BITEK_MASK_TOUT21_MFP_SEL;
        BITEK_TxByte(BITEK_0F3_TOUT_ATTR, b0F3 | BITEK_MASK_TOUT21_MFP_SEL_UART_TXD | BITEK_MASK_TOUT21_MFP_EN);

        // TOUT_GPIO (0xF4[1]) = 0
      //b0F4 = BITEK_RxByte(BITEK_0F4_TOUT_GPIO) & ~BITEK_MASK_TOUT_GPIO1;
      //BITEK_TxByte(BITEK_0F4_TOUT_GPIO, b0F4);

        // TOUT_TRI  (0xFE[3]) = 1
      //b0FE = BITEK_RxByte(BITEK_0FE_PORT_ATTR) | BITEK_MASK_TOUT_TRI;
      //BITEK_TxByte(BITEK_0FE_PORT_ATTR, b0FE);
	}
    #endif



    SCON    = 0x50; // [3]67,92 [6]17 Serial Port Control register
                    // Mode 1 (8-bit UART, Variable Baud Rate)
                    // [5]13 Timer 2 in Baud Rate Generator Mode
    PCON    = 0xC0; // [3]77 Baud rate is double (SMOD = 1)

    #if (PLATFORM_UART)
    bTxData = 'A';
    #endif
} /* UART_Init */


/* -------------------------------------------------------------------
    Name: UART_ISR -
    Purpose: Serial Port Interrupt Service Routine (ISR).
    Passed: None
    Returns: None.
    Notes:
    Reference: [2]19
   ------------------------------------------------------------------- */
void UART_ISR (void) interrupt UART_INT         /* Serial Port */
{
    /* /// Tx /// */
    if (TI)
    {
        SBUF = bTxData;

        bTxData++;
        if (bTxData > 'Z')
            bTxData = 'A';

        TI = 0;             // [3]67
    } /* TI */

    /* /// Rx /// */
    if (RI)
    {
        BITEK_TxByte(VP_SHOW_UART_RXD, SBUF);

        RI = 0;             // [3]67
    } /* RI */
} /* UART_ISR */
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

        Since this discussion assumes the use of a standard UART for RS-232
    serial communications, the UART will be used in modes 1 or 3 (variable
    baud rates) and timer 1 will be used in mode 2 (8-bit auto-reload mode)
    as the baud rate generator.

    1) The basic equation for a timer reload value can be stated as:
        if SMOD = 0
            TH1 = 256 - (Crystal Frequency / 384) / Baud Rate

        or,
        if SMOD = 1
            TH1 = 256 - (Crystal Frequency / 192) / Baud Rate

    2) The equation can also be solved to derive the baud rate or the crystal
        frequency from the other information as follows:

        if SMOD = 0
            Baud Rate = Crystal Frequency / 384 / (256 - TH1)

        or,
        if SMOD = 1
            Baud Rate = Crystal Frequency / 192 / (256 - TH1)

    3)  In the 80C51, the baud rates in Modes 1 and 3 are determined by
        Timer 1 overflow rate.

                      SMOD
                    2     * (Timer 1 Overflow Rate)
        Baud Rate = -------------------------------------
                    32


        When Timer 1 is used as the baud rate generator, the baud rates in
        Modes 1 and 3 are determined by the Timer 1 overflow rate and the value
        of SMOD as follows:

                      SMOD
                    2     * (Oscillator frequency)
        Baud Rate = -------------------------------------
                    32 * 12 * [256 - (TH1)]



        3.1) P89C738 @XTAL= 14.7456 MHz
            Fclk = 14.7456 MHz

            3.1.1) SMOD = 0
                3.1.1.1) Baud Rate = 19200

                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 14745600 / 384 / (256 - TH1)
                              = 19200

                    256 - TH1 = 14745600 / 384 / 19200
                              = 2

                3.1.1.2) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 14745600 / 384 / (256 - TH1)
                              = 57600

                    256 - TH1 = 14745600 / 384 / 57600
                              = 2 / 3
                              = 0.66666666667           (Fail !)

                3.1.1.3) Baud Rate = 76800

                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 14745600 / 384 / (256 - TH1)
                              = 76800

                    256 - TH1 = 14745600 / 384 / 19200
                              = 2 / 4
                              = 0.5                     (Fail !)

            3.1.2) SMOD = 1
                3.1.2.1) Baud Rate = 19200

                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 14745600 / 192 / (256 - TH1)
                              = 19200

                    256 - TH1 = 14745600 / 192 / 19200
                              = 4

                3.1.2.2) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 14745600 / 192 / (256 - TH1)
                              = 57600

                    256 - TH1 = 14745600 / 192 / 57600
                              = 4 / 3
                              = 1.3333333333            (Fail !)

                3.1.2.3) Baud Rate = 76800

                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 14745600 / 192 / (256 - TH1)
                              = 76800

                    256 - TH1 = 14745600 / 192 / 19200
                              = 4 / 4
                              = 1

        3.2) P89C738 @XTAL= 18.4320 MHz
            Fclk = 18.4320 MHz


            3.2.1) SMOD = 0
                3.2.1.1) Baud Rate = 19200
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 18432000 / 384 / (256 - TH1)
                              = 19200

                    256 - TH1 = 18432000 / 384 / 19200
                              = 2.5                 (Fail !)

                3.2.1.2) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 18432000 / 384 / (256 - TH1)
                              = 57600

                    256 - TH1 = 18432000 / 384 / 57600
                              = 2.5 / 3
                              = 0.83333333          (Fail !)

                3.2.1.3) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 18432000 / 384 / (256 - TH1)
                              = 76800

                    256 - TH1 = 18432000 / 384 / 76800
                              = 2.5 / 4
                              = 0.625               (Fail !)

            3.2.1) SMOD = 1
                3.2.1.1) Baud Rate = 19200
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 18432000 / 192 / (256 - TH1)
                              = 19200

                    256 - TH1 = 18432000 / 192 / 19200
                              = 5

                3.2.1.2) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 18432000 / 192 / (256 - TH1)
                              = 57600

                    256 - TH1 = 18432000 / 192 / 57600
                              = 5 / 3
                              = 1.666666667         (Fail !)

                3.2.1.3) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 18432000 / 192 / (256 - TH1)
                              = 76800

                    256 - TH1 = 18432000 / 192 / 76800
                              = 5 / 4
                              = 1.25                (Fail !)


        3.3) P89C738 @XTAL= 22.1184 MHz
            Fclk = 22.1184 MHz

            3.3.1) SMOD = 0

                3.3.1.1) Baud Rate = 19200,
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 22118400 / 384 / (256 - TH1)
                              = 19200

                    256 - TH1 = 22118400 / 384 / 19200
                              = 3

                3.3.1.2) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 22118400 / 384 / (256 - TH1)
                              = 57600

                    256 - TH1 = 22118400 / 384 / 57600
                              = 3 / 3
                              = 1

                3.3.1.3) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 22118400 / 384 / (256 - TH1)
                              = 76800

                    256 - TH1 = 22118400 / 384 / 76800
                              = 3 / 4
                              = 0.75        (Fail !)

            3.3.2) SMOD = 1

                3.3.2.1) Baud Rate = 19200
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 19200

                    256 - TH1 = 22118400 / 192 / 19200
                              = 6

                3.3.2.2) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 57600

                    256 - TH1 = 22118400 / 192 / 57600
                              = 6 / 3
                              = 2

                3.3.2.3) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 76800

                    256 - TH1 = 22118400 / 192 / 76800
                              = 6 / 3
                              = 1.5         (Fail !)


        3.4) P89C51Rx2 @XTAL= 22.1184 MHz
            Fclk = 22.1184 MHz

            3.4.2) 6 clock mode

                         SMOD
                        2     * (Oscillator frequency)
            Baud Rate = -------------------------------------
                        32 * 6 * [256 - (TH1)]

            3.4.2.1) SMOD = 0

                                Oscillator frequency
                Baud Rate = -------------------------------------
                            32 * 6 * [256 - (TH1)]

                3.4.2.1.1) Baud Rate = 9600,
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 9600

                    256 - TH1 = 22118400 / 192 / 9600
                              = 12
                    TH1 = 244

                3.4.2.1.2) Baud Rate = 19200,
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 19200

                    256 - TH1 = 22118400 / 192 / 19200
                              = 6
                    TH1 = 250

                3.4.2.1.3) Baud Rate = 38400,
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 38400

                    256 - TH1 = 22118400 / 192 / 38400
                              = 3

                3.4.2.1.4) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 57600

                    256 - TH1 = 22118400 / 192 / 57600
                              = 2

                    TH1 = 254

                3.4.2.1.5) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 76800

                    256 - TH1 = 22118400 / 192 / 76800
                              = 3 / 2
                              = 1.5     (Fail !)


            3.4.2.2) SMOD = 1

                                2 * Oscillator frequency
                Baud Rate = -------------------------------------
                            32 * 6 * [256 - (TH1)]

                3.4.2.2.1) Baud Rate = 9600
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 22118400 / 96 / (256 - TH1)
                              = 9600

                    256 - TH1 = 22118400 / 96 / 9600
                              = 24
                    TH1 = 232

                3.4.2.2.2) Baud Rate = 19200
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 22118400 / 96 / (256 - TH1)
                              = 19200

                    256 - TH1 = 22118400 / 96 / 19200
                              = 12
                    TH1 = 244

                3.4.2.2.3) Baud Rate = 38400
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 22118400 / 96 / (256 - TH1)
                              = 38400

                    256 - TH1 = 22118400 / 96 / 38400
                              = 6
                    TH1 = 250

                3.4.2.2.4) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 22118400 / 96 / (256 - TH1)
                              = 57600

                    256 - TH1 = 22118400 / 96 / 57600
                              = 4
                    TH1 = 252

                3.4.2.2.5) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 22118400 / 96 / (256 - TH1)
                              = 76800

                    256 - TH1 = 22118400 / 96 / 76800
                              = 3
                    TH1 = 253

            3.4.2) 12 clock mode

                         SMOD
                        2     * (Oscillator frequency)
            Baud Rate = -------------------------------------
                        32 * 12 * [256 - (TH1)]

            3.4.2.1) SMOD = 0

                                Oscillator frequency
                Baud Rate = -------------------------------------
                            32 * 12 * [256 - (TH1)]

                3.4.2.1.1) Baud Rate = 9600,
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 22118400 / 384 / (256 - TH1)
                              = 9600

                    256 - TH1 = 22118400 / 384 / 9600
                              = 6
                    TH1 = 250

                3.4.2.1.2) Baud Rate = 19200,
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 22118400 / 384 / (256 - TH1)
                              = 19200

                    256 - TH1 = 22118400 / 384 / 19200
                              = 3
                    TH1 = 253

                3.4.2.1.3) Baud Rate = 38400,
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 22118400 / 384 / (256 - TH1)
                              = 38400

                    256 - TH1 = 22118400 / 384 / 38400
                              = 1.5         (Fail !)

                3.4.2.1.4) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 22118400 / 384 / (256 - TH1)
                              = 57600

                    256 - TH1 = 22118400 / 384 / 57600
                              = 3 / 3
                              = 1
                    TH1 = 255

                3.4.2.1.5) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 22118400 / 384 / (256 - TH1)
                              = 76800

                    256 - TH1 = 22118400 / 384 / 76800
                              = 3 / 4
                              = 0.75        (Fail !)


            3.4.2.2) SMOD = 1

                                2 * Oscillator frequency
                Baud Rate = -------------------------------------
                            32 * 12 * [256 - (TH1)]

                3.4.2.2.1) Baud Rate = 9600
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 9600

                    256 - TH1 = 22118400 / 192 / 9600
                              = 12
                    TH1 = 244

                3.4.2.2.2) Baud Rate = 19200
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 19200

                    256 - TH1 = 22118400 / 192 / 19200
                              = 6
                    TH1 = 250

                3.4.2.2.3) Baud Rate = 38400
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 38400

                    256 - TH1 = 22118400 / 192 / 38400
                              = 3
                    TH1 = 253

                3.4.2.2.4) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 57600

                    256 - TH1 = 22118400 / 192 / 57600
                              = 6 / 3
                              = 2
                    TH1 = 254

                3.4.2.2.5) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 76800

                    256 - TH1 = 22118400 / 192 / 76800
                              = 6 / 3
                              = 1.5         (Fail !)

    4) Use Timer 2 as baud rate generator

        4.1) P89C738 @XTAL= 14.7456 MHz
            Fclk = 14.7456 MHz

                                 Fclk
            Baud Rate = --------------------------------------
                        32 * (65536 - (RCAP2H * 256 + RCAP2L))

            4.1.1) Baud = 19200
                65536 - (RCAP2H * 256 + RCAP2L) = 14.7456 MHz / 19200 / 32
                                                = 24

                (RCAP2H * 256 + RCAP2L) = 65506 = 0xFFE2

            4.1.2) Baud = 57600
                65536 - (RCAP2H * 256 + RCAP2L) = 14.7456 MHz / 57600 / 32
                                                = 8
                (RCAP2H * 256 + RCAP2L) = 65526 = 0xFFF6

            4.1.3) Baud = 76800
                65536 - (RCAP2H * 256 + RCAP2L) = 14.7456 MHz / 76800 / 32
                                                = 6

        4.2) P89C738 @XTAL= 18.4320 MHz
            Fclk = 18.4320 MHz

                                 Fclk
            Baud Rate = --------------------------------------
                        32 * (65536 - (RCAP2H * 256 + RCAP2L))

            4.2.1) Baud = 19200
                65536 - (RCAP2H * 256 + RCAP2L) = 18.432 MHz / 19200 / 32
                                                = 30

                (RCAP2H * 256 + RCAP2L) = 65506 = 0xFFE2

            4.2.2) Baud = 57600
                65536 - (RCAP2H * 256 + RCAP2L) = 18.432 MHz / 57600 / 32
                                                = 10
                (RCAP2H * 256 + RCAP2L) = 65526 = 0xFFF6

            4.2.1) Baud = 76800
                65536 - (RCAP2H * 256 + RCAP2L) = 18.432 MHz / 76800 / 32
                                                = 7.5       (Fail !)

        4.3) P89C738 @XTAL= 22.1184 MHz
            Fclk = 22.1184 MHz

                                 Fclk
            Baud Rate = --------------------------------------
                        32 * (65536 - (RCAP2H * 256 + RCAP2L))

            4.3.1) Baud = 19200
                65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 19200 / 32
                                                = 36
                (RCAP2H * 256 + RCAP2L) = 65500 = 0xFFDC

            4.3.2) Baud = 38400
                65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 38400 / 32
                                                = 18
                (RCAP2H * 256 + RCAP2L) = 65518 = 0xFFEE

            4.3.3) Baud = 57600
                65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 57600 / 32
                                                = 12
                (RCAP2H * 256 + RCAP2L) = 65524 = 0xFFF4

            4.3.4) Baud = 76800
                65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 76800 / 32
                                                = 9
                (RCAP2H * 256 + RCAP2L) = 65527 = 0xFFF7

        4.4) P89C51Rx2
            Fclk = 22.1184 MHz

                                 Fclk
            Baud Rate = --------------------------------------
                        n * (65536 - (RCAP2H * 256 + RCAP2L))

            where
                n = 16 in 6 clock mode
                  = 32 in 12 clock mode

            4.4.1) 6 clock mode
                4.4.1.1) Baud = 9600
                    65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 9600 / 16
                                                    = 144
                    (RCAP2H * 256 + RCAP2L) = 65392 = 0xFF70

                4.4.1.2) Baud = 19200
                    65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 19200 / 16
                                                    = 72
                    (RCAP2H * 256 + RCAP2L) = 65464 = 0xFFB8

                4.4.1.3) Baud = 38400
                    65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 38400 / 16
                                                    = 36
                    (RCAP2H * 256 + RCAP2L) = 65500 = 0xFFDC

                4.4.1.4) Baud = 57600
                    65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 57600 / 16
                                                    = 24
                    (RCAP2H * 256 + RCAP2L) = 65512 = 0xFFE8

                4.4.1.5) Baud = 76800
                    65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 76800 / 16
                                                    = 18
                    (RCAP2H * 256 + RCAP2L) = 65518 = 0xFFEE

            4.4.2) 12 clock mode
                4.4.2.1) Baud = 9600
                    65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 9600 / 32
                                                    = 72
                    (RCAP2H * 256 + RCAP2L) = 65464 = 0xFFB8

                4.4.2.2) Baud = 19200
                    65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 19200 / 32
                                                    = 36
                    (RCAP2H * 256 + RCAP2L) = 65500 = 0xFFDC

                4.4.2.3) Baud = 38400
                    65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 38400 / 32
                                                    = 18
                    (RCAP2H * 256 + RCAP2L) = 65518 = 0xFFEE

                4.4.2.4) Baud = 57600
                    65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 57600 / 32
                                                    = 12
                    (RCAP2H * 256 + RCAP2L) = 65524 = 0xFFF4

                4.4.2.5) Baud = 76800
                    65536 - (RCAP2H * 256 + RCAP2L) = 22.1184 MHz / 76800 / 32
                                                    = 9
                    (RCAP2H * 256 + RCAP2L) = 65527 = 0xFFF7

        4.5) P89C61x2
            Fclk = 14.318 MHz

                                 Fclk
            Baud Rate = --------------------------------------
                        n * (65536 - (RCAP2H * 256 + RCAP2L))

            where
                n = 16 in 6 clock mode
                  = 32 in 12 clock mode

            4.5.1) 6 clock mode
                4.5.1.1) Baud = 9600
                    65536 - (RCAP2H * 256 + RCAP2L) = 14.318 MHz / 9600 / 16
                                                    = 93.21614
                    (RCAP2H * 256 + RCAP2L) = 65536 - 93 = 65443 = 0xFFA3

                4.5.1.2) Baud = 19200
                    65536 - (RCAP2H * 256 + RCAP2L) = 14.318 MHz / 19200 / 16
                                                    = 46.60807
                    (RCAP2H * 256 + RCAP2L) = 65536 - 47 = 65489 = 0xFFD1

                4.5.1.3) Baud = 38400
                    65536 - (RCAP2H * 256 + RCAP2L) = 14.318 MHz / 38400 / 16
                                                    = 23.30403
                    (RCAP2H * 256 + RCAP2L) = 65536 - 23 = 65513 = 0xFFE9

                4.5.1.4) Baud = 57600
                    65536 - (RCAP2H * 256 + RCAP2L) = 14.318 MHz / 57600 / 16
                                                    = 15.53602
                    (RCAP2H * 256 + RCAP2L) = 65536 - 16 = 65520 = 0xFFF0

                4.5.1.5) Baud = 76800
                    65536 - (RCAP2H * 256 + RCAP2L) = 14.318 MHz / 76800 / 16
                                                    = 11.652018
                    (RCAP2H * 256 + RCAP2L) = 65536 - 12 = 65524 = 0xFFF4

            4.5.2) 12 clock mode
                4.5.2.1) Baud = 9600
                    65536 - (RCAP2H * 256 + RCAP2L) = 14.318 MHz / 9600 / 32
                                                    = 46.60807
                    (RCAP2H * 256 + RCAP2L) = 65536 - 47 = 65489 = 0xFFD1

                4.5.2.2) Baud = 19200
                    65536 - (RCAP2H * 256 + RCAP2L) = 14.318 MHz / 19200 / 32
                                                    = 23.30403
                    (RCAP2H * 256 + RCAP2L) = 65536 - 23 = 65513 = 0xFFE9

                4.5.2.3) Baud = 38400
                    65536 - (RCAP2H * 256 + RCAP2L) = 14.318 MHz / 38400 / 32
                                                    = 11.652018
                    (RCAP2H * 256 + RCAP2L) = 65536 - 12 = 65524 = 0xFFF4

                4.5.2.4) Baud = 57600
                    65536 - (RCAP2H * 256 + RCAP2L) = 14.3184 MHz / 57600 / 32
                                                    = 7.768012
                    (RCAP2H * 256 + RCAP2L) = 65536 - 8 = 65528 = 0xFFF8

                4.5.2.5) Baud = 76800
                    65536 - (RCAP2H * 256 + RCAP2L) = 14.318 MHz / 76800 / 32
                                                    = 5.826009
                    (RCAP2H * 256 + RCAP2L) = 65536 - 6 = 65530 = 0xFFFA

    5)
        5.1) XTAL = 14.7456 MHz

            Baud                9600    19200   38400   57600   76800
            Timer 1, SMOD=0             OK              Fail    Fail
            Timer 1, SMOD=1             OK              Fail    OK
            Timer 2                     OK              OK      OK

        5.2) XTAL = 18.4320 MHz

            Baud                9600    19200   38400   57600   76800
            Timer 1, SMOD=0             OK              OK      Fail
            Timer 1, SMOD=1             OK              OK      Fail
            Timer 2                     OK              OK      Fail

        5.3) XTAL = 22.1184 MHz

            Baud                9600    19200   38400   57600   76800
            Timer 1, SMOD=0     OK      OK      Fail    OK      Fail
            Timer 1, SMOD=1     OK      OK      OK      OK      Fail
            Timer 2 (6x mode)   OK      OK      OK      OK      OK
            Timer 2 (12x mode)  OK      OK      OK      OK      OK


    6) [11]28 P89C61x2
        When Timer 1 is used as the baud rate generator (T2CON.RCLK = 0,
        T2CON.TCLK = 0), the baud rates in Modes 1 and 3 are determined by
        the Timer 1 overflow rate and the value of SMOD as follows:

        Mode 1,3
                      SMOD
                    2     * (Timer 1 Overflow Rate)
        Baud Rate = -------------------------------------
                    n

        where
            n = 32 in 12-clock mode, 16 in 6-clock mode

        The Time 1 interrupt should be disabled in this application. The Timer
        itself can be configured for either "timer" or "counter" operation, and
        in any of its 3 running modes. In the most typical applications, it is
        configured for "timer" operation, in the auto-reload mode (high nibble
        of TMOD = 0010B). In that case the baud rate is given by the formula:


        Mode 1,3
                      SMOD
                    2     * (Oscillator frequency)
        Baud Rate = -------------------------------------
                    n * 12 * [256 - (TH1)]

        where
            n = 32 in 12-clock mode, 16 in 6-clock mode


        6.1) 6 clocks mode

                        SMOD
                       2     * (Oscillator frequency)
           Baud Rate = -------------------------------------
                       16 * 12 * [256 - (TH1)]

            6.1.1) SMOD = 0

                               Oscillator frequency
                Baud Rate = -------------------------------------
                            16 * 12 * [256 - (TH1)]

                6.1.1.1) Baud Rate = 9600,
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                         = 14318000 / 192 / (256 - TH1)
                         = 9600

                    256 - TH1 = 14318000 / 192 / 9600
                              = 7.7680

                    TH1 = 256 - 8 = 248

                6.1.1.2) Baud Rate = 19200,
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 14318000 / 192 / (256 - TH1)
                              = 19200

                    256 - TH1 = 14318000 / 192 / 19200
                              = 3.8840

                    TH1 = 256 - 4 = 252

                6.1.1.3) Baud Rate = 38400,
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 14318000 / 192 / (256 - TH1)
                              = 38400

                    256 - TH1 = 14318000 / 192 / 38400
                              = 1.9420

                    TH1 = 256 - 2 = 254

                6.1.1.4) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 14318000 / 192 / (256 - TH1)
                              = 57600

                    256 - TH1 = 14318000 / 192 / 57600
                              = 1.2946

                    TH1 = 256 - 1 = 255

                6.1.1.5) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 14318000 / 192 / (256 - TH1)
                              = 76800

                    256 - TH1 = 14318000 / 192 / 76800
                              = 0.9710   (Fail !)


            6.1.2) SMOD = 1


                            2     * (Oscillator frequency)
                Baud Rate = -------------------------------------
                            16 * 12 * [256 - (TH1)]


                6.1.2.1) Baud Rate = 9600
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 14318000 / 96 / (256 - TH1)
                              = 9600

                    256 - TH1 = 14318000 / 96 / 9600
                              = 15.5360

                    TH1 = 256 - 16 = 240

                6.1.2.2) Baud Rate = 19200
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 14318000 / 96 / (256 - TH1)
                              = 19200

                    256 - TH1 = 14318000 / 96 / 19200
                              = 7.7680

                    TH1 = 256 - 8 = 248

                6.1.2.3) Baud Rate = 38400
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 14318000 / 96 / (256 - TH1)
                              = 38400

                    256 - TH1 = 14318000 / 96 / 38400
                              = 3.8840

                    TH1 = 256 - 4 = 252

                6.1.2.4) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 14318000 / 96 / (256 - TH1)
                              = 57600

                    256 - TH1 = 14318000 / 96 / 57600
                              = 2.5893

                    TH1 = 256 - 3 = 253

                6.1.2.5) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 14318000 / 96 / (256 - TH1)
                              = 76800

                    256 - TH1 = 14318000 / 96 / 76800
                              = 1.9420

                    TH1 = 256 - 2 = 254

        6.2) 12 clocks mode

                        SMOD
                       2     * (Oscillator frequency)
           Baud Rate = -------------------------------------
                       32 * 12 * [256 - (TH1)]

            6.2.1) SMOD = 0

                               Oscillator frequency
                Baud Rate = -------------------------------------
                            32 * 12 * [256 - (TH1)]

                6.2.1.1) Baud Rate = 9600,
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                         = 14318000 / 384 / (256 - TH1)
                         = 9600

                    256 - TH1 = 14318000 / 384 / 9600
                              = 3.8840

                    TH1 = 256 - 4 = 252

                6.2.1.2) Baud Rate = 19200,
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 14318000 / 384 / (256 - TH1)
                              = 19200

                    256 - TH1 = 14318000 / 384 / 19200
                              = 1.9420

                    TH1 = 256 - 2 = 254

                6.2.1.3) Baud Rate = 38400,
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 14318000 / 384 / (256 - TH1)
                              = 38400

                    256 - TH1 = 14318000 / 384 / 38400
                              = 0.9710      (Fail !)


                6.2.1.4) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 14318000 / 384 / (256 - TH1)
                              = 57600

                    256 - TH1 = 14318000 / 384 / 57600
                              = 0.6473      (Fail !)


                6.2.1.5) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 14318000 / 384 / (256 - TH1)
                              = 76800

                    256 - TH1 = 14318000 / 384 / 76800
                              = 0.4855      (Fail !)


            6.2.2) SMOD = 1


                            2     * (Oscillator frequency)
                Baud Rate = -------------------------------------
                            32 * 12 * [256 - (TH1)]


                6.2.2.1) Baud Rate = 9600
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 14318000 / 192 / (256 - TH1)
                              = 9600

                    256 - TH1 = 14318000 / 192 / 9600
                              = 7.7680

                    TH1 = 256 - 8 = 248

                6.2.2.2) Baud Rate = 19200
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 14318000 / 192 / (256 - TH1)
                              = 19200

                    256 - TH1 = 14318000 / 192 / 19200
                              = 3.8840

                    TH1 = 256 - 4 = 252

                6.2.2.3) Baud Rate = 38400
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 14318000 / 192 / (256 - TH1)
                              = 38400

                    256 - TH1 = 14318000 / 192 / 38400
                              = 1.9420

                    TH1 = 256 - 2 = 254

                6.2.2.4) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 14318000 / 192 / (256 - TH1)
                              = 57600

                    256 - TH1 = 14318000 / 192 / 57600
                              = 1.2946

                    TH1 = 256 - 1 = 255

                6.2.2.5) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 14318000 / 192 / (256 - TH1)
                              = 76800

                    256 - TH1 = 14318000 / 192 / 76800
                              = 0.9710      (Fail !)

    7) P89C61x2, XTAL = 14.318 MHz

        7.1) Timer 1
           Baud                 9600    19200   38400   57600   76800
           6-clock, SMOD=0      248     252     254     255     Fail
           6-clock, SMOD=1      240     248     252     253     254
           12-clock, SMOD=0     252     254     Fail    Fail    Fail
           12-clock, SMOD=1     248     252     254     255     Fail

        7.2) Timer 2
           Baud                 9600    19200   38400   57600   76800
           6-clock              0xFFA3  0xFFD1  0xFFE9  0xFFF0  0xFFF4
           12-clock             oxFFD1  0xFFE9  0xFFF4  0xFFF8  0xFFFA


    8) [11]28 P89C61x2
        When Timer 1 is used as the baud rate generator (T2CON.RCLK = 0,
        T2CON.TCLK = 0), the baud rates in Modes 1 and 3 are determined by
        the Timer 1 overflow rate and the value of SMOD as follows:

        Mode 1,3
                      SMOD
                    2     * (Timer 1 Overflow Rate)
        Baud Rate = -------------------------------------
                    n

        where
            n = 32 in 12-clock mode, 16 in 6-clock mode

        The Time 1 interrupt should be disabled in this application. The Timer
        itself can be configured for either "timer" or "counter" operation, and
        in any of its 3 running modes. In the most typical applications, it is
        configured for "timer" operation, in the auto-reload mode (high nibble
        of TMOD = 0010B). In that case the baud rate is given by the formula:


        Mode 1,3
                      SMOD
                    2     * (Oscillator frequency)
        Baud Rate = -------------------------------------
                    n * 12 * [256 - (TH1)]

        where
            n = 32 in 12-clock mode, 16 in 6-clock mode


        8.1) 6 clocks mode

                        SMOD
                       2     * (Oscillator frequency)
           Baud Rate = -------------------------------------
                       16 * 12 * [256 - (TH1)]

            8.1.1) SMOD = 0

                               Oscillator frequency
                Baud Rate = -------------------------------------
                            16 * 12 * [256 - (TH1)]

                8.1.1.1) Baud Rate = 9600,
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                         = 22118400 / 192 / (256 - TH1)
                         = 9600

                    256 - TH1 = 22118400 / 192 / 9600
                              = 12

                    TH1 = 256 - 12 = 244

                8.1.1.2) Baud Rate = 19200,
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 19200

                    256 - TH1 = 22118400 / 192 / 19200
                              = 6

                    TH1 = 256 - 6 = 250

                8.1.1.3) Baud Rate = 38400,
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 38400

                    256 - TH1 = 22118400 / 192 / 38400
                              = 3

                    TH1 = 256 - 3 = 253

                8.1.1.4) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 57600

                    256 - TH1 = 22118400 / 192 / 57600
                              = 2

                    TH1 = 256 - 2 = 254

                8.1.1.5) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 76800

                    256 - TH1 = 22118400 / 192 / 76800
                              = 1.5   (Fail !)


            8.1.2) SMOD = 1


                            2     * (Oscillator frequency)
                Baud Rate = -------------------------------------
                            16 * 12 * [256 - (TH1)]


                8.1.2.1) Baud Rate = 9600
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 22118400 / 96 / (256 - TH1)
                              = 9600

                    256 - TH1 = 22118400 / 96 / 9600
                              = 24

                    TH1 = 256 - 24 = 232

                8.1.2.2) Baud Rate = 19200
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 22118400 / 96 / (256 - TH1)
                              = 19200

                    256 - TH1 = 22118400 / 96 / 19200
                              = 12

                    TH1 = 256 - 12 = 244

                8.1.2.3) Baud Rate = 38400
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 22118400 / 96 / (256 - TH1)
                              = 38400

                    256 - TH1 = 22118400 / 96 / 38400
                              = 6

                    TH1 = 256 - 6 = 250

                8.1.2.4) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 22118400 / 96 / (256 - TH1)
                              = 57600

                    256 - TH1 = 22118400 / 96 / 57600
                              = 4

                    TH1 = 256 - 4 = 252

                8.1.2.5) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 96 / (256 - TH1)
                              = 22118400 / 96 / (256 - TH1)
                              = 76800

                    256 - TH1 = 22118400 / 96 / 76800
                              = 3

                    TH1 = 256 - 3 = 253

        8.2) 12 clocks mode

                        SMOD
                       2     * (Oscillator frequency)
           Baud Rate = -------------------------------------
                       32 * 12 * [256 - (TH1)]

            8.2.1) SMOD = 0

                               Oscillator frequency
                Baud Rate = -------------------------------------
                            32 * 12 * [256 - (TH1)]

                8.2.1.1) Baud Rate = 9600,
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                         = 22118400 / 384 / (256 - TH1)
                         = 9600

                    256 - TH1 = 22118400 / 384 / 9600
                              = 6

                    TH1 = 256 - 6 = 250

                8.2.1.2) Baud Rate = 19200,
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 22118400 / 384 / (256 - TH1)
                              = 19200

                    256 - TH1 = 22118400 / 384 / 19200
                              = 3

                    TH1 = 256 - 3 = 253

                8.2.1.3) Baud Rate = 38400,
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 22118400 / 384 / (256 - TH1)
                              = 38400

                    256 - TH1 = 22118400 / 384 / 38400
                              = 1.5     (Fail !)


                8.2.1.4) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 22118400 / 384 / (256 - TH1)
                              = 57600

                    256 - TH1 = 22118400 / 384 / 57600
                              = 1
                    
                    TH1 = 256 - 1 = 255


                8.2.1.5) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 384 / (256 - TH1)
                              = 22118400 / 384 / (256 - TH1)
                              = 76800

                    256 - TH1 = 22118400 / 384 / 76800
                              = 0.75    (Fail !)


            8.2.2) SMOD = 1


                            2     * (Oscillator frequency)
                Baud Rate = -------------------------------------
                            32 * 12 * [256 - (TH1)]


                8.2.2.1) Baud Rate = 9600
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 9600

                    256 - TH1 = 22118400 / 192 / 9600
                              = 12

                    TH1 = 256 - 12 = 244

                8.2.2.2) Baud Rate = 19200
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 19200

                    256 - TH1 = 22118400 / 192 / 19200
                              = 6

                    TH1 = 256 - 6 = 250

                8.2.2.3) Baud Rate = 38400
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 38400

                    256 - TH1 = 22118400 / 192 / 38400
                              = 3

                    TH1 = 256 - 3 = 253

                8.2.2.4) Baud Rate = 57600
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 57600

                    256 - TH1 = 22118400 / 192 / 57600
                              = 2

                    TH1 = 256 - 2 = 254

                8.2.2.5) Baud Rate = 76800
                    Baud Rate = Crystal Frequency / 192 / (256 - TH1)
                              = 22118400 / 192 / (256 - TH1)
                              = 76800

                    256 - TH1 = 22118400 / 192 / 76800
                              = 1.5     (Fail !)

    9) P89C61x2, Fclk = 14.318 MHz

                             Fclk
        Baud Rate = --------------------------------------
                    n * (65536 - (RCAP2H * 256 + RCAP2L))

        where
            n = 16 in 6 clock mode
              = 32 in 12 clock mode

        9.1) 6 clock mode
            9.1.1) Baud = 9600
                65536 - (RCAP2H * 256 + RCAP2L) = 22118400 / 9600 / 16
                                                = 144
                (RCAP2H * 256 + RCAP2L) = 65536 - 144   = 65392 = 0xFF70

            9.1.2) Baud = 19200
                65536 - (RCAP2H * 256 + RCAP2L) = 22118400 / 19200 / 16
                                                = 72
                (RCAP2H * 256 + RCAP2L) = 65536 - 72    = 65464 = 0xFFB8

            9.1.3) Baud = 38400
                65536 - (RCAP2H * 256 + RCAP2L) = 22118400 / 38400 / 16
                                                = 36
                (RCAP2H * 256 + RCAP2L) = 65536 - 36    = 65500 = 0xFFDC

            9.1.4) Baud = 57600
                65536 - (RCAP2H * 256 + RCAP2L) = 22118400 / 57600 / 16
                                                = 24
                (RCAP2H * 256 + RCAP2L) = 65536 - 24    = 65512 = 0xFFE8

            9.1.5) Baud = 76800
                65536 - (RCAP2H * 256 + RCAP2L) = 22118400 / 76800 / 16
                                                = 18
                (RCAP2H * 256 + RCAP2L) = 65536 - 18    = 65518 = 0xFFEE

        9.2) 12 clock mode
            9.2.1) Baud = 9600
                65536 - (RCAP2H * 256 + RCAP2L) = 22118400 / 9600 / 32
                                                = 72
                (RCAP2H * 256 + RCAP2L) = 65536 - 72    = 65464 = 0xFFB8

            9.2.2) Baud = 19200
                65536 - (RCAP2H * 256 + RCAP2L) = 22118400 / 19200 / 32
                                                = 36
                (RCAP2H * 256 + RCAP2L) = 65536 - 36    = 65500 = 0xFFDC

            9.2.3) Baud = 38400
                65536 - (RCAP2H * 256 + RCAP2L) = 22118400 / 38400 / 32
                                                = 18
                (RCAP2H * 256 + RCAP2L) = 65536 - 18    = 65518 = 0xFFEE

            9.2.4) Baud = 57600
                65536 - (RCAP2H * 256 + RCAP2L) = 22118400 / 57600 / 32
                                                = 12
                (RCAP2H * 256 + RCAP2L) = 65536 - 12    = 65524 = 0xFFF4

            9.2.5) Baud = 76800
                65536 - (RCAP2H * 256 + RCAP2L) = 22118400 / 76800 / 32
                                                = 9
                (RCAP2H * 256 + RCAP2L) = 65536 - 9     = 65527 = 0xFFF7

    10) P89C61x2, XTAL = 22.1184 MHz

        10.1) Timer 1
           Baud                 9600    19200   38400   57600   76800
           6-clock, SMOD=0      244     250     253     254     Fail
           6-clock, SMOD=1      232     244     250     252     253
           12-clock, SMOD=0     250     253     Fail    255     Fail
           12-clock, SMOD=1     244     250     253     254     Fail

        10.2) Timer 2
           Baud                 9600    19200   38400   57600   76800
           6-clock              0xFF70  0xFFB8  0xFFDC  0xFFE8  0xFFEE
           12-clock             0xFFB8  0xFFDC  0xFFEE  0xFFF4  0xFFF7

    11) Baud rate for Mode 3 using MPC89L58A

                      SMOD
                    2     * (Timer 1 Overflow Rate)
        Baud Rate = -------------------------------------
                    32




                    (Timer 2 Overflow Rate)
        Baud Rate = -------------------------------------
                    16



        11.1) MPC89L58A @XTAL= 24.576 MHz

            Fclk = 24.576 MHz = 24576000 Hz
                 = 24576 * 1000 Hz
                 = 24 * 2^10 * 10^3 Hz
                 = 2^16 * 3 * 5^3 Hz

                    (Timer 2 Overflow Rate)
        Baud Rate = ------------------------

                    16

                             Fclk
                  = --------------------------------------
                    32 * (65536 - (RCAP2H * 256 + RCAP2L))


                             Fclk
                  = --------------------------------------
                    32 * (65536 - RCAP2)




            11.1.1) Baud Rate = 9600

                    Baud Rate = Crystal Frequency / (32 * (65536 - RCAP2))
                              = 24576000 / (32 * (65536 - RCAP2))
                              = 9600

                    65536 - RCAP2 = 24576000 / 9600
                                  = 256

                    RCAP2 = 65536 - 256 = 65280 = 0xFF00

            11.1.2) Baud Rate = 19200

                    Baud Rate = Crystal Frequency / (32 * (65536 - RCAP2))
                              = 24576000 / (32 * (65536 - RCAP2))
                              = 19200

                    65536 - RCAP2 = 24576000 / 19200
                                  = 128

                    RCAP2 = 65536 - 128 = 65408 = 0xFF80

            11.1.3) Baud Rate = 38400

                    Baud Rate = Crystal Frequency / (32 * (65536 - RCAP2))
                              = 24576000 / (32 * (65536 - RCAP2))
                              = 38400

                    65536 - RCAP2 = 24576000 / 38400
                                  = 64

                    RCAP2 = 65536 - 64 = 65472 = 0xFFC0

            11.1.4) Baud Rate = 57600

                    Baud Rate = Crystal Frequency / (32 * (65536 - RCAP2))
                              = 24576000 / (32 * (65536 - RCAP2))
                              = 57600

                    65536 - RCAP2 = 24576000 / 57600
                                  = 128 / 3 = 42.6666

            11.1.4) Baud Rate = 76800

                    Baud Rate = Crystal Frequency / (32 * (65536 - RCAP2))
                              = 24576000 / (32 * (65536 - RCAP2))
                              = 76800

                    65536 - RCAP2 = 24576000 / 76800
                                  = 32

                    RCAP2 = 65536 - 32 = 65504 = 0xFFE0


   ********************************************************************** */

/* %% End Of File %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
