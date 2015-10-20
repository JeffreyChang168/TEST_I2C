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

    Module: GPI.C -

    Purpose: Implementation of GPI.

    Version: 0.01                                   07:03PM  2012/05/31

    Compiler: Keil 8051 C Compiler v9.51

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 07:03PM  2012/05/31 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ********************************************************************** */

#define _GPI_C_

/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "bit1618c.h"
#include "bitek.h"
#include "common.h"
#include "gpi.h"


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
    Name: GPI_Init -
    Purpose: To initialize GPI module.
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void GPI_Init (void)
{
    UB8 CODE abGPI_18C_194[] = {
        GPI_SRC0        ,   // BITEK_18C_KEY_SRC0
        GPI_SRC1        ,   // BITEK_18D_KEY_SRC1
        GPI_ACK         ,   // BITEK_18E_KEY_ACK
        0x00            ,   // BITEK_18F_KEY_POL
        GPI_TYPE0       ,   // BITEK_190_KEY_TYPE0
        GPI_TYPE1       ,   // BITEK_191_KEY_TYPE1
        GPI_LONG_EN     ,   // BITEK_192_KEY_LONG_EN
        GPI_LONG_SEL    ,   // BITEK_193_KEY_LONG_SEL
        GPI_LONG_TIME   ,   // BITEK_194_KEY_ATTR
    };


    BITEK_TxBurst(BITEK_18C_KEY_SRC0, sizeof(abGPI_18C_194), abGPI_18C_194);

    #if (GPI_CFG == GPI_RIN)
    // RIN as GPI (Active Low)
    BITEK_TxByte(BITEK_0EA_RIN_PU, GPI_ACK);

    #elif (GPI_CFG == GPI_GIN)
    // GIN as GPI (Active Low)
    BITEK_TxByte(BITEK_0ED_GIN_PU, GPI_ACK);

    #elif (GPI_CFG == GPI_BIN)
    // BIN as GPI (Active Low)
    BITEK_TxByte(BITEK_0F0_BIN_PU, GPI_ACK);

    #elif (GPI_CFG == TOUT_TOUT)
    #endif // GPI_CFG

} /* GPI_Init */


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
