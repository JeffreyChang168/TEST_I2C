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

    Module: DISPATCH.C -

    Purpose: Implementation of DISPATCH.

    Version: 0.01                                   05:08PM  2013/03/20

    Compiler: Keil 8051 C Compiler v9.51

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 05:08PM  2013/03/20 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ********************************************************************** */

#define _DISPATCH_C_

/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "bit1618c.h"
#include "bitek.h"
#include "dispatch.h"
#include "mcu.h"
#include "osd.h"
#include "platform.h"
//#include "saradc.h"
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
#define MSG_MENU_TITLE_SIZE     10
#define MSG_RESET_SIZE          10

#define MSG_TITLE_SIZE          10

/* ------------------------------------
    Type Definitions
   ------------------------------------ */
//typedef void (CODE * CODE VP_SET_FUNC) (UB8 bValue);


/* ------------------------------------
    Variables Definitions
   ------------------------------------ */
UB8 CODE * CODE apbMENU_TITLE[] =
{
   //1234567890
    "BRIGHTNESS",   // 0
    "CONTRAST  ",   // 1
    "SATURATION",   // 2
    "RESET     ",   // 3
};


UB8 CODE abRESET_TITLE[] = {
//  0                    123456
//  FONT_2C_CHECK_ON,   "OFF   ",
//  7                    89
//  FONT_3C_CHECK_OFF,  "ON"

//  0      123456  7     89
    "\x2C""OFF   ""\x3C""ON"
};


/* ------------------------------------
    Function Prototypes
   ------------------------------------ */
//void DISPATCH_AjustVolume(void);


/* -------------------------------------------------------------------
    Name: DISPATCH_Init -
    Purpose: To initialize DISPATCH module.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void DISPATCH_Init (void)
{
    bSystemState        = SYSTEM_STANDBY;

    bMenuCursor         = MENU_DEFAULT;
    fDISPATCH_Backup    = FALSE;
} /* DISPATCH_Init */


#if (DISPATCH_TASK_BL_CNT)
/* -------------------------------------------------------------------
    Name: DISPATCH_Task_BL_Cnt -
    Purpose: To execute DISPATCH Task of BL.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void DISPATCH_Task_BL_Cnt (void)
{
#if 0
    BITEK_TxRepeat(VP_SHOW_OSD2, OSD2_AREA, FONT_20_BLANK);

    {
        UB8 CODE abDate[] =
        {
            #if (PLATFORM_FLASH)
          //"123456789012345678"
            "FLASH " VERSION_TIME " " VERSION_DATE
            #endif

            #if (PLATFORM_EEPROM)
          //"123456789012345678"
            "EEPROM " VERSION_TIME " " VERSION_DATE
            #endif
        };

        UB8 CODE abVersion[] =
        {
            VERSION_MESSAGE
        };


        // Show DATE string
        BITEK_TxBurst(VP_SHOW_DATE, sizeof(abDate) - 1, abDate);

        // Show VERSION string
        BITEK_TxBurst(VP_SHOW_VERSION, sizeof(abVersion) - 1, abVersion);
    }
#endif

    BITEK_TxByte(BITEK_12D_OSD2_ATTR2, OSD2_ON);
    bOSD_Cnt = OSD_AUTO_START;

    // BL ON
    MCU_BL_ON;
} /* DISPATCH_Task_BL_Cnt */
#endif


#if (DISPATCH_TASK_GPI)
/* -------------------------------------------------------------------
    Name: DISPATCH_Task_GPI -
    Purpose: To execute DISPATCH Task of GPI.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void DISPATCH_Task_GPI (void)
{
    // OSD Auto OFF !
    bOSD_Cnt = OSD_AUTO_START;

    #if (DEBUG_GPI)
    OSD_ShowHex(VP_SHOW_DISPATCH_CODE, bDISPATCH_Code);
    #endif

    switch (bSystemState)
    {
        case SYSTEM_STANDBY:
            switch (bDISPATCH_Code)
            {
                case DISPATCH_CODE_MENU:
                    BITEK_TxRepeat(VP_SHOW_OSD2, OSD2_AREA, FONT_20_BLANK);

                    bMenuCursor = MENU_BRIGHTNESS;

                    // TITLE string
                    BITEK_TxBurst(VP_SHOW_TITLE, MSG_MENU_TITLE_SIZE, apbMENU_TITLE[ bMenuCursor ]);

                    DISPATCH_UpdateMenu();

                    BITEK_TxByte(BITEK_12D_OSD2_ATTR2, OSD2_ON);

                    bSystemState = SYSTEM_SUB_MENU;
                    break;

                case DISPATCH_CODE_SOURCE:
                  //VP_SetTestPattern(PATTERN_NONE);
                    tsGD.bSource = (tsGD.bSource + 1) % SOURCE_SIZE;

                    VP_SetSource(tsGD.bSource);

                    BITEK_TxBurst(VP_SHOW_SOURCE_NAME, MSG_SOURCE_NAME_SIZE, apbSOURCE_Name[ tsGD.bSource ]);
                    BITEK_TxByte(BITEK_10D_OSD0_ATTR2, OSD0_ON);

                    // To force OSD off and backup values !
                    fDISPATCH_Backup = TRUE;
                    bOSD_Cnt = OSD_AUTO_START;
                    break;

                #if 0
                case DISPATCH_CODE_LEFT:        // Cursor PLUS
                    bPattern = (bPattern + 1) % PATTERN_SIZE;
                    VP_SetTestPattern(bPattern);
                    break;

                case DISPATCH_CODE_RIGHT:       // Cursor MINUS
                    bPattern = (bPattern + PATTERN_SIZE - 1) % PATTERN_SIZE;
                    VP_SetTestPattern(bPattern);
                    break;
                #endif
            } // bDISPATCH_Code
            break;

        case SYSTEM_SUB_MENU:
            switch (bDISPATCH_Code)
            {
                case DISPATCH_CODE_MENU:
                    bMenuCursor = (bMenuCursor + 1) % MENU_SIZE;

                    BITEK_TxRepeat(VP_SHOW_OSD2, OSD2_AREA, FONT_20_BLANK);

                    // TITLE string
                    BITEK_TxBurst(VP_SHOW_TITLE, MSG_TITLE_SIZE, apbMENU_TITLE[ bMenuCursor ]);
                    break;

                case DISPATCH_CODE_LEFT:        // Cursor PLUS
                    switch (bMenuCursor)
                    {
                        case MENU_BRIGHTNESS:
                            if (tsGD.bBrightness < BRIGHTNESS_MAX)
                                tsGD.bBrightness++;
                            break;

                        case MENU_CONTRAST:
                            if (tsGD.bContrast < CONTRAST_MAX)
                                tsGD.bContrast++;
                            break;

                        case MENU_SATURATION:
                            if (tsGD.bSaturation < SATURATION_MAX)
                                tsGD.bSaturation++;
                            break;

                        case MENU_RESET:
                            DISPATCH_UpdateReset();
                            break;
                    } // bMenuCurrentItem

                    fDISPATCH_Backup = TRUE;
                    break;

                case DISPATCH_CODE_RIGHT:       // Cursor MINUS
                    switch (bMenuCursor)
                    {
                        case MENU_BRIGHTNESS:
                            if (tsGD.bBrightness > BRIGHTNESS_MIN)
                                tsGD.bBrightness--;
                            break;

                        case MENU_CONTRAST:
                            if (tsGD.bContrast > CONTRAST_MIN)
                                tsGD.bContrast--;
                            break;

                        case MENU_SATURATION:
                            if (tsGD.bSaturation > SATURATION_MIN)
                                tsGD.bSaturation--;
                            break;

                        case MENU_RESET:
                            DISPATCH_UpdateReset();
                            break;
                    } // bMenuCurrentItem

                    fDISPATCH_Backup = TRUE;
                    break;

            } // bDISPATCH_Code

            DISPATCH_UpdateMenu();

            break;
    } // bSystemState
} /* DISPATCH_Task_GPI */
#endif


#if (DISPATCH_TASK_OSD_CNT)
/* -------------------------------------------------------------------
    Name: DISPATCH_Task_OSD_Cnt -
    Purpose: To execute DISPATCH Task of OSD.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void DISPATCH_Task_OSD_Cnt (void)
{
    BITEK_TxByte(BITEK_10D_OSD0_ATTR2, OSD0_OFF);
    BITEK_TxByte(BITEK_12D_OSD2_ATTR2, OSD2_OFF);


    if (fDISPATCH_Backup)
    {
    	#if PLATFORM_EEPROM
        // To backup DATA to EEPROM
        EEPROM_WriteBurst(EGD_ADDR_BASE, sizeof(tsGD), (UB8 *)&tsGD);
    	#endif

        #if PLATFORM_FLASH
    	FLASH_WriteAll();
    	#endif

        fDISPATCH_Backup = FALSE;
    }

    bSystemState = SYSTEM_STANDBY;
} /* DISPATCH_Task_OSD_Cnt */
#endif


#if (DISPATCH_TASK_SARADC)
/* -------------------------------------------------------------------
    Name: DISPATCH_Task_SARADC -
    Purpose: To execute DISPATCH Task of IR.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void DISPATCH_Task_SARADC (void)
{
  //OSD_ShowValue(VP_SHOW_AK_VALUE, bSARADC_SCAN_CODE, 100, ' ');

    switch (bSARADC_SCAN_CODE)
    {
        case AK_MENU:
            if (BITEK_RxByte(BITEK_12D_OSD2_ATTR2) & BITEK_MASK_OSD2_EN)
            {
                bMenuCursor = (bMenuCursor + 1) % MENU_SIZE;
            }
            else
            {
                bMenuCursor = MENU_BRIGHTNESS;

                BITEK_TxByte(BITEK_12D_OSD2_ATTR2, OSD2_ON);
            }

            BITEK_TxRepeat(VP_SHOW_OSD2, OSD2_AREA, FONT_20_BLANK);

            // TITLE string
            BITEK_TxBurst(VP_SHOW_TITLE, MSG_TITLE_SIZE, apbMENU_TITLE[ bMenuCursor ]);
            break;


        case AK_UP:
            switch (bMenuCursor)
            {
                case MENU_BRIGHTNESS:
                    if (tsGD.bBrightness < BRIGHTNESS_MAX)
                        tsGD.bBrightness++;
                    break;

                case MENU_CONTRAST:
                    if (tsGD.bContrast < CONTRAST_MAX)
                        tsGD.bContrast++;
                    break;

                case MENU_SATURATION:
                    if (tsGD.bSaturation < SATURATION_MAX)
                        tsGD.bSaturation++;
                    break;

                case MENU_RESET:
                    DISPATCH_UpdateReset();
                    break;
            }

            break;


        case AK_DOWN:
            switch (bMenuCursor)
            {
                case MENU_BRIGHTNESS:
                    if (tsGD.bBrightness > BRIGHTNESS_MIN)
                        tsGD.bBrightness--;
                    break;

                case MENU_CONTRAST:
                    if (tsGD.bContrast > CONTRAST_MIN)
                        tsGD.bContrast--;
                    break;

                case MENU_SATURATION:
                    if (tsGD.bSaturation > SATURATION_MIN)
                        tsGD.bSaturation--;
                    break;

                case MENU_RESET:
                    DISPATCH_UpdateReset();
                    break;
            }

            break;
    } // bSARADC_SCAN_CODE

    DISPATCH_UpdateMenu();

    if (bMenuCursor != MENU_RESET)
        bOSD_Cnt = OSD_AUTO_START;
} /* DISPATCH_Task_SARADC */
#endif


/* -------------------------------------------------------------------
    Name: DISPATCH_UpdateMenu -
    Purpose: To update MENU.
    Passed: bValue.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void DISPATCH_UpdateMenu (void)
{
    switch (bMenuCursor)
    {
        case MENU_BRIGHTNESS:
            OSD_ShowValue(VP_SHOW_VALUE, tsGD.bBrightness, 100, ' ');
            OSD_ShowProgressBar(VP_SHOW_BAR, tsGD.bBrightness);
            VP_SetBrightness(tsGD.bBrightness);
            break;

        case MENU_CONTRAST:
            OSD_ShowValue(VP_SHOW_VALUE, tsGD.bContrast, 100, ' ');
            OSD_ShowProgressBar(VP_SHOW_BAR, tsGD.bContrast);
            VP_SetContrast(tsGD.bContrast);
            break;

        case MENU_SATURATION:
            OSD_ShowValue(VP_SHOW_VALUE, tsGD.bSaturation, 100, ' ');
            OSD_ShowProgressBar(VP_SHOW_BAR, tsGD.bSaturation);
            VP_SetSaturation(tsGD.bSaturation);
            break;

        case MENU_RESET:
            BITEK_TxBurst(VP_SHOW_RESET, MSG_RESET_SIZE, abRESET_TITLE);
            break;
    }
} /* DISPATCH_UpdateMenu */


/* -------------------------------------------------------------------
    Name: DISPATCH_UpdateReset -
    Purpose:
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void DISPATCH_UpdateReset (void)
{
    tsGD = tsGD_Default;

    // To reset tsGD with default values
    #if PLATFORM_EEPROM
  //EEPROM_WriteBurst(EGD_ADDR, sizeof(tsGD_Default), (UB8 *)&tsGD_Default);
    #endif


	#if PLATFORM_FLASH
  //FLASH_WriteAll();
	#endif

    VP_Setup();

    // To force OSD OFF and backup values !
    fDISPATCH_Backup = TRUE;
    bOSD_Cnt = OSD_AUTO_STOP;
} // DISPATCH_UpdateReset



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
