/**************************************************************************
* Copyright (c) 2012 Freescale Semiconductor;
* All Rights Reserved                       
*************************************************************************** 
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
***************************************************************************//*!
* @file      mk341ph.h
* @author    R55013
* @version   2.0.07.0
* @date      Jun-09-2013
* @brief     MMK34 1-phase electricity meter header file with configurations, 
*            defines and macros. 
******************************************************************************/
#ifndef __MK341PH_H
#define __MK341PH_H

/******************************************************************************
 * module defines and macros                                             
 ******************************************************************************/
/* button control                                                             */
#define BUTTON_MASK     PIN0            // PTD0
#define BUTTON2_MASK     PIN4            // PTD0
   
/* LED control                                                                */
#define LED_USER_MASK   PIN5            // PTE5
#define LED_KWH_MASK    PIN1            // PTD1 T1 output routed via XBAR_OUT3 
#define LED_KVARH_MASK  PIN1            // PTF1 T2 output routed via XBAR_OUT6
#define PULSE_OUT_MASK  PIN1            // PTF1           routed via XBAR_OUT6

/* auxilliary power                                                           */
#define POWERAUX_MASK   PIN7            // PTF7

/* power on status                                                            */
#define POWERON_MASK    PIN5            // PTC5
  
/* RESET pin                                                                  */
#define RESET_PIN_MASK  PIN1            // PTE1  

/* FreeMaster UART                                                            */
#define UART1_TX_MASK   PIN1            // PTI1
#define UART1_RX_MASK   PIN0            // PTI0

/* IR TERMINAL                                                                */
#define UART0_TX_MASK   PIN7            // PTC7
#define UART0_RX_MASK   PIN0            // PTD0  

/* MMA8491 3-axis accelerometer module                                        */
#define MMA_EN_MASK     PIN7            // PTF7
#define I2C0_SDA_MASK   PIN4            // PTG4
#define I2C0_SCL_MASK   PIN3            // PTG3

/* CAT25040 eeprom module                                                     */
#define EEPROM_SS_MASK  PIN5            // PTD5
#define SPI0_MISO_MASK  PIN4            // PTD4
#define SPI0_MOSI_MASK  PIN3            // PTD3
#define SPI0_SCK_MASK   PIN3            // PTD2

/* Pulse output control macros: The low-jitter pulse output interface,        */
/* according to patent application number: PCT/IB2013/058921, is implemented  */
/* in a simplified form. More details about full implementation can be        */
/* obtained from bmdrv@freescale.com                                          */
#define PULSE_MAX_ONTIME(sec)       (int)((float)sec*CALCFREQ)
#define TMR_RES                     (Frac16)((float)6.144e6/CALCFREQ)  
#define PULSE_OUT(chan,energy,cnt,ontime,resolution)                          \
{                                                                             \
  if (energy.pulsDly >= 0)                                                    \
  {                                                                           \
    TMR##chan##_SetCountMode(COUNT_DISABLED);                                 \
    if (energy.pulsOut==1) { TMR##chan##_SetOutMode(CLR_ON_COMP); cnt=0l; }   \
    else                   { TMR##chan##_SetOutMode(SET_ON_COMP);         }   \
    TMR##chan##_SetCntrVal(0);                                                \
    TMR##chan##_SetComp1Val(0);                                               \
    TMR##chan##_SetCountMode(COUNT_RISING_EDGE);                              \
  }                                                                           \
  else { if ((cnt++)==ontime) { TMR##chan##_ForceOFLAG (TRUE); } }            \
}
   
/* LCD macros                                                                 */
#define LCD_DISP_TARIF_SYM(tarif)                                             \
{                                                                             \
  if (tarif == 1) { _LCD_T1_ON();  } else { _LCD_T1_OFF(); }                \
  if (tarif == 2) { _LCD_T2_ON();  } else { _LCD_T2_OFF(); }                \
  if (tarif == 3) { _LCD_T3_ON();  } else { _LCD_T3_OFF(); }                \
  if (tarif == 4) { _LCD_T4_ON();  } else { _LCD_T4_OFF(); }                \
}  

#define LCD_DISP_BATT_SYM(state)                                              \
{                                                                             \
  if (state == 1) { LCD_SYM_BATT_ON; } else { LCD_SYM_BATT_OFF; }             \
}

#define LCD_DISP_WARN_SYM(state)                                              \
{                                                                             \
  if (state == 1) { LCD_SYM_WARN_ON; } else { LCD_SYM_WARN_OFF; }             \
}

/* VBAT conversion macros                                                     */
#define MAXVBAT         (double)4.13
#define VBAT2ADC(x)     (uint16)((x/MAXVBAT)*65535.0)
#define ADC2VBAT(x)     ((MAXVBAT/65535.0)*(double)x)

/* Frequency conversion macros                                                */
#define TMRPRCLK        (double)(12.288e6/8.0)
#define TMR2FREQ(x)     (double)(TMRPRCLK/(double)x)
#define FRQAVG          5.0f

/* Buid data and time formatting                                              */
#define _SEC_   ((__TIME__[6]==' '?0:__TIME__[6]-'0')*10+(__TIME__[7]-'0'))
#define _MIN_   ((__TIME__[3]==' '?0:__TIME__[3]-'0')*10+(__TIME__[4]-'0'))
#define _HOUR_  ((__TIME__[0]==' '?0:__TIME__[0]-'0')*10+(__TIME__[1]-'0'))
#define _DAY_   ((__DATE__[4]==' '?0:__DATE__[4]-'0')*10+(__DATE__[5]-'0'))
#define _MON_   (__DATE__[2] == 'n' ? (__DATE__[1] == 'a' ? 1 : 6)            \
                :__DATE__[2] == 'b' ? 2                                       \
                :__DATE__[2] == 'r' ? (__DATE__[0] == 'M' ? 3 : 4)            \
                :__DATE__[2] == 'y' ? 5                                       \
                :__DATE__[2] == 'l' ? 7                                       \
                :__DATE__[2] == 'g' ? 8                                       \
                :__DATE__[2] == 'p' ? 9                                       \
                :__DATE__[2] == 't' ? 10                                      \
                :__DATE__[2] == 'v' ? 11 : 12)
#define _YEAR_  ((((__DATE__[7]-'0')*10+(__DATE__[8] -'0'))*10+               \
                   (__DATE__[9]-'0'))*10+(__DATE__[10]-'0'))
#define M_TAB   (_MON_ == 1 ? 6: _MON_ == 2 ? 2: _MON_ == 3 ? 2:              \
                 _MON_ == 4 ? 5: _MON_ == 5 ? 0: _MON_ == 6 ? 3:              \
                 _MON_ == 7 ? 5: _MON_ == 8 ? 1: _MON_ == 9 ? 4:              \
                 _MON_ == 10? 6: _MON_ == 11? 2: 4)
#define Y2000   (_YEAR_ % 2000)
#define _WDAY_  (((!(Y2000%4)) && (_MON_<3))?(Y2000+Y2000/4+M_TAB+_DAY_-1)%7: \
                                             (Y2000+Y2000/4+M_TAB+_DAY_)%7)

#define BUILD_DATE_TIME(t)                                                    \
{                                                                             \
  t.sec = _SEC_; t.min = _MIN_; t.hour = _HOUR_; t.day = _DAY_;               \
  t.mon = _MON_; t.year= _YEAR_; t.wday = _WDAY_;                             \
}

#endif /* __MK341PH_H */