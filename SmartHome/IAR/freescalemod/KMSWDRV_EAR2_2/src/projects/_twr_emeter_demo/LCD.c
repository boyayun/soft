/******************************************************************************
* 
* Copyright (c) 2009 Freescale Semiconductor;
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
* @file      LCD.c
* @author    b19186
* @version   0.0.2.0
* @date      April-11-2011
* @brief     Driver header for LCD on TWR board, type S-Tek Displays GDH-1247WP
* 					
******************************************************************************/
#include <string.h>
#include "common.h"
#include "LCD.h"

unsigned char LCD_buffer[_LCD_CHARNUM1+1];   /* LCD buffer */
unsigned char bLCD_CharPosition;             /* current pos. of the character */

unsigned char *bpaPlace[ ] =
{ 
 (unsigned char *)&Char1,   
 (unsigned char *)&Char2,   
 (unsigned char *)&Char3,   
 (unsigned char *)&Char4,   
 (unsigned char *)&Char5,   
 (unsigned char *)&Char6,   
 (unsigned char *)&Char7,   
 (unsigned char *)&Char8,   
 (unsigned char *)&Char9,   
 (unsigned char *)&Char10,   
 (unsigned char *)&Char11,   
 (unsigned char *)&Char12,   
 (unsigned char *)&Char13,   
 (unsigned char *)&Char14,   
 (unsigned char *)&Char15,   
 };

/* Codification from Ascci code from '0' to 'Z' for the main char. group */
const unsigned char baAscii1 [ ] =
{
 ( SEG1A+ SEG1F+ SEG1B+ !SEG1G+ SEG1E+ SEG1C+ SEG1D),     //Char = 0,   offset=0
 (!SEG1A+!SEG1F+ SEG1B+ !SEG1G+!SEG1E+ SEG1C+!SEG1D),     //Char = 1,   offset=1
 ( SEG1A+!SEG1F+ SEG1B+  SEG1G+ SEG1E+!SEG1C+ SEG1D),     //Char = 2,   offset=2
 ( SEG1A+!SEG1F+ SEG1B+  SEG1G+!SEG1E+ SEG1C+ SEG1D),     //Char = 3,   offset=3
 (!SEG1A+ SEG1F+ SEG1B+  SEG1G+!SEG1E+ SEG1C+!SEG1D),     //Char = 4,   offset=4
 ( SEG1A+ SEG1F+!SEG1B+  SEG1G+!SEG1E+ SEG1C+ SEG1D),     //Char = 5,   offset=5
 ( SEG1A+ SEG1F+!SEG1B+  SEG1G+ SEG1E+ SEG1C+ SEG1D),     //Char = 6,   offset=6
 ( SEG1A+!SEG1F+ SEG1B+ !SEG1G+!SEG1E+ SEG1C+!SEG1D),     //Char = 7,   offset=7
 ( SEG1A+ SEG1F+ SEG1B+  SEG1G+ SEG1E+ SEG1C+ SEG1D),     //Char = 8,   offset=8
 ( SEG1A+ SEG1F+ SEG1B+  SEG1G+!SEG1E+ SEG1C+ SEG1D),     //Char = 9,   offset=9
 (!SEG1A+!SEG1F+!SEG1B+ !SEG1G+!SEG1E+!SEG1C+!SEG1D),     //Char =  ,   offset=10
 ( SEG1A+ SEG1F+ SEG1B+  SEG1G+ SEG1E+ SEG1C+!SEG1D),     //Char = A,   offset=11
 (!SEG1A+ SEG1F+!SEG1B+  SEG1G+ SEG1E+ SEG1C+ SEG1D),     //Char = b,   offset=12
 (!SEG1A+!SEG1F+!SEG1B+  SEG1G+ SEG1E+!SEG1C+ SEG1D),     //Char = c,   offset=13
 (!SEG1A+!SEG1F+ SEG1B+  SEG1G+ SEG1E+ SEG1C+ SEG1D),     //Char = d,   offset=14
 ( SEG1A+ SEG1F+!SEG1B+  SEG1G+ SEG1E+!SEG1C+ SEG1D),     //Char = E,   offset=15
 ( SEG1A+ SEG1F+!SEG1B+  SEG1G+ SEG1E+!SEG1C+!SEG1D),     //Char = F,   offset=16
 ( SEG1A+ SEG1F+!SEG1B+  SEG1G+ SEG1E+ SEG1C+ SEG1D),     //Char = G,   offset=17
 (!SEG1A+ SEG1F+!SEG1B+  SEG1G+ SEG1E+ SEG1C+!SEG1D),     //Char = H,   offset=18
 (!SEG1A+!SEG1F+ SEG1B+ !SEG1G+!SEG1E+ SEG1C+!SEG1D),     //Char = I,   offset=19
 (!SEG1A+!SEG1F+ SEG1B+ !SEG1G+ SEG1E+ SEG1C+ SEG1D),     //Char = J,   offset=20
 (!SEG1A+ SEG1F+ SEG1B+  SEG1G+ SEG1E+ SEG1C+!SEG1D),     //Char = K,   offset=21
 (!SEG1A+ SEG1F+!SEG1B+ !SEG1G+ SEG1E+!SEG1C+ SEG1D),     //Char = L,   offset=22
 ( SEG1A+ SEG1F+ SEG1B+ !SEG1G+ SEG1E+ SEG1C+!SEG1D),     //Char = M,   offset=23
 (!SEG1A+!SEG1F+!SEG1B+  SEG1G+ SEG1E+ SEG1C+!SEG1D),     //Char = n,   offset=24
 (!SEG1A+!SEG1F+!SEG1B+  SEG1G+ SEG1E+ SEG1C+ SEG1D),     //Char = o,   offset=25
 ( SEG1A+ SEG1F+ SEG1B+  SEG1G+ SEG1E+!SEG1C+!SEG1D),     //Char = P,   offset=26
 (!SEG1A+!SEG1F+!SEG1B+  SEG1G+ SEG1E+ SEG1C+ SEG1D),     //Char = Q,   offset=27
 (!SEG1A+!SEG1F+!SEG1B+  SEG1G+ SEG1E+!SEG1C+!SEG1D),     //Char = r,   offset=28
 ( SEG1A+ SEG1F+!SEG1B+  SEG1G+!SEG1E+ SEG1C+ SEG1D),     //Char = s,   offset=29
 (!SEG1A+ SEG1F+!SEG1B+  SEG1G+ SEG1E+!SEG1C+ SEG1D),     //Char = t,   offset=30
 (!SEG1A+ SEG1F+ SEG1B+ !SEG1G+ SEG1E+ SEG1C+ SEG1D),     //Char = U,   offset=31
 (!SEG1A+!SEG1F+!SEG1B+ !SEG1G+ SEG1E+ SEG1C+ SEG1D),     //Char = v,   offset=32
 (!SEG1A+!SEG1F+!SEG1B+ !SEG1G+ SEG1E+ SEG1C+ SEG1D),     //Char = w,   offset=33
 (!SEG1A+ SEG1F+ SEG1B+  SEG1G+ SEG1E+ SEG1C+!SEG1D),     //Char = X,   offset=34
 (!SEG1A+ SEG1F+ SEG1B+  SEG1G+ SEG1E+!SEG1C+!SEG1D),     //Char = Y,   offset=35
 ( SEG1A+!SEG1F+ SEG1B+  SEG1G+ SEG1E+!SEG1C+ SEG1D),     //Char = Z,   offset=36
};

/* Codification from Ascci code from '0' to 'Z' for the OBIS code group */
const unsigned char baAscii2 [ ] =
{
 ( SEG2A+ SEG2F+ SEG2B+ !SEG2G+ SEG2E+ SEG2C+ SEG2D),     //Char = 0,   offset=0
 (!SEG2A+!SEG2F+ SEG2B+ !SEG2G+!SEG2E+ SEG2C+!SEG2D),     //Char = 1,   offset=1
 ( SEG2A+!SEG2F+ SEG2B+  SEG2G+ SEG2E+!SEG2C+ SEG2D),     //Char = 2,   offset=2
 ( SEG2A+!SEG2F+ SEG2B+  SEG2G+!SEG2E+ SEG2C+ SEG2D),     //Char = 3,   offset=3
 (!SEG2A+ SEG2F+ SEG2B+  SEG2G+!SEG2E+ SEG2C+!SEG2D),     //Char = 4,   offset=4
 ( SEG2A+ SEG2F+!SEG2B+  SEG2G+!SEG2E+ SEG2C+ SEG2D),     //Char = 5,   offset=5
 ( SEG2A+ SEG2F+!SEG2B+  SEG2G+ SEG2E+ SEG2C+ SEG2D),     //Char = 6,   offset=6
 ( SEG2A+!SEG2F+ SEG2B+ !SEG2G+!SEG2E+ SEG2C+!SEG2D),     //Char = 7,   offset=7
 ( SEG2A+ SEG2F+ SEG2B+  SEG2G+ SEG2E+ SEG2C+ SEG2D),     //Char = 8,   offset=8
 ( SEG2A+ SEG2F+ SEG2B+  SEG2G+!SEG2E+ SEG2C+ SEG2D),     //Char = 9,   offset=9
 (!SEG2A+!SEG2F+!SEG2B+ !SEG2G+!SEG2E+!SEG2C+!SEG2D),     //Char =  ,   offset=10
 ( SEG2A+ SEG2F+ SEG2B+  SEG2G+ SEG2E+ SEG2C+!SEG2D),     //Char = A,   offset=11
 (!SEG2A+ SEG2F+!SEG2B+  SEG2G+ SEG2E+ SEG2C+ SEG2D),     //Char = b,   offset=12
 (!SEG2A+!SEG2F+!SEG2B+  SEG2G+ SEG2E+!SEG2C+ SEG2D),     //Char = c,   offset=13
 (!SEG2A+!SEG2F+ SEG2B+  SEG2G+ SEG2E+ SEG2C+ SEG2D),     //Char = d,   offset=14
 ( SEG2A+ SEG2F+!SEG2B+  SEG2G+ SEG2E+!SEG2C+ SEG2D),     //Char = E,   offset=15
 ( SEG2A+ SEG2F+!SEG2B+  SEG2G+ SEG2E+!SEG2C+!SEG2D),     //Char = F,   offset=16
 ( SEG2A+ SEG2F+!SEG2B+  SEG2G+ SEG2E+ SEG2C+ SEG2D),     //Char = G,   offset=17
 (!SEG2A+ SEG2F+!SEG2B+  SEG2G+ SEG2E+ SEG2C+!SEG2D),     //Char = H,   offset=18
 (!SEG2A+!SEG2F+ SEG2B+ !SEG2G+!SEG2E+ SEG2C+!SEG2D),     //Char = I,   offset=19
 (!SEG2A+!SEG2F+ SEG2B+ !SEG2G+ SEG2E+ SEG2C+ SEG2D),     //Char = J,   offset=20
 (!SEG2A+ SEG2F+ SEG2B+  SEG2G+ SEG2E+ SEG2C+!SEG2D),     //Char = K,   offset=21
 (!SEG2A+ SEG2F+!SEG2B+ !SEG2G+ SEG2E+!SEG2C+ SEG2D),     //Char = L,   offset=22
 ( SEG2A+ SEG2F+ SEG2B+ !SEG2G+ SEG2E+ SEG2C+!SEG2D),     //Char = M,   offset=23
 (!SEG2A+!SEG2F+!SEG2B+  SEG2G+ SEG2E+ SEG2C+!SEG2D),     //Char = n,   offset=24
 (!SEG2A+!SEG2F+!SEG2B+  SEG2G+ SEG2E+ SEG2C+ SEG2D),     //Char = o,   offset=25
 ( SEG2A+ SEG2F+ SEG2B+  SEG2G+ SEG2E+!SEG2C+!SEG2D),     //Char = P,   offset=26
 (!SEG2A+!SEG2F+!SEG2B+  SEG2G+ SEG2E+ SEG2C+ SEG2D),     //Char = Q,   offset=27
 (!SEG2A+!SEG2F+!SEG2B+  SEG2G+ SEG2E+!SEG2C+!SEG2D),     //Char = r,   offset=28
 ( SEG2A+ SEG2F+!SEG2B+  SEG2G+!SEG2E+ SEG2C+ SEG2D),     //Char = s,   offset=29
 (!SEG2A+ SEG2F+!SEG2B+  SEG2G+ SEG2E+!SEG2C+ SEG2D),     //Char = t,   offset=30
 (!SEG2A+ SEG2F+ SEG2B+ !SEG2G+ SEG2E+ SEG2C+ SEG2D),     //Char = U,   offset=31
 (!SEG2A+!SEG2F+!SEG2B+ !SEG2G+ SEG2E+ SEG2C+ SEG2D),     //Char = v,   offset=32
 (!SEG2A+!SEG2F+!SEG2B+ !SEG2G+ SEG2E+ SEG2C+ SEG2D),     //Char = w,   offset=33
 (!SEG2A+ SEG2F+ SEG2B+  SEG2G+ SEG2E+ SEG2C+!SEG2D),     //Char = X,   offset=34
 (!SEG2A+ SEG2F+ SEG2B+  SEG2G+ SEG2E+!SEG2C+!SEG2D),     //Char = Y,   offset=35
 ( SEG2A+!SEG2F+ SEG2B+  SEG2G+ SEG2E+!SEG2C+ SEG2D),     //Char = Z,   offset=36
};

/* Codification from Ascci code from '0' to '9' for the alone segment */
const unsigned char baAscii3 [ ] =
{
 ( SEG3A+ SEG3F+ SEG3B+ !SEG3G+ SEG3E+ SEG3C+ SEG3D),     //Char = 0,   offset=0
 (!SEG3A+!SEG3F+ SEG3B+ !SEG3G+!SEG3E+ SEG3C+!SEG3D),     //Char = 1,   offset=1
 ( SEG3A+!SEG3F+ SEG3B+  SEG3G+ SEG3E+!SEG3C+ SEG3D),     //Char = 2,   offset=2
 ( SEG3A+!SEG3F+ SEG3B+  SEG3G+!SEG3E+ SEG3C+ SEG3D),     //Char = 3,   offset=3
 (!SEG3A+ SEG3F+ SEG3B+  SEG3G+!SEG3E+ SEG3C+!SEG3D),     //Char = 4,   offset=4
 ( SEG3A+ SEG3F+!SEG3B+  SEG3G+!SEG3E+ SEG3C+ SEG3D),     //Char = 5,   offset=5
 ( SEG3A+ SEG3F+!SEG3B+  SEG3G+ SEG3E+ SEG3C+ SEG3D),     //Char = 6,   offset=6
 ( SEG3A+!SEG3F+ SEG3B+ !SEG3G+!SEG3E+ SEG3C+!SEG3D),     //Char = 7,   offset=7
 ( SEG3A+ SEG3F+ SEG3B+  SEG3G+ SEG3E+ SEG3C+ SEG3D),     //Char = 8,   offset=8
 ( SEG3A+ SEG3F+ SEG3B+  SEG3G+!SEG3E+ SEG3C+ SEG3D),     //Char = 9,   offset=9
 (!SEG3A+!SEG3F+!SEG3B+ !SEG3G+!SEG3E+!SEG3C+!SEG3D),     //Char =  ,   offset=10
};

/***************************************************************************//*!
*
* @brief    Initialization of LCD   
*
* @param    nothing
* @return   nothing
*
******************************************************************************/
void vfnLCD_Init(void)
{
  MCG_C1|= 0x02;                  // enable IRCLK
   
  LCD_PENL= 0x9FFFfe00;           // enable LCD pins (28 pins)
  LCD_PENH= 0x0000083F;
  
  LCD_BPENL = 0x0001FE00;         // enable LCD backplanes 9...16

  LCD_WF19TO16 = 0x00000001;      // assign backplanes 
  LCD_WF15TO12 = 0x02040810;      // assign backplanes 
  LCD_WF11TO8 = 0x20408000;      // assign backplanes 

  LCD_GCR&=~LCD_GCR_LCDDOZE_MASK; 
  LCD_GCR|=LCD_GCR_CPSEL_MASK;
  LCD_GCR|=LCD_GCR_LADJ(0);       // !!!
  LCD_GCR|=LCD_GCR_DUTY(7);
  LCD_GCR|=LCD_GCR_LCLK(1);       
  LCD_GCR|=LCD_GCR_LCDEN_MASK;   //Enable LCD
}
  
/***************************************************************************//*!
*
* @brief    Switching the LCD off
*
* @param    nothing
* @return   nothing
*
******************************************************************************/
void vfnLCD_OFF(void)
{
  LCD_GCR|= LCD_GCR_LCDDOZE_MASK;   
  LCD_GCR&= ~LCD_GCR_LCDEN_MASK;   //Disable LCD
}

/***************************************************************************//*!
*
* @brief    Switching the LCD on
*
* @param    nothing
* @return   nothing
*
******************************************************************************/
void vfnLCD_ON(void)
{
  LCD_GCR&= ~LCD_GCR_LCDDOZE_MASK;   
  LCD_GCR|= LCD_GCR_LCDEN_MASK;   //Enable LCD
}

/***************************************************************************//*!
*
* @brief    Change of contrast of LCD   
*
* @param    value (0..15)
* @return   nothing
*
******************************************************************************/
void vfnLCD_Contrast (unsigned char lbContrast)
{
  lbContrast &= 0x0F;                                                                                                           
 // LCD_GCR|= LCD_GCR_RVEN_MASK; 
  
  LCD_GCR &= ~LCD_GCR_RVTRIM_MASK;
  LCD_GCR |= LCD_GCR_RVTRIM(lbContrast);                 
}
 
/******************************************************************************
                        Symbol mask for LCD segments 1..14
******************************************************************************/
const unsigned char symbol_mask[] = {
    0x01,        // Char 1
    0x01,        // Char 2
    0x01,        // Char 3
    0x01,        // Char 4
    0x01,        // Char 5
    0x01,        // Char 6
    0x01,        // Char 7
    0x01,        // Char 8
    0x01,        // Char 9
    0x01,        // Char 10
    0x01,        // Char 11
    0x01,        // Char 12
    0x01,        // Char 13
    0x01,        // Char 14
    0x80,        // Char 15
};

/***************************************************************************//*!
*
* @brief    Write one character to LCD   
*
* @param    character (ASCII format), support only '0'..'9',':','a'..'z'
*           position of LCD digit (0..13)
* @return   nothing
*
******************************************************************************/
void vfnLCD_Write_Char (unsigned char lbValue, unsigned char pos)
{
    unsigned char *lbpLCDWF;
    unsigned char lbCounter;
    unsigned char lbShadow;
    unsigned char lbIndex;

    /* Only ASCII character, numbers and space */
    if (lbValue >='a' && lbValue <='z') lbValue -= 32;  // UpperCase
    if (lbValue >='A' && lbValue <='Z') lbValue -= 6;   // Only characters and numbers
    if (lbValue < '0' || lbValue > 'Z') lbValue = ':';  // default value as space
    
    lbValue -= '0';       // Remove the offset to search in the ASCII table
    lbValue *= _LCDTYPE;  // Compensate matrix offset

    lbCounter = _LCDTYPE;  //number of writings to complete one char

    while (lbCounter-- /* && bLCD_CharPosition < (_LCD_CHARNUM1+1) */ )
    {
        lbIndex = (unsigned char)(lbCounter + (pos&0x0f) * _LCDTYPE);
        lbpLCDWF = bpaPlace[lbIndex];
        lbShadow  = *lbpLCDWF;
        lbShadow &= symbol_mask[lbIndex];
        if (pos<=8) lbShadow |= baAscii1[lbValue + lbCounter];
        else if ((pos>=9) && (pos<=13)) lbShadow |= baAscii2[lbValue + lbCounter];
        else lbShadow |= baAscii3[lbValue + lbCounter];
        *lbpLCDWF = lbShadow;
    }
}

/***************************************************************************//*!
*
* @brief    Write message to upper or lower LCD digits  
*
* @param    message (ASCII format)
* @return   nothing
*
******************************************************************************/
void  vfnLCD_Write_Msg (unsigned char maxDigits, unsigned char *lbpMessage)
{
    unsigned char lbSize = 0;          
    while (lbSize < maxDigits  && *lbpMessage) 
    {
       vfnLCD_Write_Char (*lbpMessage,bLCD_CharPosition++);
       lbSize++;    
       lbpMessage++;
    }
}
 
/***************************************************************************//*!
*
* @brief    Text message roll  
*
* @param    text for scrolling on upper LCD digits
* @return   nothing
*
******************************************************************************/
  void vfnLCD_MessageRoll (unsigned char *text) {
    unsigned char i,j;
    unsigned char *pTextAll,*pTextScr;

    pTextAll = text;                   // initial text message on LCD
    for (i=0;i<strlen((const char*)text);i++) {
      pTextScr = pTextAll;
      for (j=0;j<_LCD_CHARNUM1;j++) {
        LCD_buffer[j] = *pTextScr++;
      }
      LCD_buffer[_LCD_CHARNUM1] = '\0';
      vfnLCD_All_Segments_OFF();
      bLCD_CharPosition = 0;  //Home display
      vfnLCD_Write_Msg(_LCD_CHARNUM1,LCD_buffer);
      arch_delay(8000000l);          // wait ms         
      pTextAll++;
    }
  }

/***************************************************************************//*!
*
* @brief    Write string to LCD 
*
* @param    string (if > 8 digits then string is scrolled) - upper stage
*           string (if > 5 digits then string is cut-off) - lower stage
*           stage=1 for upper stage, stage=2 for lower stage
* @return   nothing
* @note     when number and text is written on one LCD stage, it is recommended
*           to call vfnLCD_Write_Num firstly and then vfnLCD_Write_Str, because
*           vfnLCD_Write_Num fills up all LCD digits on the stage
*           String is lined up from left to right
******************************************************************************/
void  vfnLCD_Write_Str (unsigned char *lbpMessage, unsigned char stage)
{
    unsigned char lbSize = 0;
    unsigned char *pMsg;

    if ((stage == 1) || (stage == 2)) {       // only stage 1 or 2
        pMsg = lbpMessage;
        while(*pMsg++) lbSize++;
        
        if (stage == 1) {
          bLCD_CharPosition=0;                //upper part of display
         }
        else
        {
          if (lbSize > (_MAX_NUM_DIGITS2) ) 
            lbSize = _MAX_NUM_DIGITS2;        // cut-off remaining digits on lower stage
          bLCD_CharPosition=9;                //lower part of display
        }
        
        if (lbSize > (_MAX_NUM_DIGITS1) )
        {
          // string on upper LCD stage is too long to show - must be scrolled
          vfnLCD_MessageRoll(lbpMessage);   
        }
        else
        {
           do
            {
                vfnLCD_Write_Char (*lbpMessage,bLCD_CharPosition++);
                lbpMessage ++;
                
            } while (--lbSize);
        }
    }
}
 
/***************************************************************************//*!
*
* @brief    Turn-on all LCD segments 
*
* @param    nothing
* @return   nothing
*
******************************************************************************/
void vfnLCD_All_Segments_ON(void)
{              
  CharacterPlace(43) = ALLON;
  CharacterPlace(37) = ALLON;
  CharacterPlace(36) = ALLON;
  CharacterPlace(35) = ALLON;
  CharacterPlace(34) = ALLON;
  CharacterPlace(33) = ALLON;
  CharacterPlace(32) = ALLON;
  CharacterPlace(31) = ALLON;
  CharacterPlace(28) = ALLON;
  CharacterPlace(27) = ALLON;
  CharacterPlace(26) = ALLON;
  CharacterPlace(25) = ALLON;
  CharacterPlace(24) = ALLON;
  CharacterPlace(23) = ALLON;
  CharacterPlace(22) = ALLON;
  CharacterPlace(21) = ALLON;
  CharacterPlace(20) = ALLON;
  CharacterPlace(19) = ALLON;
  CharacterPlace(18) = ALLON;
  CharacterPlace(17) = ALLON;
}

/***************************************************************************//*!
*
* @brief    Turn-off all LCD segments 
*
* @param    nothing
* @return   nothing
*
******************************************************************************/
void vfnLCD_All_Segments_OFF (void)
{              
  CharacterPlace(43) = ALLOFF;
  CharacterPlace(37) = ALLOFF;
  CharacterPlace(36) = ALLOFF;
  CharacterPlace(35) = ALLOFF;
  CharacterPlace(34) = ALLOFF;
  CharacterPlace(33) = ALLOFF;
  CharacterPlace(32) = ALLOFF;
  CharacterPlace(31) = ALLOFF;
  CharacterPlace(28) = ALLOFF;
  CharacterPlace(27) = ALLOFF;
  CharacterPlace(26) = ALLOFF;
  CharacterPlace(25) = ALLOFF;
  CharacterPlace(24) = ALLOFF;
  CharacterPlace(23) = ALLOFF;
  CharacterPlace(22) = ALLOFF;
  CharacterPlace(21) = ALLOFF;
  CharacterPlace(20) = ALLOFF;
  CharacterPlace(19) = ALLOFF;
  CharacterPlace(18) = ALLOFF;
  CharacterPlace(17) = ALLOFF;
}

/***************************************************************************//*!
*
* @brief    Fill LCD buffer with number and send it to LCD 
*
* @param    number - max. 7 digits with sign or 8 digits without sign
*           mindigits - minimum number of digits to show on LCD (number format)
*           stage - 1=upper LCD stage (8 digits max.), 2=lower LCD stage (5 digits max.)
* @return   nothing
* @note     when number and text is written on one LCD stage, it is recommended
*           to call vfnLCD_Write_Num firstly and then vfnLCD_Write_Str, because
*           vfnLCD_Write_Num fills up all LCD digits on the stage
*           Number is lined up from right to left
******************************************************************************/
void vfnLCD_Write_Num(long number, unsigned char minDigits, unsigned char stage) {
  unsigned char dig,i;
  long reghelp;
  unsigned char position=0;
  long decimal;
  unsigned char numPos=0;
  unsigned char tempLCDb[_MAX_NUM_DIGITS1];
  unsigned char *s=LCD_buffer;
  unsigned char maxDigits;
    
  if ((stage == 1) || (stage == 2)) {       // only stage 1 or 2
      if (number < 0)
        reghelp=-number;
      else 
        reghelp=number;
      
      if (stage == 1) {
        maxDigits = _MAX_NUM_DIGITS1;       // number of digits overflow
        bLCD_CharPosition=0;  //upper part of display
        decimal=100000000;
       }
      else
      {
        maxDigits = _MAX_NUM_DIGITS2;       // number of digits overflow
        bLCD_CharPosition=9;  //lower part of display
        decimal=10000;
      }
      if (minDigits>maxDigits) minDigits = maxDigits;        // number of digits overflow
     
      for (i=0;i<maxDigits;i++){
        dig=(unsigned char)(reghelp / decimal);                    
        if ((dig==0) && (!numPos)) {
          tempLCDb[position++] = (unsigned char) ('0');        // fill zero to upper left digits
        }
        else {
          tempLCDb[position++] = (unsigned char) (dig + '0');  // fill numbers
          numPos++;
        }
        reghelp= (long)(reghelp-dig*decimal);             
        decimal /= 10;
      }
      if (numPos<minDigits) numPos = minDigits;             // number of digits is less than minimum represented digits
      
      position = 0;
    
      for (i=0;i<(maxDigits-numPos);i++){             // normal state of digits
        s[position++] = ' ';
      }
      i = (unsigned char)(maxDigits-numPos);
    
      for (;i<maxDigits;i++){                    // fill number
        s[position++] = tempLCDb[i];
      }
      
      s[position] = '\0';                             // end of string
    
      vfnLCD_Write_Msg(maxDigits,LCD_buffer);               // send data to LCD !
      if ((stage == 1) && (LCD_buffer[0] == ' ')) { 
        if (number<0) _LCD_MINUS_ON()
        else _LCD_MINUS_OFF();
      }
  }
}
