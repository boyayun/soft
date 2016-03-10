/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      irtc.c
 * @version   1.0.5.0
 * @date      Jun-08-2013
 * @brief     IRTC Real Time Clock (IRTC) driver source code.
 ******************************************************************************/
#include "common.h"
#include "irtc.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
#if defined (__ICCARM__)    /* IAR                                            */
  volatile uint8_t IRTC_BbRam[32] @0x40051000u;                          
#elif defined(__GNUC__)     /* CW GCC                                         */
  volatile uint8_t __attribute__((section (".RtcRamSection"))) IRTC_BbRam[32];  
#endif
  
static IRTC_CALLBACK pCallbackIRTC=NULL;

/******************************************************************************
 * interrupt functions prototype                                              *
 ******************************************************************************/
void IRTC_isr (void);

/******************************************************************************
 * Public functions definitions                                               *  
 ******************************************************************************/
void IRTC_InstallCallback (uint8 ip, IRTC_CALLBACK pCallback)
{
  if (pCallback != NULL) { pCallbackIRTC = pCallback; NVIC_SetIsr(INT_RTC,ip); }
}

void IRTC_Init (tIRTC irtc, tTAMPER t0, tTAMPER t1, tTAMPER t2, uint32 mask)
{
  register uint16 tmp;
  
  IRTC_LockUnlock();
    
  /* setting common registers                                                 */
  /* disable and don't support at all TAMPER3 (battery removal) interrupt     */ 
  RTC_TAMPER_SCR    &=~0x0008;    
  RTC_TAMPER_SCR    |= 0x0f08;    /* clear all tamper pin flags               */
  RTC_ISR            = 0xffff;    /* clear all IRTC module interrupt flags    */
  IRTC_DisableIsr(IRTC_ALL_MASK); /* disable all supported interrupts         */
  IRTC_EnableIsr (mask);          /* enable only selected interrupts          */
  RTC_CTRL           =  irtc.CTRL;
  RTC_COMPEN         =  irtc.COMPEN;
  RTC_TAMPER_QSCR    =  irtc.QSCR; 
   
  /* tamper 0 pin setting - used as the tamper pin                            */
  RTC_CTRL2 &= ~RTC_CTRL2_WAKEUP_MODE_MASK;
  tmp  = RTC_TAMPER_DIRECTION_A_P_TAMP(TAMPER0_PIN)|
         RTC_TAMPER_DIRECTION_I_O_TAMP(TAMPER0_PIN); 
  RTC_TAMPER_DIRECTION  = (t0.DIRECTION & tmp);
  tmp  = RTC_FILTER01_CFG_POL0_MASK|RTC_FILTER01_CFG_CLK_SEL0_MASK|
         RTC_FILTER01_CFG_FIL_DUR0_MASK;
  RTC_FILTER01_CFG      = ((uint16)(t0.FILTER_CFG<<8) & tmp);
    
  /* tamper 1 pin setting                                                     */
  tmp  = RTC_TAMPER_DIRECTION_A_P_TAMP(TAMPER1_PIN)|
         RTC_TAMPER_DIRECTION_I_O_TAMP(TAMPER1_PIN); 
  RTC_TAMPER_DIRECTION |= (t1.DIRECTION & tmp);
  tmp  = RTC_FILTER01_CFG_POL1_MASK|RTC_FILTER01_CFG_CLK_SEL1_MASK|
         RTC_FILTER01_CFG_FIL_DUR1_MASK;
  RTC_FILTER01_CFG     |= (t1.FILTER_CFG & tmp);

  /* tamper 2 pin setting                                                     */
  tmp  = RTC_TAMPER_DIRECTION_A_P_TAMP(TAMPER2_PIN)|
         RTC_TAMPER_DIRECTION_I_O_TAMP(TAMPER2_PIN); 
  RTC_TAMPER_DIRECTION |= (t2.DIRECTION & tmp);
  tmp  = RTC_FILTER2_CFG_POL2_MASK|RTC_FILTER2_CFG_CLK_SEL2_MASK|
         RTC_FILTER2_CFG_FIL_DUR2_MASK;
  RTC_FILTER2_CFG       = ((uint16)(t2.FILTER_CFG<<8) & tmp);
  
  /* enable tamper filter processing                                          */ 
  RTC_CTRL2 |= RTC_CTRL2_TAMP_CFG_OVER_MASK; 
}

void IRTC_WkupInit (tIRTC irtc, tTAMPER t1, tTAMPER t2, uint32 mask)
{
  register uint16 tmp;
  
  IRTC_LockUnlock();
    
  /* setting common registers                                                 */
  /* disable and don't support at all TAMPER3 (battery removal) interrupt     */ 
  RTC_TAMPER_SCR    &=~0x0008;    
  RTC_TAMPER_SCR    |= 0x0f08;    /* clear all tamper pin flags               */
  RTC_ISR            = 0xffff;    /* clear all IRTC module interrupt flags    */
  IRTC_DisableIsr(IRTC_ALL_MASK); /* disable all supported interrupts         */
  IRTC_EnableIsr (mask);          /* enable only selected interrupts          */
  RTC_CTRL           =  irtc.CTRL;
  RTC_COMPEN         =  irtc.COMPEN;
  RTC_TAMPER_QSCR    =  irtc.QSCR; 
   
  /* tamper 0 pin setting - used as the wakeup/hibernate pin                  */
  RTC_CTRL2 |= RTC_CTRL2_WAKEUP_MODE_MASK;

  /* tamper 1 pin setting                                                     */
  tmp  = RTC_TAMPER_DIRECTION_A_P_TAMP(TAMPER1_PIN)|
         RTC_TAMPER_DIRECTION_I_O_TAMP(TAMPER1_PIN); 
  RTC_TAMPER_DIRECTION |= (t1.DIRECTION & tmp);
  tmp  = RTC_FILTER01_CFG_POL1_MASK|RTC_FILTER01_CFG_CLK_SEL1_MASK|
         RTC_FILTER01_CFG_FIL_DUR1_MASK;
  RTC_FILTER01_CFG     |= (t1.FILTER_CFG & tmp);

  /* tamper 2 pin setting                                                     */
  tmp  = RTC_TAMPER_DIRECTION_A_P_TAMP(TAMPER2_PIN)|
         RTC_TAMPER_DIRECTION_I_O_TAMP(TAMPER2_PIN); 
  RTC_TAMPER_DIRECTION |= (t2.DIRECTION & tmp);
  tmp  = RTC_FILTER2_CFG_POL2_MASK|RTC_FILTER2_CFG_CLK_SEL2_MASK|
         RTC_FILTER2_CFG_FIL_DUR2_MASK;
  RTC_FILTER2_CFG       = ((uint16)(t2.FILTER_CFG<<8) & tmp);
  
  /* enable tamper filter processing                                          */ 
  RTC_CTRL2 |= RTC_CTRL2_TAMP_CFG_OVER_MASK; 
}

void IRTC_GetClock (tTIME *ptr)
{                                      
  register uint16 reg_yearmon, reg_days, reg_hourmin, reg_seconds;
  
  /* read RTC registers and store their values in variables - we are loading  */
  /* variables by content of registers to minimize number of read operations  */
  while (1)
  {  
    if (RTC_STATUS & RTC_STATUS_INVAL_BIT_MASK) { continue; } 
    reg_yearmon = RTC_YEARMON;
    if (RTC_STATUS & RTC_STATUS_INVAL_BIT_MASK) { continue; }
    reg_days    = RTC_DAYS;
    if (RTC_STATUS & RTC_STATUS_INVAL_BIT_MASK) { continue; }
    reg_hourmin = RTC_HOURMIN;
    if (RTC_STATUS & RTC_STATUS_INVAL_BIT_MASK) { continue; }
    reg_seconds = RTC_SECONDS;
    if (RTC_STATUS & RTC_STATUS_INVAL_BIT_MASK) { continue; }
    break;
  } 
  /* clock formating                                                          */
  ptr->year = (((int16)reg_yearmon>>RTC_YEARMON_YROFST_SHIFT) + BASE_YEAR);
  ptr->mon  = ((uint16)reg_yearmon>>RTC_YEARMON_MON_CNT_SHIFT)  & 0x0f;
  ptr->day  = ((uint16)reg_days   >>RTC_DAYS_DAY_CNT_SHIFT)     & 0x1f; 
  ptr->wday = ((uint16)reg_days   >>RTC_DAYS_DOW_SHIFT)         & 0x07;
  ptr->hour = ((uint16)reg_hourmin>>RTC_HOURMIN_HOUR_CNT_SHIFT) & 0x1f;
  ptr->min  = ((uint16)reg_hourmin>>RTC_HOURMIN_MIN_CNT_SHIFT)  & 0x3f;
  ptr->sec  = ((uint16)reg_seconds>>RTC_SECONDS_SEC_CNT_SHIFT)  & 0x3f;
}

void IRTC_SetClock (tTIME *ptr)
{
  register int16 tmp = ptr->year - BASE_YEAR;
  
  IRTC_LockUnlock();
  while (1)
  {  
    if (RTC_STATUS & RTC_STATUS_INVAL_BIT_MASK) { continue; }
    RTC_YEARMON = RTC_YEARMON_YROFST(tmp)|RTC_YEARMON_MON_CNT(ptr->mon);
    if (RTC_STATUS & RTC_STATUS_INVAL_BIT_MASK) { continue; }
    RTC_DAYS    = RTC_DAYS_DAY_CNT(ptr->day)|RTC_DAYS_DOW(ptr->wday);
    if (RTC_STATUS & RTC_STATUS_INVAL_BIT_MASK) { continue; }
    RTC_HOURMIN = RTC_HOURMIN_HOUR_CNT(ptr->hour)|RTC_HOURMIN_MIN_CNT(ptr->min); 
    if (RTC_STATUS & RTC_STATUS_INVAL_BIT_MASK) { continue; }
    RTC_SECONDS = RTC_SECONDS_SEC_CNT(ptr->sec);
    if (RTC_STATUS & RTC_STATUS_INVAL_BIT_MASK) { continue; }
    break;
  } 
}

void IRTC_WrBbRam (void *src, uint8 size)
{                                                              
  register uint8 *ps = src, *pd=(uint8*)IRTC_BbRam;
  if (size > sizeof(IRTC_BbRam)) size = sizeof(IRTC_BbRam); 
  IRTC_LockUnlock();                                           
  while (size--) *pd++ = *ps++;
}

void IRTC_RdBbRam (void *dst, uint8 size)
{                                                              
  register uint8 *pd = dst, *ps=(uint8*)IRTC_BbRam;
  if (size > sizeof(IRTC_BbRam)) size = sizeof(IRTC_BbRam); 
  while (size--) *pd++ = *ps++;
}

/******************************************************************************
 * interrupt functions definitions                                            *
 ******************************************************************************/ 
#pragma diag_suppress=Pe550
void IRTC_isr(void)
{
  static tTAMPER_LOG log;
  static tTIME       tmp;
  register uint16    reg_ier, reg_tamper_scr, reg_isr;
  
  /* read RTC registers and store their values in variables - we're loading   */
  /* variables by content of registers to minimize number of read operations  */
  reg_ier = RTC_IER; reg_tamper_scr = RTC_TAMPER_SCR; reg_isr = RTC_ISR;

  IRTC_LockUnlock();
  /* TAMPER pin callback processing                                           */
  if(reg_ier & RTC_IER_TAMPER_IE_MASK)  
  {    
    /* TAMPER0 pin                                                            */
    if((reg_tamper_scr & RTC_TAMPER_SCR_TMPR_EN (TAMPER0_PIN)) &&
       (reg_tamper_scr & RTC_TAMPER_SCR_TMPR_STS(TAMPER0_PIN)))
    {
      IRTC_GetTamperLog (&log);
      ioseth (RTC_TAMPER_SCR,RTC_TAMPER_SCR_TMPR_STS(TAMPER0_PIN));
      if (pCallbackIRTC != NULL) { pCallbackIRTC(IRTC_TAMPER0_CALLBACK, &log); }
    }
    /* TAMPER1 pin                                                            */
    if((reg_tamper_scr & RTC_TAMPER_SCR_TMPR_EN (TAMPER1_PIN)) &&
       (reg_tamper_scr & RTC_TAMPER_SCR_TMPR_STS(TAMPER1_PIN)))
    {
      IRTC_GetTamperLog (&log);
      ioseth (RTC_TAMPER_SCR,RTC_TAMPER_SCR_TMPR_STS(TAMPER1_PIN));
      if (pCallbackIRTC != NULL) { pCallbackIRTC(IRTC_TAMPER1_CALLBACK, &log); }
    }
    /* TAMPER2 pin                                                            */
    if((reg_tamper_scr & RTC_TAMPER_SCR_TMPR_EN (TAMPER2_PIN)) &&
       (reg_tamper_scr & RTC_TAMPER_SCR_TMPR_STS(TAMPER2_PIN)))
    {
      IRTC_GetTamperLog (&log);
      ioseth (RTC_TAMPER_SCR,RTC_TAMPER_SCR_TMPR_STS(TAMPER2_PIN));
      if (pCallbackIRTC != NULL) { pCallbackIRTC(IRTC_TAMPER2_CALLBACK, &log); }
    }
  }
  IRTC_GetClock (&tmp);
  /* ALARM callback processing                                                */
  if((reg_ier & RTC_IER_ALM_IE_MASK) && (reg_isr & RTC_ISR_ALM_IS_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_ALM_IS_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_ALM_CALLBACK, &tmp);   }
  }    
  /* DAY callback processing                                                  */
  if((reg_ier & RTC_IER_DAY_IE_MASK) && (reg_isr & RTC_ISR_DAY_IS_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_DAY_IS_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_DAY_CALLBACK, &tmp);   }
  }    
  /* HOUR callback processing                                                 */
  if((reg_ier & RTC_IER_HOUR_IE_MASK) && (reg_isr & RTC_ISR_HOUR_IS_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_HOUR_IS_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_HOUR_CALLBACK, &tmp);  }
  }
  /* MIN callback processing                                                  */
  if((reg_ier & RTC_IER_MIN_IE_MASK) && (reg_isr & RTC_ISR_MIN_IS_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_MIN_IS_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_MIN_CALLBACK, &tmp);   }
  }    
  /* 1HZ callback processing                                                  */
  if((reg_ier & RTC_IER_IE_1HZ_MASK) && (reg_isr & RTC_ISR_IS_1HZ_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_IS_1HZ_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_1HZ_CALLBACK, &tmp);   }
  }    
  /* 2HZ callback processing                                                  */
  if((reg_ier & RTC_IER_IE_2HZ_MASK) && (reg_isr & RTC_ISR_IS_2HZ_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_IS_2HZ_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_2HZ_CALLBACK, &tmp);   }
  }    
  /* 4HZ callback processing                                                  */
  if((reg_ier & RTC_IER_IE_4HZ_MASK) && (reg_isr & RTC_ISR_IS_4HZ_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_IS_4HZ_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_4HZ_CALLBACK, &tmp);   }
  }    
  /* 8HZ callback processing                                                  */
  if((reg_ier & RTC_IER_IE_8HZ_MASK) && (reg_isr & RTC_ISR_IS_8HZ_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_IS_8HZ_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_8HZ_CALLBACK, &tmp);   }
  }
  /* 16HZ callback processing                                                 */
  if((reg_ier & RTC_IER_IE_16HZ_MASK) && (reg_isr & RTC_ISR_IS_16HZ_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_IS_16HZ_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_16HZ_CALLBACK, &tmp);  }
  }
  /* 32HZ callback processing                                                 */
  if((reg_ier & RTC_IER_IE_32HZ_MASK) && (reg_isr & RTC_ISR_IS_32HZ_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_IS_32HZ_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_32HZ_CALLBACK, &tmp);  }
  }
  /* 64HZ callback processing                                                 */
  if((reg_ier & RTC_IER_IE_64HZ_MASK) && (reg_isr & RTC_ISR_IS_64HZ_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_IS_64HZ_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_64HZ_CALLBACK, &tmp);  }
  }
  /* 128HZ callback processing                                                */
  if((reg_ier & RTC_IER_IE_128HZ_MASK) && (reg_isr & RTC_ISR_IS_128HZ_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_IS_128HZ_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_128HZ_CALLBACK, &tmp); }
  }
  /* 256HZ callback processing                                                */
  if((reg_ier & RTC_IER_IE_256HZ_MASK) && (reg_isr & RTC_ISR_IS_256HZ_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_IS_256HZ_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_256HZ_CALLBACK, &tmp); }
  }
  /* 512HZ callback processing                                                */
  if((reg_ier & RTC_IER_IE_512HZ_MASK) && (reg_isr & RTC_ISR_IS_512HZ_MASK))  
  {
    ioseth (RTC_ISR,RTC_ISR_IS_512HZ_MASK);
    if (pCallbackIRTC != NULL)  { pCallbackIRTC(IRTC_512HZ_CALLBACK, &tmp); }
  }      
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/