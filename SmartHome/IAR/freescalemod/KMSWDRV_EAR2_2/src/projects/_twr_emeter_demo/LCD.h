/******************************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved                       
*
*******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************//*!
*
* @file      LCD.h
* @author    b19186
* @version   0.0.2.0
* @date      April-11-2011
* @brief     Driver header for LCD on TWR board, type S-Tek Displays GDH-1247WP
*
******************************************************************************/
#ifndef __LCD_H
#define __LCD_H

    #define _LCD_CHARNUM1  (9) /* max. number of upper (main) LCD segments */
    #define _LCD_CHARNUM2  (5) /* max. number of lower (aux.) LCD segments */
    #define _MAX_NUM_DIGITS1  (_LCD_CHARNUM1) /* max. number of LCD digits for upper segments */
    #define _MAX_NUM_DIGITS2  (_LCD_CHARNUM2) /* max. number of LCD digits for lower segments */
    #define _LCDTYPE (1)       /* 1 byte to complete one character (depends on the LCD) */

    #define DIM(x)                (sizeof(x)/sizeof(x[0]))
    
    #define  CharacterPlace(LCDn) *((unsigned char *)&LCD_WF3TO0 + LCDn)

    /* Indicate how Character (digits) are connected to the LCD and MCU pins */
    #define   Char1        CharacterPlace (27)   // MCU's LCD pin 27, LCD Pin 10
    #define   Char2        CharacterPlace (26)   // MCU's LCD pin 26, LCD Pin 11
    #define   Char3        CharacterPlace (25)   // MCU's LCD pin 25, LCD Pin 12
    #define   Char4        CharacterPlace (24)   // MCU's LCD pin 24, LCD Pin 13
    #define   Char5        CharacterPlace (23)   // MCU's LCD pin 23, LCD Pin 14
    #define   Char6        CharacterPlace (22)   // MCU's LCD pin 22, LCD Pin 15
    #define   Char7        CharacterPlace (21)   // MCU's LCD pin 21, LCD Pin 16
    #define   Char8        CharacterPlace (20)   // MCU's LCD pin 20, LCD Pin 17
    #define   Char9        CharacterPlace (19)   // MCU's LCD pin 19, LCD Pin 18
    #define   Char10       CharacterPlace (34)   // MCU's LCD pin 34, LCD Pin 5
    #define   Char11       CharacterPlace (35)   // MCU's LCD pin 35, LCD Pin 4
    #define   Char12       CharacterPlace (36)   // MCU's LCD pin 36, LCD Pin 3
    #define   Char13       CharacterPlace (37)   // MCU's LCD pin 37, LCD Pin 2
    #define   Char14       CharacterPlace (43)   // MCU's LCD pin 43, LCD Pin 1
    #define   Char15       CharacterPlace (33)   // MCU's LCD pin 33, LCD Pin 6
    
   /* Special symbols ON */
    #define _LCD_TAMPER_ON()          SymbolON(19,0)   
    #define _LCD_RF_ON()              SymbolON(32,0)   
    #define _LCD_CLOCK_ON()           SymbolON(17,0)   
    #define _LCD_BATT1_ON()           SymbolON(31,6)   
    #define _LCD_BATT2_ON()           SymbolON(31,7)
    #define _LCD_RS_ON()              SymbolON(28,7)
    #define _LCD_COS_ON()             SymbolON(32,7)
    #define _LCD_IND_ON()             SymbolON(28,6)
    #define _LCD_CAP_ON()             SymbolON(28,5)
    #define _LCD_L1_ON()              SymbolON(23,0)
    #define _LCD_L2_ON()              SymbolON(22,0)
    #define _LCD_L3_ON()              SymbolON(21,0)
    #define _LCD_T1_ON()              SymbolON(17,2)
    #define _LCD_T2_ON()              SymbolON(17,3)
    #define _LCD_T3_ON()              SymbolON(17,4)
    #define _LCD_T4_ON()              SymbolON(17,5)
    #define _LCD_I1_ON()              SymbolON(28,3)
    #define _LCD_I2_ON()              SymbolON(28,0)
    #define _LCD_I3_ON()              SymbolON(25,0)
    #define _LCD_WARNING_ON()         SymbolON(20,0)
    #define _LCD_I1_LEFT_ON()         SymbolON(28,4)
    #define _LCD_I1_RIGHT_ON()        SymbolON(28,2)
    #define _LCD_I2_LEFT_ON()         SymbolON(28,1)
    #define _LCD_I2_RIGHT_ON()        SymbolON(27,0)
    #define _LCD_I3_LEFT_ON()         SymbolON(26,0)
    #define _LCD_I3_RIGHT_ON()        SymbolON(24,0)
    #define _LCD_I_ON()               SymbolON(33,7)
    #define _LCD_C_ON()               SymbolON(34,0)
    #define _LCD_P1_ON()              SymbolON(18,7)   
    #define _LCD_P2_ON()              SymbolON(18,6)   
    #define _LCD_P3_ON()              SymbolON(18,1)   
    #define _LCD_P4_ON()              SymbolON(35,0)   
    #define _LCD_P5_ON()              SymbolON(36,0)   
    #define _LCD_P6_ON()              SymbolON(37,0)   
    #define _LCD_P7_ON()              SymbolON(43,0)   
    #define _LCD_MINUS_ON()           SymbolON(27,4)    
    #define _LCD_RMS_ON()             SymbolON(17,1)    
    #define _LCD_HZ_ON()              SymbolON(17,6)    
    #define _LCD_Volts_ON()           SymbolON(18,5)   
    #define _LCD_Amp_ON()             SymbolON(18,4)
    #define _LCD_VA_ON()              {SymbolON(18,4);SymbolON(18,5);}
    #define _LCD_R_ON()               {SymbolON(32,4);SymbolON(31,2);SymbolON(32,2);SymbolON(31,3);SymbolON(31,5);SymbolON(32,5);SymbolON(32,6);}
    #define _LCD_KW_ON()              SymbolON(18,3)
    #define _LCD_KWh_ON()             {SymbolON(18,3); SymbolON(18,2);}  
    #define _LCD_KVArh_ON()           {SymbolON(18,5); SymbolON(18,4); SymbolON(17,7);}  
    #define _LCD_OBIS_ON(num)         {vfnLCD_Write_Num(##num*10,4,2);vfnLCD_Write_Char(':',13);_LCD_P5_ON();_LCD_P6_ON();}
  
    /* Special symbols OFF   */
    #define _LCD_TAMPER_OFF()          SymbolOFF(19,0)   
    #define _LCD_RF_OFF()              SymbolOFF(32,0)   
    #define _LCD_CLOCK_OFF()           SymbolOFF(17,0)   
    #define _LCD_BATT1_OFF()           SymbolOFF(31,6)   
    #define _LCD_BATT2_OFF()           SymbolOFF(31,7)
    #define _LCD_RS_OFF()              SymbolOFF(28,7)
    #define _LCD_COS_OFF()             SymbolOFF(32,7)
    #define _LCD_IND_OFF()             SymbolOFF(28,6)
    #define _LCD_CAP_OFF()             SymbolOFF(28,5)
    #define _LCD_L1_OFF()              SymbolOFF(23,0)
    #define _LCD_L2_OFF()              SymbolOFF(22,0)
    #define _LCD_L3_OFF()              SymbolOFF(21,0)
    #define _LCD_T1_OFF()              SymbolOFF(17,2)
    #define _LCD_T2_OFF()              SymbolOFF(17,3)
    #define _LCD_T3_OFF()              SymbolOFF(17,4)
    #define _LCD_T4_OFF()              SymbolOFF(17,5)
    #define _LCD_I1_OFF()              SymbolOFF(28,3)
    #define _LCD_I2_OFF()              SymbolOFF(28,0)
    #define _LCD_I3_OFF()              SymbolOFF(25,0)
    #define _LCD_WARNING_OFF()         SymbolOFF(20,0)
    #define _LCD_I1_LEFT_OFF()         SymbolOFF(28,4)
    #define _LCD_I1_RIGHT_OFF()        SymbolOFF(28,2)
    #define _LCD_I2_LEFT_OFF()         SymbolOFF(28,1)
    #define _LCD_I2_RIGHT_OFF()        SymbolOFF(27,0)
    #define _LCD_I3_LEFT_OFF()         SymbolOFF(26,0)
    #define _LCD_I3_RIGHT_OFF()        SymbolOFF(24,0)
    #define _LCD_I_OFF()               SymbolOFF(33,7)
    #define _LCD_C_OFF()               SymbolOFF(34,0)
    #define _LCD_P1_OFF()              SymbolOFF(18,7)   
    #define _LCD_P2_OFF()              SymbolOFF(18,6)   
    #define _LCD_P3_OFF()              SymbolOFF(18,1)   
    #define _LCD_P4_OFF()              SymbolOFF(35,0)   
    #define _LCD_P5_OFF()              SymbolOFF(36,0)   
    #define _LCD_P6_OFF()              SymbolOFF(37,0)   
    #define _LCD_P7_OFF()              SymbolOFF(43,0)   
    #define _LCD_MINUS_OFF()           SymbolOFF(27,4)    
    #define _LCD_RMS_OFF()             SymbolOFF(17,1)    
    #define _LCD_HZ_OFF()              SymbolOFF(17,6)    
    #define _LCD_Volts_OFF()           SymbolOFF(18,5)   
    #define _LCD_Amp_OFF()             SymbolOFF(18,4)
    #define _LCD_VA_OFF()              {SymbolOFF(18,4);SymbolOFF(18,5);}
    #define _LCD_R_OFF()               {SymbolOFF(31,2);SymbolOFF(32,2);SymbolOFF(31,3);SymbolOFF(31,5);SymbolOFF(32,5);SymbolOFF(32,4);SymbolOFF(32,6);}
    #define _LCD_KW_OFF()              SymbolOFF(18,3)
    #define _LCD_KWh_OFF()             {SymbolOFF(18,3); SymbolOFF(18,2);}  
    #define _LCD_KVArh_OFF()           {SymbolOFF(18,5); SymbolOFF(18,4); SymbolOFF(17,7);}  
    #define _LCD_OBIS_OFF()            {vfnLCD_Write_Str("     ",2);_LCD_P5_OFF();_LCD_P6_OFF();}

//------------------------------------------------------------------------------
 /* Special symbols ON */
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S3_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S3_ON    SymbolON(31,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S6_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S6_ON    SymbolON(31,5)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S7_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S7_ON    SymbolON(32,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S9_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S9_ON    SymbolON(31,4)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S11_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S11_ON   SymbolON(32,4)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S12_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S12_ON   SymbolON(32,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S13_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S13_ON   SymbolON(32,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_I_S8_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_I_S8_ON    SymbolON(31,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_I_S12_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_I_S12_ON   SymbolON(32,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_DOT_P3_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_DOT_P3_ON       SymbolON(18,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_T1_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_T1_ON           SymbolON(17,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_T2_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_T2_ON           SymbolON(17,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_T3_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_T3_ON           SymbolON(17,4)

/***************************************************************************//*!
 * @brief   Macro displays LCD_RMS_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_T4_ON           SymbolON(17,5)

/***************************************************************************//*!
 * @brief   Macro displays LCD_RMS_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_RMS_ON          SymbolON(17,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_HZ_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_HZ_ON           SymbolON(17,6)

//------------------------------------------------------------------------------
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_T_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_T_ON       SymbolON(38,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_COLS_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_COLS_ON         SymbolON(20,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_COMMA_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_COMMA_ON         SymbolON(20,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_T1_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_T1_ON        SymbolON(28,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_T2_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_T2_ON        SymbolON(26,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_T3_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_T3_ON        SymbolON(24,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_T4_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_T4_ON        SymbolON(22,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_kW_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_kW_ON        SymbolON(19,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_h_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_h_ON         SymbolON(17,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_BATT_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_BATT_ON      SymbolON(36,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_WARN_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_WARN_ON       SymbolON(34,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_LOCK_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_LOCK_ON      SymbolON(32,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_DOTS_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_OBIS_DOTS_ON SymbolON(19,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_ONE_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_ONE_ON      SymbolON(18,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D1B_C_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D1B_C_ON    SymbolON(20,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D1A_E_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D1A_E_ON    SymbolON(20,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D2B_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D2B_ON      SymbolON(19,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D2E_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D2E_ON      SymbolON(19,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3B_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3B_ON    SymbolON(19,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3A_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3A_ON         SymbolON(18,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3C_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3C_ON         SymbolON(18,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3D_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3D_ON         SymbolON(18,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3E_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3E_ON         SymbolON(17,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3F_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3F_ON         SymbolON(17,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3G_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3G_ON         SymbolON(17,2)


    /* Special symbols OFF */
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S3_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S3_OFF       SymbolOFF(31,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S6_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S6_OFF       SymbolOFF(31,5)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S7_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S7_OFF       SymbolOFF(32,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S9_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S9_OFF       SymbolOFF(31,4)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S11_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S11_OFF      SymbolOFF(32,4)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S12_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S12_OFF      SymbolOFF(32,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_V_S1_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_V_S13_OFF      SymbolOFF(32,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_I_S8_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_I_S8_OFF       SymbolOFF(31,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_I_S12_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_I_S12_OFF      SymbolOFF(32,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_DOT_P3_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_DOT_P3_OFF          SymbolOFF(18,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_T1_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_T1_OFF              SymbolOFF(17,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_T2_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_T2_OFF              SymbolOFF(17,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_T3_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_T3_OFF              SymbolOFF(17,4)

/***************************************************************************//*!
 * @brief   Macro displays LCD_T4_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_T4_OFF              SymbolOFF(17,5)

/***************************************************************************//*!
 * @brief   Macro displays LCD_RMS_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_RMS_OFF             SymbolOFF(17,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_HZ_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_HZ_OFF              SymbolOFF(17,6)
   
//------------------------------------------------------------------------------
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_T_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_CHAR_T_OFF        SymbolOFF(38,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_COLS_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_COLS_OFF         SymbolOFF(20,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_COMMA_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_COMMA_OFF         SymbolOFF(20,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_T1_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_T1_OFF        SymbolOFF(28,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_T2_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_T2_OFF        SymbolOFF(26,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_T3_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_T3_OFF        SymbolOFF(24,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_T4_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_T4_OFF        SymbolOFF(22,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_kW_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_kW_OFF        SymbolOFF(19,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_h_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_h_OFF         SymbolOFF(17,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_BATT_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_BATT_OFF      SymbolOFF(36,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_WARN_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_WARN_OFF       SymbolOFF(34,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_LOCK_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_LOCK_OFF      SymbolOFF(32,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_DOTS_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define LCD_SYM_OBIS_DOTS_OFF  SymbolOFF(19,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_ONE_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_ONE_OFF       SymbolOFF(18,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D1B_C_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D1B_C_OFF     SymbolOFF(20,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D1A_E_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D1A_E_OFF     SymbolOFF(20,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D2B_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D2B_OFF       SymbolOFF(19,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D2E_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D2E_OFF       SymbolOFF(19,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3B_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3B_OFF       SymbolOFF(19,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3A_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3A_OFF         SymbolOFF(18,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3C_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3C_OFF         SymbolOFF(18,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3D_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3D_OFF         SymbolOFF(18,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3E_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3E_OFF         SymbolOFF(17,0)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3F_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3F_OFF         SymbolOFF(17,1)

/***************************************************************************//*!
 * @brief   Macro displays LCD_SYM_OBIS_D3G_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_SYM_OBIS_D3G_OFF         SymbolOFF(17,2)
   
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_P_S12_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_P_S12_OFF           SymbolOFF(32,2)
   
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_P_S12_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_P_S12_ON            SymbolON(32,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_P_S8_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_P_S8_ON    SymbolON(31,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_P_S8_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_P_S8_OFF    SymbolOFF(31,2)
   
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_P_S6_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_P_S6_ON    SymbolON(31,5)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_P_S6_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_P_S6_OFF    SymbolOFF(31,5)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_P_S11_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_P_S11_ON   SymbolON(32,4)
   
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_P_S11_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_P_S11_OFF   SymbolOFF(32,4)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_P_S5_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_P_S5_ON   SymbolON(31,3)
   
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_P_S5_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_P_S5_OFF   SymbolOFF(31,3)
   
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_P_S10_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_P_S10_ON   SymbolON(32,5)
   
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_P_S10_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_P_S10_OFF   SymbolOFF(32,5)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_F_S5_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_F_S5_ON   SymbolON(31,3)
   
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_F_S5_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_F_S5_OFF   SymbolOFF(31,3)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_F_S6_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_F_S6_ON    SymbolON(31,5)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_F_S6_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_F_S6_OFF    SymbolOFF(31,5)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_F_S8_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_F_S8_ON    SymbolON(31,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_F_S8_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_F_S8_OFF    SymbolOFF(31,2)

/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_F_S11_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_F_S11_ON   SymbolON(32,4)
   
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_F_S11_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_F_S11_OFF   SymbolOFF(32,4)   
   
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_F_S12_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_F_S12_ON            SymbolON(32,2)
   
/***************************************************************************//*!
 * @brief   Macro displays LCD_CHAR_F_S12_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define LCD_CHAR_F_S12_OFF           SymbolOFF(32,2)


// Tower LCD large V,I,R symbols
//------------------------------------------------------------------------------
#define _LCD_BIG_V_ON() LCD_CHAR_V_S3_ON; LCD_CHAR_V_S6_ON; \
                        LCD_CHAR_V_S7_ON; LCD_CHAR_V_S9_ON; LCD_CHAR_V_S11_ON;\
                        LCD_CHAR_V_S12_ON; LCD_CHAR_V_S13_ON;
                        
#define _LCD_BIG_V_OFF() LCD_CHAR_V_S3_OFF; LCD_CHAR_V_S6_OFF; \
                         LCD_CHAR_V_S7_OFF; LCD_CHAR_V_S9_OFF; LCD_CHAR_V_S11_OFF;\
                         LCD_CHAR_V_S12_OFF; LCD_CHAR_V_S13_OFF;
                         
#define _LCD_BIG_I_ON()  LCD_CHAR_I_S8_ON; LCD_CHAR_I_S12_ON;                         
                         
#define _LCD_BIG_I_OFF() LCD_CHAR_I_S8_OFF; LCD_CHAR_I_S12_OFF;
                         
#define _LCD_BIG_P_ON()  LCD_CHAR_P_S5_ON; LCD_CHAR_P_S6_ON; LCD_CHAR_P_S8_ON;\
                         LCD_CHAR_P_S10_ON; LCD_CHAR_P_S11_ON;LCD_CHAR_P_S12_ON; 
                         
#define _LCD_BIG_P_OFF() LCD_CHAR_P_S5_OFF;LCD_CHAR_P_S6_OFF;LCD_CHAR_P_S8_OFF;\
                         LCD_CHAR_P_S10_OFF;LCD_CHAR_P_S11_OFF;LCD_CHAR_P_S12_OFF;

#define _LCD_BIG_F_ON()  LCD_CHAR_F_S5_ON; LCD_CHAR_F_S6_ON; LCD_CHAR_F_S8_ON;\
                         LCD_CHAR_F_S11_ON;LCD_CHAR_F_S12_ON; 
                         
#define _LCD_BIG_F_OFF() LCD_CHAR_F_S5_OFF;LCD_CHAR_F_S6_OFF;LCD_CHAR_F_S8_OFF;\
                         LCD_CHAR_F_S11_OFF;LCD_CHAR_F_S12_OFF;

#define _LCD_BIG_SYMB_OFF() _LCD_BIG_V_OFF();_LCD_BIG_I_OFF();_LCD_BIG_P_OFF();\
                            _LCD_BIG_F_OFF();
                         
#define _LCD_SML_SYMB_OFF() _LCD_Volts_OFF();_LCD_Amp_OFF();_LCD_RMS_OFF();\
                            _LCD_KW_OFF();_LCD_VA_OFF();_LCD_KWh_OFF();\
                            _LCD_KVArh_OFF();_LCD_HZ_OFF();
 
                         
                         

//------------------------------------------------------------------------------


   
    /* Turns-on a special symbol on the LCD */
    #define SymbolON(LCDn, mask)    {*((unsigned char *)&LCD_WF3TO0 + LCDn) |= (1<<(mask));} 
    
    /* Turns-off a special symbol on the LCD */
    #define SymbolOFF(LCDn, mask)   {*((unsigned char *)&LCD_WF3TO0 + LCDn) &= ~(1<<(mask));}  
   
    /* 7-LCD segments for the main group (9 digits) */
    #define  SEG1A   0x02
    #define  SEG1B   0x08
    #define  SEG1C   0x40
    #define  SEG1D   0x80
    #define  SEG1E   0x20
    #define  SEG1F   0x04
    #define  SEG1G   0x10

    /* 7-LCD segments for the auxiliary group (5 digits) */
    #define  SEG2A   0x80
    #define  SEG2B   0x40
    #define  SEG2C   0x08
    #define  SEG2D   0x02
    #define  SEG2E   0x04
    #define  SEG2F   0x20
    #define  SEG2G   0x10

    /* 7-LCD segments for the alone digit */
    #define  SEG3A   0x40
    #define  SEG3B   0x20
    #define  SEG3C   0x04
    #define  SEG3D   0x01
    #define  SEG3E   0x02
    #define  SEG3F   0x10
    #define  SEG3G   0x08

    /* Common segments configuration */
    #define  ALLON   0xFF
    #define  ALLOFF  0x00

/********************** General purpose functions *****************************/
    /* Initialization of the LCD */
    void vfnLCD_Init  (void);
    
    /* switch the LCD off */
    void vfnLCD_OFF(void);

    /* switch the LCD on */
    void vfnLCD_ON(void);
        
    /* Change of contrast of the LCD */
    void vfnLCD_Contrast (unsigned char lbContrast);
    
    /* Show one ASCII character on the LCD on demand position (0..13) */
    void vfnLCD_Write_Char (unsigned char lbValue, unsigned char pos);
   
    /* Write string to LCD - writting to upper (1) or lower (2) stage, when string
    is too long then it is scrolled (upper) or cut-off (lower) */
    void  vfnLCD_Write_Str (unsigned char *lbpMessage, unsigned char stage);
    
    /* Write number to LCD (number with sign for upper 8 digits, without sign for lower 5 digits,
    minDigits=minimum number of digits to show, writting to upper (1) or lower (2) stage */
    void vfnLCD_Write_Num(long number, unsigned char minDigits, unsigned char stage);
    
    /* Turns-on all the segments on the LCD */
    void vfnLCD_All_Segments_ON (void);
                                                                                       
    /* Turns-off all the segments on the LCD */
    void vfnLCD_All_Segments_OFF (void);
    
    /* Write message to upper or lower LCD digits  */
    void  vfnLCD_Write_Msg (unsigned char maxDigits, unsigned char *lbpMessage);
    
    /* message is rolling */
    void vfnLCD_MessageRoll (unsigned char *text);

/********************** General purpose functions *****************************/

#endif /* __LCD_H */