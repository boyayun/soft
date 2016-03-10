/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      tmr.c
 * @version   1.0.2.0
 * @date      May-14-2012
 * @brief     Quad Timer (TMR) driver source code.
 ******************************************************************************/
#include "common.h"
#include "tmr.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static TMR_CH_CALLBACK pCallbackTMR0=NULL;
static TMR_CH_CALLBACK pCallbackTMR1=NULL;
static TMR_CH_CALLBACK pCallbackTMR2=NULL;
static TMR_CH_CALLBACK pCallbackTMR3=NULL;

/******************************************************************************
 * interrupt functions prototype                                              *
 ******************************************************************************/
void TMR0_isr (void);
void TMR1_isr (void);
void TMR2_isr (void);
void TMR3_isr (void);

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
void TMR_CH0_Init (tTMR_CH ch, uint16 comp1, uint16 comp2, uint16 load, 
                   uint16 cmpld1, uint16 cmpld2, uint8 ip, TMR_CH_CALLBACK pCallback)
{
  if (pCallback != NULL) { pCallbackTMR0 = pCallback; NVIC_SetIsr(INT_TMR0,ip); }  
  TMR0_ENBL   &= ~TMR_ENBL_ENBL(0);
  TMR0_CNTR    = load;
  TMR0_ENBL   |= TMR_ENBL_ENBL(0);
  
  TMR0_COMP1   = comp1;
  TMR0_COMP2   = comp2;
  TMR0_LOAD    = load;
  TMR0_CMPLD1  = cmpld1;
  TMR0_CMPLD2  = cmpld2;
  
  TMR0_SCTRL   = ch.SCTRL;
  TMR0_CSCTRL  = ch.CSCTRL;
  TMR0_CTRL    = ch.CTRL;
}

void TMR_CH1_Init (tTMR_CH ch, uint16 comp1, uint16 comp2, uint16 load, 
                   uint16 cmpld1, uint16 cmpld2, uint8 ip, TMR_CH_CALLBACK pCallback)
{
if (pCallback != NULL) { pCallbackTMR1 = pCallback; NVIC_SetIsr(INT_TMR1,ip); }  
  TMR0_ENBL   &= ~TMR_ENBL_ENBL(1);
  TMR1_CNTR    = load;
  TMR0_ENBL   |= TMR_ENBL_ENBL(1);
  
  TMR1_COMP1   = comp1;
  TMR1_COMP2   = comp2;
  TMR1_LOAD    = load;
  TMR1_CMPLD1  = cmpld1;
  TMR1_CMPLD2  = cmpld2;
  
  TMR1_SCTRL   = ch.SCTRL;
  TMR1_CSCTRL  = ch.CSCTRL;
  TMR1_CTRL    = ch.CTRL;
}

void TMR_CH2_Init (tTMR_CH ch, uint16 comp1, uint16 comp2, uint16 load, 
                   uint16 cmpld1, uint16 cmpld2, uint8 ip, TMR_CH_CALLBACK pCallback)
{
  if (pCallback != NULL) { pCallbackTMR2 = pCallback; NVIC_SetIsr(INT_TMR2,ip); }
  TMR0_ENBL   &= ~TMR_ENBL_ENBL(2);
  TMR2_CNTR    = load;
  TMR0_ENBL   |= TMR_ENBL_ENBL(2);
  
  TMR2_COMP1   = comp1;
  TMR2_COMP2   = comp2;
  TMR2_LOAD    = load;
  TMR2_CMPLD1  = cmpld1;
  TMR2_CMPLD2  = cmpld2;
  
  TMR2_SCTRL   = ch.SCTRL;
  TMR2_CSCTRL  = ch.CSCTRL;
  TMR2_CTRL    = ch.CTRL;
}

void TMR_CH3_Init (tTMR_CH ch, uint16 comp1, uint16 comp2, uint16 load, 
                   uint16 cmpld1, uint16 cmpld2, uint8 ip, TMR_CH_CALLBACK pCallback)
{
  if (pCallback != NULL) { pCallbackTMR3 = pCallback; NVIC_SetIsr(INT_TMR3,ip); }
  TMR0_ENBL   &= ~TMR_ENBL_ENBL(3);
  TMR3_CNTR    = load;
  TMR0_ENBL   |= TMR_ENBL_ENBL(3);
  
  TMR3_COMP1   = comp1;
  TMR3_COMP2   = comp2;
  TMR3_LOAD    = load;
  TMR3_CMPLD1  = cmpld1;
  TMR3_CMPLD2  = cmpld2;
  
  TMR3_SCTRL   = ch.SCTRL;
  TMR3_CSCTRL  = ch.CSCTRL;
  TMR3_CTRL    = ch.CTRL;
}

/******************************************************************************
 * interrupt functions definitions                                            *
 ******************************************************************************/
void TMR0_isr (void) 
{   
  /* process TMR channel 0 interrupt                                          */
  /* Check for Compare flag only it its interrupt is enabled                  */
  if (((TMR0_SCTRL&TMR_SCTRL_TCFIE_MASK) == TMR_SCTRL_TCFIE_MASK))
  {
    if (((TMR0_SCTRL&TMR_SCTRL_TCF_MASK) == TMR_SCTRL_TCF_MASK))
    {
      ioclrh (TMR0_SCTRL,TMR_SCTRL_TCF_MASK);  /* clear interrupt flag       */
      if (pCallbackTMR0 != (TMR_CH_CALLBACK)NULL) 
        pCallbackTMR0 (COMPARE_CALLBACK,0);
    }
  }
  /* Check for Input capture flag only it its interrupt is enabled            */
  if (((TMR0_SCTRL&TMR_SCTRL_IEFIE_MASK) == TMR_SCTRL_IEFIE_MASK))
  {
    if (((TMR0_SCTRL&TMR_SCTRL_IEF_MASK) == TMR_SCTRL_IEF_MASK))
    {
      ioclrh (TMR0_SCTRL,TMR_SCTRL_IEF_MASK);  /* clear interrupt flag       */
      if (pCallbackTMR0 != (TMR_CH_CALLBACK)NULL) 
        pCallbackTMR0 (CAPTURE_CALLBACK,0);
    }
  }
  /* Check for Overflow flag only it its interrupt is enabled                 */
  if (((TMR0_SCTRL&TMR_SCTRL_TOFIE_MASK) == TMR_SCTRL_TOFIE_MASK))
  {
    if (((TMR0_SCTRL&TMR_SCTRL_TOF_MASK) == TMR_SCTRL_TOF_MASK))
    {
      ioclrh (TMR0_SCTRL,TMR_SCTRL_TOF_MASK);  /* clear interrupt flag       */
      if (pCallbackTMR0 != (TMR_CH_CALLBACK)NULL) 
        pCallbackTMR0 (ROLLOVER_CALLBACK,0);
    }
  }
}

void TMR1_isr (void) 
{   
  /* process TMR channel 1 interrupt                                          */
  /* Check for Compare flag only it its interrupt is enabled                  */
  if (((TMR1_SCTRL&TMR_SCTRL_TCFIE_MASK) == TMR_SCTRL_TCFIE_MASK))
  {
    if (((TMR1_SCTRL&TMR_SCTRL_TCF_MASK) == TMR_SCTRL_TCF_MASK))
    {
      ioclrh (TMR1_SCTRL,TMR_SCTRL_TCF_MASK);  /* clear interrupt flag       */
      if (pCallbackTMR1 != (TMR_CH_CALLBACK)NULL) 
        pCallbackTMR1 (COMPARE_CALLBACK,0);
    }
  }
  /* Check for Input capture flag only it its interrupt is enabled            */
  if (((TMR1_SCTRL&TMR_SCTRL_IEFIE_MASK) == TMR_SCTRL_IEFIE_MASK))
  {
    if (((TMR1_SCTRL&TMR_SCTRL_IEF_MASK) == TMR_SCTRL_IEF_MASK))
    {
      ioclrh (TMR1_SCTRL,TMR_SCTRL_IEF_MASK);  /* clear interrupt flag       */
      if (pCallbackTMR1 != (TMR_CH_CALLBACK)NULL) 
        pCallbackTMR1 (CAPTURE_CALLBACK,0);
    }
  }
   /* Check for Overflow flag only it its interrupt is enabled                */
  if (((TMR1_SCTRL&TMR_SCTRL_TOFIE_MASK) == TMR_SCTRL_TOFIE_MASK))
  {
    if (((TMR1_SCTRL&TMR_SCTRL_TOF_MASK) == TMR_SCTRL_TOF_MASK))
    {
      ioclrh (TMR1_SCTRL,TMR_SCTRL_TOF_MASK);  /* clear interrupt flag       */
      if (pCallbackTMR1 != (TMR_CH_CALLBACK)NULL) 
        pCallbackTMR1 (ROLLOVER_CALLBACK,0);
    }
  }
}

void TMR2_isr (void) 
{   
  /* process TMR channel 2 interrupt                                          */
  /* Check for Compare flag only it its interrupt is enabled                  */
  if (((TMR2_SCTRL&TMR_SCTRL_TCFIE_MASK) == TMR_SCTRL_TCFIE_MASK))
  {
    if (((TMR2_SCTRL&TMR_SCTRL_TCF_MASK) == TMR_SCTRL_TCF_MASK))
    {
      ioclrh (TMR2_SCTRL,TMR_SCTRL_TCF_MASK);  /* clear interrupt flag       */
      if (pCallbackTMR2 != (TMR_CH_CALLBACK)NULL) 
        pCallbackTMR2 (COMPARE_CALLBACK,0);
    }
  }
  /* Check for Input capture flag only it its interrupt is enabled            */
  if (((TMR2_SCTRL&TMR_SCTRL_IEFIE_MASK) == TMR_SCTRL_IEFIE_MASK))
  {
    if (((TMR2_SCTRL&TMR_SCTRL_IEF_MASK) == TMR_SCTRL_IEF_MASK))
    {
      ioclrh (TMR2_SCTRL,TMR_SCTRL_IEF_MASK);  /* clear interrupt flag       */
      if (pCallbackTMR2 != (TMR_CH_CALLBACK)NULL) 
        pCallbackTMR2 (CAPTURE_CALLBACK,0);
    }
  }
   /* Check for Overflow flag only it its interrupt is enabled                */
  if (((TMR2_SCTRL&TMR_SCTRL_TOFIE_MASK) == TMR_SCTRL_TOFIE_MASK))
  {
    if (((TMR2_SCTRL&TMR_SCTRL_TOF_MASK) == TMR_SCTRL_TOF_MASK))
    {
      ioclrh (TMR2_SCTRL,TMR_SCTRL_TOF_MASK);  /* clear interrupt flag       */
      if (pCallbackTMR2 != (TMR_CH_CALLBACK)NULL) 
        pCallbackTMR2 (ROLLOVER_CALLBACK,0);
    }
   }
}

void TMR3_isr (void) 
{   
  /* process TMR channel 3 interrupt                                          */
  /* Check for Compare flag only it its interrupt is enabled                  */
  if (((TMR3_SCTRL&TMR_SCTRL_TCFIE_MASK) == TMR_SCTRL_TCFIE_MASK))
  {
    if (((TMR3_SCTRL&TMR_SCTRL_TCF_MASK) == TMR_SCTRL_TCF_MASK))
    {
      ioclrh (TMR3_SCTRL,TMR_SCTRL_TCF_MASK);  /* clear interrupt flag       */
      if (pCallbackTMR3 != (TMR_CH_CALLBACK)NULL) 
        pCallbackTMR3 (COMPARE_CALLBACK,0);
    }
  }
  /* Check for Input capture flag only it its interrupt is enabled            */
  if (((TMR3_SCTRL&TMR_SCTRL_IEFIE_MASK) == TMR_SCTRL_IEFIE_MASK))
  {
    if (((TMR3_SCTRL&TMR_SCTRL_IEF_MASK) == TMR_SCTRL_IEF_MASK))
    {
      ioclrh (TMR3_SCTRL,TMR_SCTRL_IEF_MASK);  /* clear interrupt flag       */
      if (pCallbackTMR3 != (TMR_CH_CALLBACK)NULL) 
        pCallbackTMR3 (CAPTURE_CALLBACK,0);
    }
  }
   /* Check for Overflow flag only it its interrupt is enabled                */
  if (((TMR3_SCTRL&TMR_SCTRL_TOFIE_MASK) == TMR_SCTRL_TOFIE_MASK))
  {
    if (((TMR3_SCTRL&TMR_SCTRL_TOF_MASK) == TMR_SCTRL_TOF_MASK))
    {
      ioclrh (TMR3_SCTRL,TMR_SCTRL_TOF_MASK);  /* clear interrupt flag       */
      if (pCallbackTMR3 != (TMR_CH_CALLBACK)NULL) 
        pCallbackTMR3 (ROLLOVER_CALLBACK,0);
    }
  }
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
