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

    Module: MCU.C

    Purpose: Implementation of MCU.

    Version: 0.01                                   02:49PM  2014/05/23

    Compiler: Keil 8051 C Compiler v9.52

    Reference:
    [1] BIT5101 8051 MICROCONTROLLER WITH 64K FLASH AND ISP Version 0.01,
        2004/10/25, Beyond Innovation Technology
    [2] TP2804  8051 MICROCONTROLLER WITH 64K FLASH AND ISP Version 1.2,
        June 2004, TOPRO
    [3] MPC89x58A 8-bit MICRO-CONTROLLER Version A9,
        2006/08, Megawin Technology Co., Ltd.

   ----------------------------------------------------------------------
    Modification:

    v0.01 02:49PM  2014/05/23 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ********************************************************************** */


#define  _MCU_C_

/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "bit1618c.h"
#include "bitek.h"
#include "dispatch.h"
#include "gpi.h"
#include "i2c.h"
#include "mcu.h"
#include "osd.h"
#include "platform.h"
#include "timer.h"
#include "vp.h"

#if (PLATFORM_EEPROM)
    #include "eeprom.h"
#endif

#if (PLATFORM_FLASH)
    #include "flash.h"
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

/* -------------------------------------------------------------------
    Name: MCU_Init -
    Purpose: To initialize MCU module.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void MCU_Init (void)
{
    /* :::::::::::::::::::::::::
        MCU Port0 Configuration
       ::::::::::::::::::::::::: */
    #if (MCU_P0_RIN == ON)
    {
        UB8 b0E8;


        // RIN10_MFP_EN (0xE8[0]) = 0
        // RIN10_MFP_SEL (0xE8[2:1]) = 00
        // RIN_SPI_EN (0xE8[3]) = 0
        b0E8 = BITEK_RxByte(BITEK_0E8_RIN_ATTR) & ~(BITEK_MASK_RIN_SPI_EN | BITEK_MASK_RIN10_MFP_SEL | BITEK_MASK_RIN10_MFP_EN);

        // RIN_GPIO (0xE8[7]) = 1
        BITEK_TxByte(BITEK_0E8_RIN_ATTR, b0E8 | BITEK_MASK_RIN_GPIO);
    }
    #endif

    /* :::::::::::::::::::::::::
        MCU Port1 Configuration
       ::::::::::::::::::::::::: */
    #if (MCU_P1_GIN == ON)
    {
        UB8 b0EB;


        // GIN10_MFP_EN (0xEB[0]) = 0
        // GIN10_MFP_SEL (0xEB[2:1]) = 00
        // GIN_SPI_EN (0xEB[3]) = 0
        b0EB = BITEK_RxByte(BITEK_0EB_GIN_ATTR) & ~(BITEK_MASK_GIN_SPI_EN | BITEK_MASK_GIN10_MFP_SEL | BITEK_MASK_GIN10_MFP_EN);

        // GIN_GPIO (0xEB[7]) = 1
        BITEK_TxByte(BITEK_0EB_GIN_ATTR, b0EB | BITEK_MASK_GIN_GPIO);
    }
    #endif

    /* :::::::::::::::::::::::::
        MCU Port2 Configuration
       ::::::::::::::::::::::::: */
    #if (MCU_P2_BIN == ON)
    {
        UB8 b0EE;


        // GIN10_MFP_EN (0xEB[0]) = 0
        // GIN10_MFP_SEL (0xEB[2:1]) = 00
        // GIN_SPI_EN (0xEB[3]) = 0
        b0EE = BITEK_RxByte(BITEK_0EE_BIN_ATTR) & ~(BITEK_MASK_BIN_SPI_EN | BITEK_MASK_BIN10_MFP_SEL | BITEK_MASK_BIN10_MFP_EN);

        // GIN_GPIO (0xEB[7]) = 1
        BITEK_TxByte(BITEK_0EE_BIN_ATTR, b0EE | BITEK_MASK_BIN_GPIO);
    }
    #endif

    /* :::::::::::::::::::::::::
        MCU Port3 Configuration
       ::::::::::::::::::::::::: */
    #if (MCU_P3_TOUT == ON)
    {
        UB8 b0F3;


        // TOUT21_MFP_EN (0xF3[0]) = 0
        // TOUT21_MFP_SEL (0xF3[2:1]) = 00
        // TOUT_SPI_EN (0xF3[3]) = 0
        b0F3 = BITEK_RxByte(BITEK_0F3_TOUT_ATTR) & ~(BITEK_MASK_TOUT3_IR_EN | BITEK_MASK_TOUT_SPI_EN | BITEK_MASK_TOUT21_MFP_SEL | BITEK_MASK_TOUT21_MFP_EN);
        BITEK_TxByte(BITEK_0F3_TOUT_ATTR, b0F3);

        // TOUT_GPIO (0xF4[7:0]) = 0xFF
        BITEK_TxByte(BITEK_0F4_TOUT_GPIO, 0xFF);
    }
    #endif

    fVD_NoSignal    = TRUE;
    bMCU_BL_Cnt     = BL_START;
    bVP_STD_Cnt     = STD_START;
} /* MCU_Init */


/* -------------------------------------------------------------------
    Name: MCU_INT0_ISR -
    Purpose: INT0 Interrupt Service Routine (ISR).
    Passed: None
    Returns: None.
    Notes:

    Reference: [2]19
   ------------------------------------------------------------------- */
void MCU_INT0_ISR (void) interrupt 0    using 1
{
    UB8 bINT0;


    bINT0 = BITEK_RxByte(BITEK_002_INT0_FLAG_O);

    #if (DEBUG_MCU_INT0)
    OSD_ShowHex(VP_SHOW_OSD0_ROW0 + 0, bINT0);
    #endif

    if (bINT0 & BITEK_MASK_FLAG0_VD_SIGNALREADY)
    {
        fVD_NoSignal    = FALSE;
        bVP_STD_Cnt     = STD_START;

        // R0CD[6]=1 for Y/C separate modes only.
        if (tsGD.bSource != SOURCE_CVBS2)
            BITEK_TxByte(BITEK_0CD_DCLAMP_ATTR1, BITEK_RxByte(BITEK_0CD_DCLAMP_ATTR1) | BITEK_MASK_DCLAMP_STABLE_EN);

    }

	#if (I2C_SLAVE)
	#if (PLATFORM_I2C_INT0)
    if (bINT0 & BITEK_MASK_FLAG0_IR_I2CS)
	{
        I2C_SLAVE_Handler();

        #if (DEBUG_MCU_INT0)
        bMCU_INT0_Cnt++;
        OSD_ShowHex(VP_SHOW_OSD0_ROW0 + 3, bMCU_INT0_Cnt);
        #endif

	}
	#endif
	#endif

    #if (DEBUG_MCU_INT0)
  //bMCU_INT0_Cnt++;
  //OSD_ShowHex(VP_SHOW_OSD0_ROW0 + 3, bMCU_INT0_Cnt);
    #endif

    MCU_INT0_RESET();
} /* MCU_INT0_ISR */


/* -------------------------------------------------------------------
    Name: MCU_INT0_RESET -
    Purpose: To reset INT0.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void MCU_INT0_RESET (void)
{
    BITEK_TxByte(BITEK_003_INT0_MASK, 0xFF);


    BITEK_TxByte(BITEK_004_INT0_ACK, 0x00);

    // Caution: ISR6 can be assigned to INT0 or INT1 only,
    #if (PLATFORM_I2C_INT0)
    BITEK_TxByte(BITEK_004_INT0_ACK, INT0_ACK | INT0_IR);   // For I2C Slave interrupt @INT0
    #else
    BITEK_TxByte(BITEK_004_INT0_ACK, INT0_ACK);
    #endif

    BITEK_TxByte(BITEK_003_INT0_MASK, 0x00);
} /* MCU_INT0_RESET */


/* -------------------------------------------------------------------
    Name: MCU_INT1_ISR -
    Purpose: INT1 Interrupt Service Routine (ISR).
    Passed: None
    Returns: None.
    Notes:

    Reference: [2]19
   ------------------------------------------------------------------- */
void MCU_INT1_ISR (void) interrupt 2    using 2
{
    UB8 bINT1;


    bINT1 = BITEK_RxByte(BITEK_005_INT1_FLAG_O);

    #if (DEBUG_MCU_INT1)
    OSD_ShowHex(VP_SHOW_OSD0_ROW1 + 0, bINT1);
    #endif

    if (bINT1 & BITEK_MASK_FLAG1_VD_NOSIGNAL)
    {
        fVD_NoSignal    = TRUE;
        bVP_STD_Cnt     = STD_START;

        // R0CD[6]=0 for Y/C separate modes only.
        if (tsGD.bSource != SOURCE_CVBS2)
            BITEK_TxByte(BITEK_0CD_DCLAMP_ATTR1, BITEK_RxByte(BITEK_0CD_DCLAMP_ATTR1) & ~BITEK_MASK_DCLAMP_STABLE_EN);
    }

    if (bINT1 & BITEK_MASK_FLAG1_VD_STANDARD)
    {
        if (! fVD_NoSignal)
            bVP_STD_Cnt = STD_START;
    }

    #if (PLATFORM_GPI)
    if (bINT1 & BITEK_MASK_FLAG1_GPI)
    {
        // INT1_GPI_TASK
        UB8  bGPI;


        #if 0
                                                            //  Normal  Long
        bGPI_Flag   = BITEK_RxByte(BITEK_195_KEY_FLAG_O);   //  O       O
        bGPI_Long   = BITEK_RxByte(BITEK_197_KEY_LONG_O);   //  X       O
        #endif

        bGPI = BITEK_RxByte(BITEK_197_KEY_LONG_O);

        #if (DEBUG_GPI)
        bGPI_Status = BITEK_RxByte(BITEK_196_KEY_STATUS_O);
        OSD_ShowHex(VP_SHOW_GPI_STATUS, bGPI_Status);
        OSD_ShowHex(VP_SHOW_GPI_FLAG, bGPI_Flag);
        OSD_ShowHex(VP_SHOW_GPI_LONG, bGPI_Long);
        #endif


        if (bGPI)
        {
            // Auto-Repeat interval
         // BITEK_TxByte(BITEK_194_KEY_ATTR, GPI_DEBOUNCE | (GPI_AUTO_REPEAT / 2));
            BITEK_TxByte(BITEK_194_KEY_ATTR, GPI_DEBOUNCE | (GPI_AUTO_REPEAT / 4));

            // Note: bDISPATCH_Code must match with GPI !
            bDISPATCH_Code = bGPI;

    		fGPI_Task = TRUE;
        }
        else
        {
            bGPI = BITEK_RxByte(BITEK_195_KEY_FLAG_O);

            if (bGPI)
            {
                // Auto-Repeat start delay
                BITEK_TxByte(BITEK_194_KEY_ATTR, GPI_DEBOUNCE | GPI_AUTO_REPEAT);

                // Note: bDISPATCH_Code must match with GPI !
                bDISPATCH_Code = bGPI;

        		fGPI_Task = TRUE;
            }
        }
    }
    #endif


	#if (I2C_SLAVE)
	#if (PLATFORM_I2C_INT1)
    if (bINT1 & BITEK_MASK_FLAG1_IR_I2CS)
	{
        I2C_SLAVE_Handler();

        #if (DEBUG_MCU_INT1)
        bMCU_INT1_Cnt++;
        OSD_ShowHex(VP_SHOW_OSD0_ROW1 + 3, bMCU_INT1_Cnt);
        #endif

	}
	#endif
	#endif

    #if (DEBUG_MCU_INT1)
  //bMCU_INT1_Cnt++;
  //OSD_ShowHex(VP_SHOW_OSD0_ROW1 + 3, bMCU_INT1_Cnt);
    #endif

    MCU_INT1_RESET();
} /* MCU_INT1_ISR */


/* -------------------------------------------------------------------
    Name: MCU_INT1_RESET -
    Purpose: To reset INT0.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void MCU_INT1_RESET (void)
{
    BITEK_TxByte(BITEK_006_INT1_MASK, 0xFF);

    #if (PLATFORM_GPI)
    // GPI ACK
    BITEK_TxByte(BITEK_18E_KEY_ACK, 0x00);
    #endif


    // INT1 ACK
    BITEK_TxByte(BITEK_007_INT1_ACK, 0x00);
    
    // Caution: ISR6 can be assigned to INT0 or INT1 only,
    #if (PLATFORM_I2C_INT1)
    BITEK_TxByte(BITEK_007_INT1_ACK, INT1_ACK | INT1_IR);   // For I2C Slave interrupt @INT1
    #else
    BITEK_TxByte(BITEK_007_INT1_ACK, INT1_ACK);
    #endif

    #if (PLATFORM_GPI)
    // GPI ACK
    BITEK_TxByte(BITEK_18E_KEY_ACK, GPI_ACK);
    #endif

    BITEK_TxByte(BITEK_006_INT1_MASK, 0x00);
} /* MCU_INT1_RESET */


/* -------------------------------------------------------------------
    Name: MCU_SETUP_INT -
    Purpose: To setup interrupt.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void MCU_SETUP_INT (void)
{
    BITEK_TxByte(BITEK_008_INT_ATTR, INT_ATTR);

    /* R009
    ; 7 6 5 4 3 2 1 0
    ; | | | | | |_|_|_ INT0_ISR6_SEL (Interrupt 6 source select for INT0
    ; | | | | |         000=IR
    ; | | | | |         001=I2C_Slave_S (R_I2C_SINT=1)  or
    ; | | | | |             I2C_Slave_Rx                or
    ; | | | | |             I2C_Slave_Tx
    ; | | | | |         010=I2C_Slave_S (R_I2C_SINT=1)  or
    ; | | | | |             I2C_Slave_Tx
    ; | | | | |         011=I2C_Slave_S (R_I2C_SINT=1)  or
    ; | | | | |             I2C_Slave_Rx                or
    ; | | | | |         100=I2C_Slave_Tx (Write)
    ; | | | | |         101=I2C_Slave_Rx (Read)
    ; | | | | |         110=I2C_Slave_S  (Start)
    ; | | | | |         111=I2C_Slave_P  (Stop)
    ; | | | | |
    ; | | |_|_|_______ INT1_ISR6_SEL (Interrupt 6 source select for INT1
    ; | |               000=IR
    ; | |               001=I2C_Slave_S (R_I2C_SINT=1)  or
    ; | |                   I2C_Slave_Rx                or
    ; | |                   I2C_Slave_Tx
    ; | |               010=I2C_Slave_S (R_I2C_SINT=1)  or
    ; | |                   I2C_Slave_Tx
    ; | |               011=I2C_Slave_S (R_I2C_SINT=1)  or
    ; | |                   I2C_Slave_Rx                or
    ; | |               100=I2C_Slave_Tx
    ; | |               101=I2C_Slave_Rx
    ; | |               110=I2C_Slave_S
    ; | |               111=I2C_Slave_P
    ; | |
    ; | |_____________ INT0_ISR4_SEL (Interrupt 4 source select for INT0
    ; |                 0=Video Decoder CC decoder
    ; |                 1=Master I2C
    ; |
    ; |_______________ INT1_ISR4_SEL (Interrupt 4 source select for INT1
    ;                   0=Line-Buffer Error/Bus Interface Error
    ;                   1=Master I2C
    */

	#if (PLATFORM_I2C)
    BITEK_TxByte(BITEK_009_INT_SEL, BITEK_MASK_INT1_ISR6_SWR | BITEK_MASK_INT0_ISR6_SWR);  // INT1: Start+Write+Read, INT0: Start+Write+Read
  //BITEK_TxByte(BITEK_009_INT_SEL, BITEK_MASK_INT1_ISR6_SWR);  // Start+Write+Read
  //BITEK_TxByte(BITEK_009_INT_SEL, BITEK_MASK_INT1_ISR6_S);    // Start
  //BITEK_TxByte(BITEK_009_INT_SEL, BITEK_MASK_INT1_ISR6_SW);   // Start+Write
	#else
    BITEK_TxByte(BITEK_009_INT_SEL, 0x00);      // ISR6 as IR Interrupt !
    #endif

    MCU_INT0_RESET();
    MCU_INT1_RESET();

    IP      = SFR_IP;
    TCON    = SFR_TCON;
    T2CON   = SFR_T2CON;
    IE      = INT_ON;
} /* MCU_SETUP_INT */



/* **********************************************************************

    Description:


   ********************************************************************** */

/* %% End Of File %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
