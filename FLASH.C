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

    Module: FLASH.C -

    Purpose: Implementation of FLASH.

    Version: 0.01                                   03:22PM  2014/05/23

    Compiler: Keil 8051 C Compiler v9.52

    Reference:

   ----------------------------------------------------------------------
    Modification:

    v0.01 03:22PM  2014/05/23 Jeffrey Chang
    Reason:
        1. Original.
    Solution:

   ********************************************************************** */

#define _FLASH_C_


/* ------------------------------------
    Header Files
   ------------------------------------ */
#include "bit1618c.h"
#include "bitek.h"
#include "common.h"
#include "flash.h"
#include "osd.h"
#include "platform.h"
#include "vp.h"


/* ------------------------------------
    Macro Definitions
   ------------------------------------ */
#define SPI_WRPP	0x02				// Page Program
#define SPI_WRITE	0x02				// Write to Memory instruction
#define SPI_WRSR	0x01				// Write Status Register instruction
#define SPI_WREN	0x06				// Write enable instruction
#define SPI_WRDI	0x04				// Write Disable instruction
#define SPI_QREAD	0x6B				// Read Quad Output from Memory instruction
#define SPI_FREAD	0x0B				// Fast Read from Memory instruction
#define SPI_READ	0x03				// Read from Memory instruction
#define SPI_RDSR	0x05				// Read Status Register instruction
#define SPI_RDID	0x9F				// Read identification
#define SPI_SE		0xD8				// Sector Erase instruction (32KB)
#define SPI_BES		0x20				// Bulk Erase instruction (4KB)
#define SPI_BEB		0x52				// Bulk Erase instruction (32KB)
#define SPI_BE		0xC7				// Bulk Erase instruction
#define SPI_CE		0x60				// Chip Erase instruction
#define SPI_LCE		0x60				// Legacy Chip Erase instruction
#define SPI_DP		0xB9				// Deep Power-down instruction
#define SPI_RES		0xAB				// Release Deep Power-down instruction
#define SPI_WOTP	0x9B				// Program OTP Security Register
#define SPI_ROTP	0x77				// Read OTP Security Register
#define RDSR_WIP	0x01				// Write In Progress (WIP) flag
#define RDSR_WEL	0x02				// Write Enable Lanch flag
#define RDSR_BP0	0x04				// Blcok protect 0
#define RDSR_BP1	0x08				// Blcok protect 1
#define RDSR_BP2	0x10				// Blcok protect 2
#define RDSR_EPE	0x20				// Erase/Program Error
#define RDSR_WD		0x80				// Status Register write protect


#define DMA_SPI_READ_EN	      (BITEK_MASK_DMA_EN  + BITEK_MASK_DMA_SPI_BUS + BITEK_MASK_DMA_READ_MODE)
#define DMA_SPI_READ_DIS	  (BITEK_MASK_DMA_DIS + BITEK_MASK_DMA_SPI_BUS + BITEK_MASK_DMA_READ_MODE)

#define DMA_SPI_WRITE_EN	  (BITEK_MASK_DMA_EN  + BITEK_MASK_DMA_SPI_BUS + BITEK_MASK_DMA_WRITE_MODE + BITEK_MASK_DMA_WAITRDY_EN + BITEK_MASK_DMA_RSTATUS_EN + BITEK_MASK_DMA_WENABLE_EN)
#define DMA_SPI_WRITE_DIS     (BITEK_MASK_DMA_DIS + BITEK_MASK_DMA_SPI_BUS + BITEK_MASK_DMA_WRITE_MODE + BITEK_MASK_DMA_WAITRDY_EN + BITEK_MASK_DMA_RSTATUS_EN + BITEK_MASK_DMA_WENABLE_EN)


/* ------------------------------------
    Type Definitions
   ------------------------------------ */


/* ------------------------------------
    Variables Definitions
   ------------------------------------ */

/* ------------------------------------
    Function Prototypes
   ------------------------------------ */


#if (FLASH_CHECK_STATUS)
/* -------------------------------------------------------------------
    Name: FLASH_CheckStatus -
    Purpose:
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void FLASH_CheckStatus (void)
{
	UB8 bStatus;


	EA = 0;

	for (;;)
	{
		FLASH_Delay(10);

		// FLASH_READ_STATUS
        // (OPCODE) + STATUS
		FLASH_SetCommand(SPI_RDSR, 0x03, DMA_SPI_READ_EN, DMA_SPI_READ_DIS);

		bStatus = BITEK_RxByte(BITEK_171_DMA_OUT);


	    #if (FLASH_CFG == FLASH_AT25F512B)

		if (((bStatus & RDSR_WIP) == 0x00)     ||
            ((bStatus & RDSR_EPE) == RDSR_EPE)     )
		{
			EA = 1;
			return;
		}

        #else

		if ((bStatus & RDSR_WIP) == 0x00)
		{

			EA = 1;
			return;
		}

	    #endif

	} // forever
}
#endif


#if (FLASH_DELAY)
/* -------------------------------------------------------------------
    Name: FLASH_Delay -
    Purpose:
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void FLASH_Delay (UW16 wTime)
{
	UB8 bIdx;


	while (wTime--)
	    bIdx++;
}
#endif


#if (FLASH_ERASE_SECTOR)
/* -------------------------------------------------------------------
    Name: FLASH_EraseSector -
    Purpose: To erase a BLOCK (4KBytes).
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void FLASH_EraseSector (UL32 dwAddr)
{
	FLASH_CheckStatus();

	// FLASH_WRITE_ENABLE: (OPCODE)
	FLASH_SetCommand(SPI_WREN, 0x00, DMA_SPI_READ_EN, DMA_SPI_READ_DIS);

	FLASH_SetAddrCnt(dwAddr, 0x00);

	// FLASH_SECTOR_ERASE
    // BLOCK ERASE: (OPCODE) + ADDRESS BITS A23-A0
	#if (FLASH_CFG == FLASH_AT25F512A)
	FLASH_SetCommand(SPI_BEB, 0x01, 0x2F, 0x2E);
	#else
   	FLASH_SetCommand(SPI_BES, 0x01, 0x2F, 0x2E);
	#endif

	FLASH_CheckStatus();
}
#endif


#if (FLASH_INIT)
/* -------------------------------------------------------------------
    Name: FLASH_Init -
    Purpose:
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void FLASH_Init (void)
{
	BOOL	fDataExist = FALSE;


//  #if ((FLASH_CFG == FLASH_W25X40B) && FLASH_SOFT_WP)
//	FLASH_WriteStatus();
//	#endif

    // Check VERSION_ID
    #if (DEBUG_FLASH)
    OSD_ShowHex(VP_SHOW_DEBUG3+12, FLASH_ReadByte(FGD_ADDR_VERSION_ID));
    #endif

    if (FLASH_ReadByte(FGD_ADDR_VERSION_ID) == VERSION_ID)
    {
        for (tsGD.bIndex = 0;
             FLASH_ReadByte(FGD_ADDR_INDEX) < FLASH_EMPTY_MARK;
             tsGD.bIndex++                                      )
        {
            // if it is VALID value !

            #if (DEBUG_FLASH)
            OSD_ShowHex(VP_SHOW_DEBUG3+ 9, FLASH_ReadByte(FGD_ADDR_INDEX));
            #endif

    		fDataExist = TRUE;
        }
    }

	if (fDataExist)
	{
		tsGD.bIndex--;

		FLASH_ReadAll();

        #if (DEBUG_FLASH)
        OSD_ShowHex(VP_SHOW_FLASH0+ 0, tsGD.bIndex      );
        OSD_ShowHex(VP_SHOW_FLASH0+ 3, tsGD.bBrightness );
        OSD_ShowHex(VP_SHOW_FLASH0+ 6, tsGD.bContrast   );
        OSD_ShowHex(VP_SHOW_FLASH0+ 9, tsGD.bSaturation );
        OSD_ShowHex(VP_SHOW_FLASH0+12, tsGD.bSource     );
        BITEK_TxByte(BITEK_11D_OSD1_ATTR2, OSD1_ON);
        #endif
	}
	else
	{
        tsGD = tsGD_Default;

		FLASH_WriteAll();

        #if (DEBUG_FLASH)
        OSD_ShowHex(VP_SHOW_FLASH1+ 0, tsGD.bIndex      );
        OSD_ShowHex(VP_SHOW_FLASH1+ 3, tsGD.bBrightness );
        OSD_ShowHex(VP_SHOW_FLASH1+ 6, tsGD.bContrast   );
        OSD_ShowHex(VP_SHOW_FLASH1+ 9, tsGD.bSaturation );
        OSD_ShowHex(VP_SHOW_FLASH1+12, tsGD.bSource     );
        BITEK_TxByte(BITEK_11D_OSD1_ATTR2, OSD1_ON);
        #endif
	}
}
#endif


#if (FLASH_READ_ALL)
/* -------------------------------------------------------------------
    Name: FLASH_ReadAll -
    Purpose:
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void FLASH_ReadAll (void)
{
    #if (FLASH_READ_BURST)
      //FLASH_ReadBurst(FGD_ADDR_INDEX, sizeof(tsGD), 0x7C00);
        FLASH_ReadBurst(FGD_ADDR_BRIGTNESS, 4, 0x7C01);

    #else
      //tsGD.bIndex   	    = FLASH_ReadByte(FGD_ADDR_INDEX         );

    	tsGD.bBrightness    = FLASH_ReadByte(FGD_ADDR_BRIGTNESS     );
    	tsGD.bContrast      = FLASH_ReadByte(FGD_ADDR_CONTRAST      );
    	tsGD.bSaturation    = FLASH_ReadByte(FGD_ADDR_SATURATION    );
    	tsGD.bSource        = FLASH_ReadByte(FGD_ADDR_SOURCE        );
    #endif

    #if 0
	tsGD.bSharpness     = FLASH_ReadByte(FGD_ADDR_SHARPNESS     );
	tsGD.bAspectRatio   = FLASH_ReadByte(FGD_ADDR_ASPECT_RATIO  );
	tsGD.bMirrorFlip    = FLASH_ReadByte(FGD_ADDR_MIRROR_FLIP   );
	tsGD.bSARADC        = FLASH_ReadByte(FGD_ADDR_SARADC        );
    tsGD.bFont          = FLASH_ReadByte(FGD_ADDR_FONT          );
    #endif
}
#endif


#if (FLASH_READ_BURST)
/* -------------------------------------------------------------------
    Name: FLASH_ReadBurst - From FLASH to XRAM.
    Purpose:
    Passed: wCnt must be larger then 2 (wCnt >= 2).
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void FLASH_ReadBurst (UL32 dwAddr, UW16 wCnt, UW16 wDstAddr)
{
	wCnt--;

	BITEK_TxByte(BITEK_166_DMA_COUNT07_00, wCnt        & 0xFF);
	BITEK_TxByte(BITEK_167_DMA_COUNT15_08, (wCnt >> 8) & 0xFF);

    BITEK_TxByte(BITEK_168_DMA_EADDR07_00, dwAddr         & 0xFF);
    BITEK_TxByte(BITEK_169_DMA_EADDR15_08, (dwAddr >>  8) & 0xFF);
    BITEK_TxByte(BITEK_16A_DMA_EADDR23_16, (dwAddr >> 16) & 0xFF);

	BITEK_TxByte(BITEK_16B_DMA_RADDR07_00, wDstAddr        & 0xFF);
	BITEK_TxByte(BITEK_16C_DMA_RADDR15_08, (wDstAddr >> 8) & 0xFF);

    BITEK_TxByte(BITEK_16E_DMA_OPCODE, SPI_READ);
  //BITEK_TxByte(BITEK_16F_DMA_ATTR, 0x85);             // 256 bytes, Cmd+Adr+Dat(R)
  //BITEK_TxByte(BITEK_16F_DMA_ATTR, 0x45);             //  16 bytes, Cmd+Adr+Dat(R)
    BITEK_TxByte(BITEK_16F_DMA_ATTR, 0x35);             //   8 bytes, Cmd+Adr+Dat(R)
	BITEK_TxByte(BITEK_170_DMA_ATTR1, DMA_SPI_READ_DIS);

    BITEK_TxByte(BITEK_170_DMA_ATTR1, DMA_SPI_READ_EN);

    while (DMA_BUSY)
        ;

	BITEK_TxByte(BITEK_170_DMA_ATTR1, DMA_SPI_READ_DIS);
}
#endif


#if (FLASH_READ_BYTE)
/* -------------------------------------------------------------------
    Name: FLASH_ReadByte -
    Purpose:
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
UB8 FLASH_ReadByte (UL32 dwAddr)
{
	UB8 bTmp;


    FLASH_SetAddrCnt(dwAddr, 0x00);

    // READ ARRAY (5 Bytes): (OPCODE) + ADDRESS BITS A23-A0 + DATA
	FLASH_SetCommand(SPI_READ, 0x05, DMA_SPI_READ_EN, DMA_SPI_READ_DIS);


	bTmp = BITEK_RxByte(BITEK_171_DMA_OUT);

	return bTmp;
}
#endif


#if (FLASH_SET_ADDR_CNT)
/* -------------------------------------------------------------------
    Name: FLASH_SetAddrCnt -
    Purpose:
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void FLASH_SetAddrCnt (
UL32 dwAddress,
UW16 wCnt
)
{
    // Counter
	BITEK_TxByte(BITEK_166_DMA_COUNT07_00, wCnt        & 0xFF);
	BITEK_TxByte(BITEK_167_DMA_COUNT15_08, (wCnt >> 8) & 0xFF);

    // Address
    BITEK_TxByte(BITEK_168_DMA_EADDR07_00, dwAddress         & 0xFF);
    BITEK_TxByte(BITEK_169_DMA_EADDR15_08, (dwAddress >>  8) & 0xFF);
    BITEK_TxByte(BITEK_16A_DMA_EADDR23_16, (dwAddress >> 16) & 0xFF);
}
#endif


#if (FLASH_SET_COMMAND)
/* -------------------------------------------------------------------
    Name: FLASH_SetCommand -
    Purpose:
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void FLASH_SetCommand (
UB8 bOPCode,
UB8 bCmdType,
UB8 bCtrl1,
UB8 bCtrl2
)
{
    BITEK_TxByte(BITEK_16E_DMA_OPCODE,  bOPCode);
    BITEK_TxByte(BITEK_16F_DMA_ATTR,    bCmdType);
    BITEK_TxByte(BITEK_170_DMA_ATTR1,   bCtrl1);

    while (SFR_DMA_BUSY)
	    ;

	BITEK_TxByte(BITEK_170_DMA_ATTR1,   bCtrl2);
}
#endif


#if (FLASH_WRITE_ALL)
/* -------------------------------------------------------------------
    Name: FLASH_WriteAll -
    Purpose:
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void FLASH_WriteAll (void)
{
    bit bInt;


	bInt = EA;

    EA = 0;

	tsGD.bIndex++;

	if (tsGD.bIndex >= FLASH_INDEX_MAX)
	{
        // Last Record is reserved as VERSION_ID !

		tsGD.bIndex = 0;
		FLASH_EraseSector(FGD_ADDR_BASE);

        // VERSION ID
    	FLASH_WriteByte(FGD_ADDR_VERSION_ID, VERSION_ID);

	}


    #if (FLASH_WRITE_BURST)

    FLASH_WriteBurst(FGD_ADDR_INDEX, sizeof(tsGD), (UB8*)&tsGD);

    #else

	FLASH_WriteByte(FGD_ADDR_INDEX,         tsGD.bIndex         );

	FLASH_WriteByte(FGD_ADDR_BRIGTNESS,     tsGD.bBrightness    );
	FLASH_WriteByte(FGD_ADDR_CONTRAST,      tsGD.bContrast      );
	FLASH_WriteByte(FGD_ADDR_SATURATION,    tsGD.bSaturation    );
	FLASH_WriteByte(FGD_ADDR_SOURCE,        tsGD.bSource        );

    #if 0
	FLASH_WriteByte(FGD_ADDR_SHARPNESS,     tsGD.bSharpness     );
	FLASH_WriteByte(FGD_ADDR_ASPECT_RATIO,  tsGD.bAspectRatio   );
	FLASH_WriteByte(FGD_ADDR_MIRROR_FLIP,   tsGD.bMirrorFlip    );
	FLASH_WriteByte(FGD_ADDR_SARADC,        tsGD.bSARADC        );
	FLASH_WriteByte(FGD_ADDR_FONT,          tsGD.bFont          );
    #endif

    #endif // FLASH_WRITE_BURST

	EA = bInt;
}
#endif


#if (FLASH_WRITE_BURST)
/* -------------------------------------------------------------------
    Name: FLASH_WriteBurst -
    Purpose:
    Passed: bCNT must be larger then 2 (bCNT >= 2).
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void FLASH_WriteBurst (UL32 dwAddr, UB8 bCNT, UB8 *pbData)
{
	FLASH_CheckStatus();

	FLASH_SetCommand(SPI_WREN, SPI_ONE_CMD, DMA_SPI_READ_EN, DMA_SPI_READ_DIS);

	FLASH_SetAddrCnt(dwAddr, bCNT-1);

	BITEK_TxByte(BITEK_15D_SPI_OPCODE, SPI_WRITE);
	BITEK_TxByte(BITEK_163_SPI_ATTR, SPI_FIVE_CMD);

	BITEK_TxByte(BITEK_15E_DMA_DIN, *pbData++);

	// FLASH_PROGRAM_CONT
	BITEK_TxByte(BITEK_165_DMA_ATTR, DMA_SPI_WRITE_EN + BITEK_MASK_SPI_HOLD_EN);

	while (--bCNT > 1)
	{
		BITEK_TxByte(BITEK_15E_DMA_DIN, *pbData++);

		// FLASH_PROGRAM_NEXT
		BITEK_TxByte(BITEK_165_DMA_ATTR, DMA_SPI_WRITE_EN + BITEK_MASK_SPI_HOLD_EN + BITEK_MASK_SPI_CONT_EN);
		BITEK_TxByte(BITEK_165_DMA_ATTR, DMA_SPI_WRITE_EN + BITEK_MASK_SPI_HOLD_EN);
	}

	BITEK_TxByte(BITEK_15E_DMA_DIN, *pbData);

	// FLASH_PROGRAM_END
	BITEK_TxByte(BITEK_165_DMA_ATTR, DMA_SPI_WRITE_EN + BITEK_MASK_SPI_CONT_EN);
	BITEK_TxByte(BITEK_165_DMA_ATTR, DMA_SPI_WRITE_DIS);

	FLASH_CheckStatus();
}
#endif


#if (FLASH_WRITE_BYTE)
/* -------------------------------------------------------------------
    Name: FLASH_WriteByte -
    Purpose:
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void FLASH_WriteByte (
UL32 dwAddr,
UB8  bData
)
{
	FLASH_CheckStatus();

	FLASH_SetAddrCnt(dwAddr, 0x00);

	// FLASH_WRITE_ENABLE (1 Byte): (OPCODE)
	FLASH_SetCommand(SPI_WREN, 0x00, DMA_SPI_READ_EN, DMA_SPI_READ_DIS);

  //BITEK_TxByte(BITEK_15E_DMA_DIN, bData);
	BITEK_TxByte(BITEK_16B_DMA_RADDR07_00, bData);

	// FLASH_PROGRAM_END
    // BYTE PROGRAM (5 Bytes): (OPCODE) + ADDRESS BITS A23-A0 + DATA-IN
	FLASH_SetCommand(SPI_WRITE, 0x04, 0x0F, 0x0E);

	FLASH_CheckStatus();
}
#endif


#if (FLASH_WRITE_STATUS)
#if ((FLASH_CFG == FLASH_W25X40B) && FLASH_SOFT_WP)
/* -------------------------------------------------------------------
    Name: FLASH_WriteStatus -
    Purpose:
    Passed: None.
    Returns: None.
    Notes:	 for softwave write-protect
   ------------------------------------------------------------------- */
void FLASH_WriteStatus(void)
{
   bit bInt = EA;


   EA  = 0;

   FLASH_CheckStatus();

   FLASH_SetCommand(SPI_WREN, 0x00, DMA_SPI_READ_EN, DMA_SPI_READ_DIS);
    
   BITEK_TxByte(BITEK_16D_DMA_DIN, 0x24);

   FLASH_SetCommand(SPI_WRSR, 0xF2, DMA_SPI_WRITE_EN, DMA_SPI_WRITE_DIS);

   FLASH_CheckStatus();

   EA  = bInt;
}
#endif
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
