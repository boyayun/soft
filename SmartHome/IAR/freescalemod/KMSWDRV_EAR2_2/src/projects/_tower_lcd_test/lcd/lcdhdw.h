
/*****************************************************************************
 * (c) Copyright 2010, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      LCDHDW.h
 * @author    R55013
 * @version   1.0.1.0
 * @date      Dec-6-2010
 * @brief     LCD driver configuration file for LCD type IS18080P..
 ******************************************************************************/
#ifndef __LCDHWD_H
    #define __LCDHWD_H

    #define DIM(x)                (sizeof(x)/sizeof(x[0]))

    #define _LCD_CHARNUM          (9)                   // number of LCD segment
    #define _LCDTYPE              (1)                   // 1 byte to complete one character
    #define  CharacterPlace(LCDn) *((unsigned char *)&LCD_WF3TO0 + LCDn)

/*
    #define _LCD_CHARNUM          (9)                   // number of LCD segment
    #define _LCDTYPE              (2)                   // 2 bytes to complete one character
    #define  CharacterPlace(LCDn) *((unsigned char *)&LCD_WF3TO0 + LCDn)
*/

    #define SymbolON(LCDn, mask)                                              \
    {                                                                         \
      *((unsigned char *)&LCD_WF3TO0 + LCDn) |= (1<<(mask)); ;                \
    }
    
    #define SymbolOFF(LCDn, mask)                                             \
    {                                                                         \
      *((unsigned char *)&LCD_WF3TO0 + LCDn) &= ~(1<<(mask));                 \
    }
    
    #define SegmentsON(LCDn, mask)                                            \
    {                                                                         \
      *((unsigned char *)&LCD_WF3TO0 + LCDn) |= (mask);                       \
    }

    #define SegmentsOFF(LCDn, mask)                                           \
    {                                                                         \
      *((unsigned char *)&LCD_WF3TO0 + LCDn) &= ~(mask);                      \
    }
    
    /* Indicate how character is connected to LCDn          */
    #define   Char0     CharacterPlace(27)
    #define   Char1     CharacterPlace(26)
    #define   Char2     CharacterPlace(25)
    #define   Char3     CharacterPlace(24)
    #define   Char4     CharacterPlace(23)
    #define   Char5     CharacterPlace(22)
    #define   Char6     CharacterPlace(21)
    #define   Char7     CharacterPlace(20)
    #define   Char8     CharacterPlace(19)

/*
    #define   Char4a    CharacterPlace(38)
    #define   Char4b    CharacterPlace(37)
    #define   Char5a    CharacterPlace(36)
    #define   Char5b    CharacterPlace(35)
    #define   Char6a    CharacterPlace(34)
    #define   Char6b    CharacterPlace(33)
    #define   Char7a    CharacterPlace(32)
    #define   Char7b    CharacterPlace(31)
    #define   Char8a    CharacterPlace(30)
    #define   Char8b    CharacterPlace(29)
    #define   Char9a    CharacterPlace(28)
    #define   Char9b    CharacterPlace(27)
    #define   Char10a   CharacterPlace(26)
    #define   Char10b   CharacterPlace(25)
    #define   Char11a   CharacterPlace(24)
    #define   Char11b   CharacterPlace(23)
    #define   Char12a   CharacterPlace(22)
    #define   Char12b   CharacterPlace(21)
*/
    /* Special symbols ON */
/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S3_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S3_ON    SymbolON(31,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S6_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S6_ON    SymbolON(31,5)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S7_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S7_ON    SymbolON(32,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S9_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S9_ON    SymbolON(31,4)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S11_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S11_ON   SymbolON(32,4)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S12_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S12_ON   SymbolON(32,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S13_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S13_ON   SymbolON(32,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_I_S8_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_I_S8_ON    SymbolON(31,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_I_S12_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_I_S12_ON   SymbolON(32,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_DOT_P3_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_DOT_P3_ON       SymbolON(18,1)

   
//------------------------------------------------------------------------------
/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_T_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_T_ON       SymbolON(38,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_COLS_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_COLS_ON         SymbolON(20,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_COMMA_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_COMMA_ON         SymbolON(20,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T1_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T1_ON        SymbolON(28,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T2_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T2_ON        SymbolON(26,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T3_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T3_ON        SymbolON(24,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T4_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T4_ON        SymbolON(22,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_kW_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_kW_ON        SymbolON(19,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_h_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_h_ON         SymbolON(17,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_BATT_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_BATT_ON      SymbolON(36,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_WRN_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_WRN_ON       SymbolON(34,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_LOCK_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_LOCK_ON      SymbolON(32,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_DOTS_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_OBIS_DOTS_ON SymbolON(19,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_ONE_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_ONE_ON      SymbolON(18,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D1B_C_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D1B_C_ON    SymbolON(20,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D1A_E_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D1A_E_ON    SymbolON(20,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D2B_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D2B_ON      SymbolON(19,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D2E_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D2E_ON      SymbolON(19,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3B_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3B_ON    SymbolON(19,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3A_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3A_ON         SymbolON(18,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3C_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3C_ON         SymbolON(18,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3D_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3D_ON         SymbolON(18,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3E_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3E_ON         SymbolON(17,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3F_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3F_ON         SymbolON(17,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3G_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3G_ON         SymbolON(17,2)


    /* Special symbols OFF */
/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S3_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S3_OFF       SymbolOFF(31,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S6_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S6_OFF       SymbolOFF(31,5)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S7_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S7_OFF       SymbolOFF(32,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S9_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S9_OFF       SymbolOFF(31,4)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S11_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S11_OFF      SymbolOFF(32,4)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S12_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S12_OFF      SymbolOFF(32,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S1_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S13_OFF      SymbolOFF(32,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_I_S8_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_I_S8_OFF       SymbolOFF(31,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_I_S12_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_I_S12_OFF      SymbolOFF(32,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_DOT_P3_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_DOT_P3_OFF          SymbolOFF(18,1)

   
//------------------------------------------------------------------------------
/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_T_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_T_OFF        SymbolOFF(38,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_COLS_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_COLS_OFF         SymbolOFF(20,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_COMMA_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_COMMA_OFF         SymbolOFF(20,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T1_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T1_OFF        SymbolOFF(28,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T2_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T2_OFF        SymbolOFF(26,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T3_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T3_OFF        SymbolOFF(24,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T4_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T4_OFF        SymbolOFF(22,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_kW_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_kW_OFF        SymbolOFF(19,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_h_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_h_OFF         SymbolOFF(17,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_BATT_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_BATT_OFF      SymbolOFF(36,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_WRN_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_WRN_OFF       SymbolOFF(34,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_LOCK_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_LOCK_OFF      SymbolOFF(32,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_DOTS_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_OBIS_DOTS_OFF  SymbolOFF(19,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_ONE_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_ONE_OFF       SymbolOFF(18,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D1B_C_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D1B_C_OFF     SymbolOFF(20,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D1A_E_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D1A_E_OFF     SymbolOFF(20,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D2B_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D2B_OFF       SymbolOFF(19,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D2E_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D2E_OFF       SymbolOFF(19,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3B_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3B_OFF       SymbolOFF(19,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3A_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3A_OFF         SymbolOFF(18,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3C_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3C_OFF         SymbolOFF(18,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3D_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3D_OFF         SymbolOFF(18,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3E_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3E_OFF         SymbolOFF(17,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3F_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3F_OFF         SymbolOFF(17,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3G_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3G_OFF         SymbolOFF(17,2)

    /* Segments ON  */
    #define Segment38_ON    SegmentsON ( 38,0x0f )
    #define Segment37_ON    SegmentsON ( 37,0x0f )
    #define Segment36_ON    SegmentsON ( 36,0x0f )
    #define Segment35_ON    SegmentsON ( 35,0x0f )
    #define Segment34_ON    SegmentsON ( 34,0x0f )
    #define Segment33_ON    SegmentsON ( 33,0x0f )
    #define Segment32_ON    SegmentsON ( 32,0x0f )
    #define Segment31_ON    SegmentsON ( 31,0x0f )
    #define Segment30_ON    SegmentsON ( 30,0x0f )
    #define Segment29_ON    SegmentsON ( 29,0x0f )
    #define Segment28_ON    SegmentsON ( 28,0x0f )
    #define Segment27_ON    SegmentsON ( 27,0x0f )
    #define Segment26_ON    SegmentsON ( 26,0x0f )
    #define Segment25_ON    SegmentsON ( 25,0x0f )
    #define Segment24_ON    SegmentsON ( 24,0x0f )
    #define Segment23_ON    SegmentsON ( 23,0x0f )
    #define Segment22_ON    SegmentsON ( 22,0x0f )
    #define Segment21_ON    SegmentsON ( 21,0x0f )
    #define Segment20_ON    SegmentsON ( 20,0x0f )
    #define Segment19_ON    SegmentsON ( 19,0x0f )
    #define Segment18_ON    SegmentsON ( 18,0x0f )
    #define Segment17_ON    SegmentsON ( 17,0x0f )


     /* Segments OFF  */
    #define Segment38_OFF    SegmentsOFF(38,0x0f )
    #define Segment37_OFF    SegmentsOFF(37,0x0f )
    #define Segment36_OFF    SegmentsOFF(36,0x0f )
    #define Segment35_OFF    SegmentsOFF(35,0x0f )
    #define Segment34_OFF    SegmentsOFF(34,0x0f )
    #define Segment33_OFF    SegmentsOFF(33,0x0f )
    #define Segment32_OFF    SegmentsOFF(32,0x0f )
    #define Segment31_OFF    SegmentsOFF(31,0x0f )
    #define Segment30_OFF    SegmentsOFF(30,0x0f )
    #define Segment29_OFF    SegmentsOFF(29,0x0f )
    #define Segment28_OFF    SegmentsOFF(28,0x0f )
    #define Segment27_OFF    SegmentsOFF(27,0x0f )
    #define Segment26_OFF    SegmentsOFF(26,0x0f )
    #define Segment25_OFF    SegmentsOFF(25,0x0f )
    #define Segment24_OFF    SegmentsOFF(24,0x0f )
    #define Segment23_OFF    SegmentsOFF(23,0x0f )
    #define Segment22_OFF    SegmentsOFF(22,0x0f )
    #define Segment21_OFF    SegmentsOFF(21,0x0f )
    #define Segment20_OFF    SegmentsOFF(20,0x0f )
    #define Segment19_OFF    SegmentsOFF(19,0x0f )
    #define Segment18_OFF    SegmentsOFF(18,0x0f )
    #define Segment17_OFF    SegmentsOFF(17,0x0f )

    /*Map segment to COM mask - LINE0*/
    #define  L0SEGA   0x02
    #define  L0SEGB   0x08
    #define  L0SEGC   0x40
    #define  L0SEGD   0x80
    #define  L0SEGE   0x20
    #define  L0SEGF   0x04
    #define  L0SEGG   0x10
    

    /*Map segment to COM mask - LINE0*/
/*
    #define  L0SEGA   0x02
    #define  L0SEGB   0x04
    #define  L0SEGC   0x08
    #define  L0SEGD   0x01
    #define  L0SEGE   0x02
    #define  L0SEGF   0x04
    #define  L0SEGG   0x08
*/

#endif /* __LCDHWD_H */ 