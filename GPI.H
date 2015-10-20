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

    Module: GPI.H -

    Purpose: Interface of GPI.

    Version: 0.02                                   01:40PM  2014/05/20

    Compiler: Keil 8051 C Compiler v9.52

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 05:22PM  2013/03/19 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ----------------------------------------------------------------------
    v0.02 01:39PM  2014/05/20 Jeffrey Chang
    Reason:
        1. To revise for VID1319_101_128 platform.
    Solution:

   ********************************************************************** */

#ifndef _GPI_H_
#define _GPI_H_


/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "common.h"
#include "platform.h"


/* ------------------------------------
    Macro Definitions
   ------------------------------------ */
#undef EXTERN

#ifdef _GPI_C_
    #define EXTERN
#else
    #define EXTERN  extern
#endif



/* ::::::::::::::::::::::::::::::::::::
    GPI  Configuration
   :::::::::::::::::::::::::::::::::::: */
#define GPI_RIN         0
#define GPI_GIN         1
#define GPI_BIN         2
#define GPI_TOUT        3
//#define GPI_CFG       GPI_RIN     //
//#define GPI_CFG       GPI_GIN     //
  #define GPI_CFG       GPI_BIN     // OK 07:05PM  2013/01/07


/*
; ------------------------------------------------------------------------------
;   KEY SRC0 (0x18C)
;   KEY SRC1 (0x18D)
;   SRC1 SRC0
;   0    0      RIN
;   0    1      GIN
;   1    0      BIN
;   1    1      KEYIN
;
; 7 6 5 4 3 2 1 0
; | | | | | | | |_ RIN0
; | | | | | | |___ RIN1
; | | | | | |_____ RIN2
; | | | | |_______ RIN3
; | | | |_________ RIN4
; | | |___________ RIN5
; | |_____________ RIN6
; |_______________ RIN7
; 0 0 0 0 0 0 0 0   0x00 KEY SRC0 (0x18C)
; 0 0 0 0 0 0 0 0   0x00 KEY SRC1 (0x18D)
;
; 7 6 5 4 3 2 1 0
; | | | | | | | |_ GIN0
; | | | | | | |___ GIN1
; | | | | | |_____ GIN2
; | | | | |_______ GIN3
; | | | |_________ GIN4
; | | |___________ GIN5
; | |_____________ GIN6
; |_______________ GIN7
; 1 1 1 1 1 1 1 1   0xFF KEY SRC0 (0x18C)
; 0 0 0 0 0 0 0 0   0x00 KEY SRC1 (0x18D)
;
; 7 6 5 4 3 2 1 0
; | | | | | | | |_ BIN0
; | | | | | | |___ BIN1
; | | | | | |_____ BIN2
; | | | | |_______ BIN3
; | | | |_________ BIN4
; | | |___________ BIN5
; | |_____________ BIN6
; |_______________ BIN7
; 0 0 0 0 0 0 0 0   0x00 KEY SRC0 (0x18C)
; 1 1 1 1 1 1 1 1   0xFF KEY SRC1 (0x18D)
;
; 7 6 5 4 3 2 1 0
; | | | | | | | |_ TOUT0/HSYNC @0x0F2[4]: 0=TOUT0, 1=HSYNC
; | | | | | | |___ TOUT1/VSYNC @0x0F1[4]: 0=TOUT1, 1=VSYNC
; | | | | | |_____ TOUT2
; | | | | |_______ TOUT3
; | | | |_________ TOUT4
; | | |___________ TOUT5
; | |_____________ TOUT6
; |_______________ TOUT7
; 1 1 1 1 1 1 1 1   0xFF KEY SRC0 (0x18C)
; 1 1 1 1 1 1 1 1   0xFF KEY SRC1 (0x18D)

*/
////////////////////////////////////////////////////////////////////////////////

#if (GPI_CFG == GPI_RIN)
#define GPI_SRC0_RIN0       0x00
#define GPI_SRC1_RIN0       0x00
#define GPI_SRC0_RIN1       0x00
#define GPI_SRC1_RIN1       0x00
#define GPI_SRC0_RIN2       0x00
#define GPI_SRC1_RIN2       0x00
#define GPI_SRC0_RIN3       0x00
#define GPI_SRC1_RIN3       0x00
#define GPI_SRC0_RIN4       0x00
#define GPI_SRC1_RIN4       0x00
#define GPI_SRC0_RIN5       0x00
#define GPI_SRC1_RIN5       0x00
#define GPI_SRC0_RIN6       0x00
#define GPI_SRC1_RIN6       0x00
#define GPI_SRC0_RIN7       0x00
#define GPI_SRC1_RIN7       0x00

#elif (GPI_CFG == GPI_GIN)
#define GPI_SRC0_GIN0       0x01
#define GPI_SRC1_GIN0       0x00
#define GPI_SRC0_GIN1       0x02
#define GPI_SRC1_GIN1       0x00
#define GPI_SRC0_GIN2       0x04
#define GPI_SRC1_GIN2       0x00
#define GPI_SRC0_GIN3       0x08
#define GPI_SRC1_GIN3       0x00
#define GPI_SRC0_GIN4       0x10
#define GPI_SRC1_GIN4       0x00
#define GPI_SRC0_GIN5       0x20
#define GPI_SRC1_GIN5       0x00
#define GPI_SRC0_GIN6       0x40
#define GPI_SRC1_GIN6       0x00
#define GPI_SRC0_GIN7       0x80
#define GPI_SRC1_GIN7       0x00

#elif (GPI_CFG == GPI_BIN)
#define GPI_SRC0_BIN0       0x00
#define GPI_SRC1_BIN0       0x01
#define GPI_SRC0_BIN1       0x00
#define GPI_SRC1_BIN1       0x02
#define GPI_SRC0_BIN2       0x00
#define GPI_SRC1_BIN2       0x04
#define GPI_SRC0_BIN3       0x00
#define GPI_SRC1_BIN3       0x08
#define GPI_SRC0_BIN4       0x00
#define GPI_SRC1_BIN4       0x10
#define GPI_SRC0_BIN5       0x00
#define GPI_SRC1_BIN5       0x20
#define GPI_SRC0_BIN6       0x00
#define GPI_SRC1_BIN6       0x40
#define GPI_SRC0_BIN7       0x00
#define GPI_SRC1_BIN7       0x80

// VID1124-102-174
#define GPI_SRC0           (GPI_SRC0_BIN4 | GPI_SRC0_BIN3 | GPI_SRC0_BIN2 | GPI_SRC0_BIN1 | GPI_SRC0_BIN0)
#define GPI_SRC1           (GPI_SRC1_BIN4 | GPI_SRC1_BIN3 | GPI_SRC1_BIN2 | GPI_SRC1_BIN1 | GPI_SRC1_BIN0)


#elif (GPI_CFG == TOUT_TOUT)
#define GPI_SRC0_HSYNC      0x01     //@0x0F2[4]: 0=TOUT0, 1=HSYNC
#define GPI_SRC1_HSYNC      0x01
#define GPI_SRC0_VSYNC      0x02     //@0x0F1[4]: 0=TOUT1, 1=VSYNC
#define GPI_SRC1_VSYNC      0x02
#define GPI_SRC0_TOUT0      0x01     //@0x0F2[4]: 0=TOUT0, 1=HSYNC
#define GPI_SRC1_TOUT0      0x01
#define GPI_SRC0_TOUT1      0x02     //@0x0F1[4]: 0=TOUT1, 1=VSYNC
#define GPI_SRC1_TOUT1      0x02
#define GPI_SRC0_TOUT2      0x04
#define GPI_SRC1_TOUT2      0x04
#define GPI_SRC0_TOUT3      0x08
#define GPI_SRC1_TOUT3      0x08
#define GPI_SRC0_TOUT4      0x10
#define GPI_SRC1_TOUT4      0x10
#define GPI_SRC0_TOUT5      0x20
#define GPI_SRC1_TOUT5      0x20
#define GPI_SRC0_TOUT6      0x40
#define GPI_SRC1_TOUT6      0x40
#define GPI_SRC0_TOUT7      0x80
#define GPI_SRC1_TOUT7      0x80

#endif



/*
; ------------------------------------------------------------------------------
;   KEY ACK  (0x18E)
; 7 6 5 4 3 2 1 0                   VID1124_102_174 VID1232_100_128
; | | | | | | | |_ GPI0             SW1=MENU        SW1=LEFT (-)
; | | | | | | |___ GPI1             SW2=SELECT      SW2=SOURCE
; | | | | | |_____ GPI2     MENU    SW3=RIGHT(+)    SW3=SELECT
; | | | | |_______ GPI3     SOURCE  SW4=LEFT (-)    SW4=RIGHT(+)
; | | | |_________ GPI4             SW5=SOURCE      SW5=MENU
; | | |___________ GPI5     +
; | |_____________ GPI6     -
; |_______________ GPI7
*/
#define GPI_ACK0            0x01
#define GPI_ACK1            0x02
#define GPI_ACK2            0x04
#define GPI_ACK3            0x08
#define GPI_ACK4            0x10
#define GPI_ACK5            0x20
#define GPI_ACK6            0x40
#define GPI_ACK7            0x80


//#define GPI_ACK            (GPI_ACK6 | GPI_ACK5 | GPI_ACK4 | GPI_ACK3 | GPI_ACK2)
//#define DISPATCH_CODE_MENU           GPI_ACK2
//#define DISPATCH_CODE_SELECT         GPI_ACK3
//#define DISPATCH_CODE_RIGHT          GPI_ACK5
//#define DISPATCH_CODE_LEFT           GPI_ACK6

// VID1124_102_174
// VID1232_100_128
#define GPI_ACK             (GPI_ACK4 | GPI_ACK3 | GPI_ACK2 | GPI_ACK1 | GPI_ACK0)


/*
; ------------------------------------------------------------------------------
;   GPI TYPE0 (0x190)
;   GPI TYPE1 (0x191)
;   TYPE1 TYPE0
;   0    0      Falling or Long key
;   0    1      Rising  or Long key
;   1    0      Value changed
;   1    1      Long key ONLY
; 7 6 5 4 3 2 1 0
; | | | | | | | |_ GPI0
; | | | | | | |___ GPI1
; | | | | | |_____ GPI2
; | | | | |_______ GPI3
; | | | |_________ GPI4
; | | |___________ GPI5
; | |_____________ GPI6
; |_______________ GPI7
; 0 0 0 0 0 0 0 0   0x00 KEY TYPE0 (0x190) Falling
; 0 0 0 0 0 0 0 0   0x00 KEY TYPE1 (0x191) Falling
*/
// Falling edge (Pull-high)
#define GPI_TYPE0           0x00
#define GPI_TYPE1           0x00


/*
; ------------------------------------------------------------------------------
;   KEY LONG ENABLE  (0x192)
; 7 6 5 4 3 2 1 0           FPGA    VID1124_102_174 VID1232_100_128
; | | | | | | | |_ GPI0             SW1=MENU        SW1=LEFT (-)
; | | | | | | |___ GPI1             SW2=SELECT      SW2=SOURCE
; | | | | | |_____ GPI2     MENU    SW3=RIGHT(+)    SW3=SELECT
; | | | | |_______ GPI3     SOURCE  SW4=LEFT (-)    SW4=RIGHT(+)
; | | | |_________ GPI4             SW5=SOURCE      SW5=MENU
; | | |___________ GPI5     +
; | |_____________ GPI6     -
; |_______________ GPI7
; 0 1 1 1 1 1 0 0   0x7C    +/-/MENU/SOURCE for FPGA
; 0 1 1 0 0 0 0 0   0x60    +/-             for FPGA
; 0 0 0 0 1 1 0 0   0x0C    RIGHT/LEFT for VID1124_102_174
; 0 0 0 0 1 0 0 1   0x09    RIGHT/LEFT for VID1232_100_128
*/
//#define GPI_LONG_EN       0x7C
//#define GPI_LONG_EN       0x60
//#define GPI_LONG_EN       0x06    // RIGHT/LEFT for VID-1140-102-174
//#define GPI_LONG_EN       0x0C    // RIGHT/LEFT for VID-1140-102-174
//#define GPI_LONG_EN       0x00

#if (PLATFORM_CFG == PLATFORM_VID1124_102_174)
    #define GPI_LONG_EN       0x0C    // RIGHT/LEFT for VID1124_102_174
#elif (PLATFORM_CFG == PLATFORM_VID1232_100_128)
    #define GPI_LONG_EN       0x09    // RIGHT/LEFT for VID1232_100_128
#elif (PLATFORM_CFG == PLATFORM_VID1319_101_128)
    #define GPI_LONG_EN       0x09    // RIGHT/LEFT for VID1319_101_128
#else
    #error GPI.H - Invalid PLATFORM_CFG !
#endif // PLATFORM_CFG


/*
; ------------------------------------------------------------------------------
;   KEY LONG SEL     (0x193)    0=Falling, 1=Rising
; 7 6 5 4 3 2 1 0
; | | | | | | | |_ GPI0
; | | | | | | |___ GPI1
; | | | | | |_____ GPI2
; | | | | |_______ GPI3
; | | | |_________ GPI4
; | | |___________ GPI5
; | |_____________ GPI6
; |_______________ GPI7
; 0 0 0 0 0 0 0 0   0x00    Falling
; 0 1 1 1 1 1 0 0   0x7C    Rising
; 1 1 1 1 1 1 1 1   0xFF    Rising
*/
// Falling for BIN port (Pull-high)
#define GPI_LONG_SEL        0x00




/*
; ------------------------------------------------------------------------------
;   KEY LONG TIME   (0x194)
; 7 6 5 4 3 2 1 0
; | | | |_|_|_|_|_ LONG KEY detection time
; |_|_|___________ Debounce detection time
; 1 0 0 0 0 0 1 1           0x83
; 1 0 0 1 0 0 0 0           0x90
; 1 0 0 1 1 1 1 1           0x9F
; 1 1 1 1 1 1 1 1           0xFF
; 1 1 1 0 1 0 1 0           0xEA
; 1 1 1 1 0 0 0 0           0xF0
*/

//#define GPI_DEBOUNCE      0xE0
//#define GPI_DEBOUNCE      0x80
  #define GPI_DEBOUNCE      0xC0
  #define GPI_AUTO_REPEAT   0x1F
  #define GPI_LONG_TIME     (GPI_DEBOUNCE | GPI_AUTO_REPEAT)


/* ------------------------------------
    Type Definitions
   ------------------------------------ */


/* ------------------------------------
    Variables Definitions/Declarations
   ------------------------------------ */
#if (PLATFORM_GPI)
EXTERN UB8  bGPI_Status;
EXTERN BOOL fGPI_Task;
#endif

/* ------------------------------------
    Function Prototypes
   ------------------------------------ */
void GPI_Init(void);


#endif /* _GPI_H_ */


/* **********************************************************************

    Description:

   ********************************************************************** */

/* %% End Of File %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
