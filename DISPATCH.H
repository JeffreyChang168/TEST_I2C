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

    Module: DISPATCH.H -

    Purpose: Interface of DISPATCH.

    Version: 0.03                                   01:40PM  2014/05/20

    Compiler: Keil 8051 C Compiler v9.52

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 05:08PM  2013/03/20 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ----------------------------------------------------------------------
    v0.02 03:49PM  2013/04/15 Jeffrey Chang
    Reason:
        1. To revise for VID1232_100_128 platform.
    Solution:

   ----------------------------------------------------------------------
    v0.03 01:39PM  2014/05/20 Jeffrey Chang
    Reason:
        1. To revise for VID1319_101_128 platform.
    Solution:

   ********************************************************************** */

#ifndef _DISPATCH_H_
#define _DISPATCH_H_


/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "platform.h"

/* ------------------------------------
    Macro Definitions
   ------------------------------------ */
#undef EXTERN

#ifdef _DISPATCH_C_
    #define EXTERN
#else
    #define EXTERN  extern
#endif


#define DISPATCH_TASK_BL_CNT            ON
#define DISPATCH_TASK_GPI               OFF
#define DISPATCH_TASK_OSD_CNT           ON
#define DISPATCH_TASK_SARADC            OFF


/*
; 7 6 5 4 3 2 1 0           VID1124_102_174 VID1232_100_128
; | | | | | | | |_ GPI0     SW1=MENU        SW1=LEFT (-)
; | | | | | | |___ GPI1     SW2=SELECT      SW2=SOURCE
; | | | | | |_____ GPI2     SW3=RIGHT(+)    SW3=SELECT
; | | | | |_______ GPI3     SW4=LEFT (-)    SW4=RIGHT(+)
; | | | |_________ GPI4     SW5=SOURCE      SW5=MENU
; | | |___________ GPI5
; | |_____________ GPI6
; |_______________ GPI7
*/
#if (PLATFORM_CFG == PLATFORM_VID1124_102_174)
    #define DISPATCH_CODE_NONE          0x00
    #define DISPATCH_CODE_MENU          0x01
    #define DISPATCH_CODE_SELECT        0x02
    #define DISPATCH_CODE_RIGHT         0x04
    #define DISPATCH_CODE_LEFT          0x08
    #define DISPATCH_CODE_SOURCE        0x10
#elif (PLATFORM_CFG == PLATFORM_VID1232_100_128)
    #define DISPATCH_CODE_NONE          0x00
    #define DISPATCH_CODE_LEFT          0x01
    #define DISPATCH_CODE_SOURCE        0x02
    #define DISPATCH_CODE_SELECT        0x04
    #define DISPATCH_CODE_RIGHT         0x08
    #define DISPATCH_CODE_MENU          0x10
#elif (PLATFORM_CFG == PLATFORM_VID1319_101_128)
    #define DISPATCH_CODE_NONE          0x00
    #define DISPATCH_CODE_LEFT          0x01
    #define DISPATCH_CODE_SOURCE        0x02
    #define DISPATCH_CODE_SELECT        0x04
    #define DISPATCH_CODE_RIGHT         0x08
    #define DISPATCH_CODE_MENU          0x10
#else
    #error DISPATCH.H - Invalid PLATFORM_CFG !
#endif // PLATFORM_CFG


/* ------------------------------------
    Type Definitions
   ------------------------------------ */


/* ::::::::::::::::::::::::::::::::::::
    System Finite States
   :::::::::::::::::::::::::::::::::::: */
typedef enum SYSTEM_STATES
{
    SYSTEM_STANDBY          ,
    SYSTEM_SUB_MENU         ,

} enumSYSTEM_STATES;


typedef enum MAINMENU_ID_ENUM
{
    MAINMENU_ID_PICTURE         ,
    MAINMENU_ID_OPTION          ,
    MAINMENU_ID_SYSTEM          ,
    MAINMENU_ID_PRESET          ,

    MAINMENU_SIZE
} enumMAINMENU_ID;


////////////////////////////////////////////////////////////////////////////////
typedef enum PICTURE_ID_ENUM
{
    PICTURE_ID_BRIGHTNESS       ,
    PICTURE_ID_CONTRAST         ,
    PICTURE_ID_SATURATION       ,

    MENU_PICTURE_SIZE
} enumPICTURE_ID;


////////////////////////////////////////////////////////////////////////////////
typedef enum OPTION_ID_ENUM
{
    OPTION_ID_DISPLAY           ,
    OPTION_ID_LANGUAGE          ,

    MENU_OPTION_SIZE
} enumOPTION_ID;


typedef enum DISPLAY_ENUM
{
    DISPLAY_NORMAL              ,
    DISPLAY_FLIP                ,

    DISPLAY_SIZE                ,
    DISPLAY_DEFAULT = DISPLAY_NORMAL  ,
} enumDISPLAY;


typedef enum ZOOM_ENUM
{
    ZOOM_169                    ,
    ZOOM_43                     ,

    ZOOM_SIZE                   ,
    ZOOM_DEFAULT    = ZOOM_169  ,
} enumZOOM;


typedef enum LANGUAGE_ENUM
{
    LANGUAGE_ENGLISH            ,   // EN
    LANGUAGE_CHINESE            ,   // CN

    LANGUAGE_SIZE               ,
    LANGUAGE_DEFAULT = LANGUAGE_ENGLISH  ,
} enumLANGUAGE;


////////////////////////////////////////////////////////////////////////////////
typedef enum SYSTEM_ID_ENUM
{
    SYSTEM_ID_STANDARD          ,

    MENU_SYSTEM_SIZE
} enumSYSTEM_ID;


/* ::::::::::::::::::::::::::::::::::::
    TUNER Color Standard System
   :::::::::::::::::::::::::::::::::::: */
typedef enum SYSTEM_ENUM
{
    SYSTEM_NTSC_M     ,

    SYSTEM_PAL_B      ,
    SYSTEM_PAL_DK     ,
    SYSTEM_PAL_GH     ,
    SYSTEM_PAL_I      ,
    SYSTEM_PAL_M      ,
    SYSTEM_PAL_N      ,

    SYSTEM_SECAM_B    ,
    SYSTEM_SECAM_DK   ,
    SYSTEM_SECAM_G    ,
    SYSTEM_SECAM_L    ,

    SYSTEM_SIZE       ,
    SYSTEM_FM_RADIO   ,

    SYSTEM_MIN      = SYSTEM_NTSC_M ,

    SYSTEM_DEFAULT  = SYSTEM_NTSC_M ,

    SYSTEM_MAX      = SYSTEM_SECAM_L,
} enumSYSTEM;


////////////////////////////////////////////////////////////////////////////////
typedef enum PRESET_ID_ENUM
{
    PRESET_ID_AUTO              ,
    PRESET_ID_MANUAL            ,
    PRESET_ID_FINE              ,
    PRESET_ID_BAND              ,
    PRESET_ID_POSITION          ,
    PRESET_ID_SKIP              ,

    MENU_PRESET_SIZE
} enumPRESET_ID;


typedef enum BAND_ENUM
{
    BAND_VHFL                   ,
    BAND_VHFH                   ,
    BAND_UHF                    ,

    BAND_SIZE                   ,
    BAND_DEFAULT    = BAND_VHFL ,
} enumBAND;


typedef enum SKIP_ENUM
{
    SKIP_OFF                    ,
    SKIP_ON                     ,

    SKIP_SIZE                   ,
    SKIP_DEFAULT    = SKIP_OFF  ,
} enumSKIP;


typedef enum MENU_ENUM
{
    MENU_BRIGHTNESS     ,
    MENU_CONTRAST       ,
    MENU_SATURATION     ,
    MENU_RESET          ,

    MENU_SIZE           ,
    MENU_DEFAULT    = MENU_BRIGHTNESS
} enumMENU;



/* ------------------------------------
    Variables Definitions/Declarations
   ------------------------------------ */
EXTERN  enumSYSTEM_STATES   bSystemState;       /* Current system state */
EXTERN  UB8                 bDISPATCH_Code;
EXTERN  BOOL                fDISPATCH_Backup;
EXTERN  UB8                 bMenuCursor;


/* ------------------------------------
    Function Prototypes
   ------------------------------------ */
void DISPATCH_Init(void);

void DISPATCH_SetVolume(void);

void DISPATCH_ShowBand(void);
void DISPATCH_ShowSkip(void);


#if (DISPATCH_TASK_BL_CNT)
void DISPATCH_Task_BL_Cnt(void);
#endif

#if (DISPATCH_TASK_GPI)
void DISPATCH_Task_GPI(void);
#endif

#if (DISPATCH_TASK_OSD_CNT)
void DISPATCH_Task_OSD_Cnt(void);
#endif

#if (DISPATCH_TASK_SARADC)
void DISPATCH_Task_SARADC(void);
#endif


void DISPATCH_UpdateMenu(void);
void DISPATCH_UpdateReset(void);



#endif /* _DISPATCH_H_ */


/* **********************************************************************

    Description:

   ********************************************************************** */

/* %% End Of File %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
