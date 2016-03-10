/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      afe.c
 * @version   1.0.2.0
 * @date      Jun-8-2013
 * @brief     Analog Front End (AFE) driver source code.
 ******************************************************************************/
#include "common.h"
#include "afe.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static AFE_CH_CALLBACK pCallbackAFE_CH0=NULL;
static AFE_CH_CALLBACK pCallbackAFE_CH1=NULL;
static AFE_CH_CALLBACK pCallbackAFE_CH2=NULL;
static AFE_CH_CALLBACK pCallbackAFE_CH3=NULL;  

/******************************************************************************
 * interrupt functions prototype                                              *
 ******************************************************************************/
void AFE_CH0_isr (void);
void AFE_CH1_isr (void);
void AFE_CH2_isr (void);
void AFE_CH3_isr (void);

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
void AFE_CH0_Init(tAFE_CH ch, int16 dly, uint8 ip, AFE_CH_CALLBACK pCallback)
{
  if (pCallback != NULL) 
  { 
    pCallbackAFE_CH0 = pCallback; 
    NVIC_SetIsr(INT_AFE0,ip); 
    AFE_DI |= AFE_DI_INTEN0_MASK;
  }
  AFE_CH0_DR  = ((dly>0)?dly:0);        /* update only if value is positive   */
  AFE_CH0_CFR = ch.CFR;
}

void AFE_CH1_Init(tAFE_CH ch, int16 dly, uint8 ip, AFE_CH_CALLBACK pCallback)
{
  if (pCallback != NULL) 
  { 
    pCallbackAFE_CH1 = pCallback; 
    NVIC_SetIsr(INT_AFE1,ip); 
    AFE_DI |= AFE_DI_INTEN1_MASK;
  }
  AFE_CH1_DR  = ((dly>0)?dly:0);        /* update only if value is positive   */
  AFE_CH1_CFR = ch.CFR;
}

void AFE_CH2_Init(tAFE_CH ch, int16 dly, uint8 ip, AFE_CH_CALLBACK pCallback)
{
  if (pCallback != NULL) 
  { 
    pCallbackAFE_CH2 = pCallback; 
    NVIC_SetIsr(INT_AFE2,ip);
    AFE_DI |= AFE_DI_INTEN2_MASK; 
  }
  AFE_CH2_DR  = ((dly>0)?dly:0);        /* update only if value is positive   */
  AFE_CH2_CFR = ch.CFR;
}

void AFE_CH3_Init(tAFE_CH ch, int16 dly, uint8 ip, AFE_CH_CALLBACK pCallback)
{
  if (pCallback != NULL) 
  { 
    pCallbackAFE_CH3 = pCallback; 
    NVIC_SetIsr(INT_AFE3,ip); 
    AFE_DI |= AFE_DI_INTEN3_MASK;
  }
  AFE_CH3_DR  = ((dly>0)?dly:0);        /* update only if value is positive   */
  AFE_CH3_CFR = ch.CFR;
}

/******************************************************************************
 * interrupt function definitions                                             *
 ******************************************************************************/
#pragma diag_suppress=Pa082
void AFE_CH0_isr (void)
{ 
  register uint32 tmp = AFE_SR; 
  
  if (pCallbackAFE_CH0 != (AFE_CH_CALLBACK)NULL) 
  {       
    if (tmp & AFE_SR_COC0_MASK) { pCallbackAFE_CH0 (COC_CALLBACK, AFE_CH0_RR); }
    if (tmp & AFE_SR_OVR0_MASK) { pCallbackAFE_CH0 (OVR_CALLBACK, AFE_CH0_RR); }
  }    
}

void AFE_CH1_isr (void)
{ 
  register uint32 tmp = AFE_SR;
  
  if (pCallbackAFE_CH1 != (AFE_CH_CALLBACK)NULL) 
  {       
    if (tmp & AFE_SR_COC1_MASK) { pCallbackAFE_CH1 (COC_CALLBACK, AFE_CH1_RR); }
    if (tmp & AFE_SR_OVR1_MASK) { pCallbackAFE_CH1 (OVR_CALLBACK, AFE_CH1_RR); }
  }   
}

void AFE_CH2_isr (void)
{
  register uint32 tmp = AFE_SR;
 
  if (pCallbackAFE_CH2 != (AFE_CH_CALLBACK)NULL) 
  {       
    if (tmp & AFE_SR_COC2_MASK) { pCallbackAFE_CH2 (COC_CALLBACK, AFE_CH2_RR); }
    if (tmp & AFE_SR_OVR2_MASK) { pCallbackAFE_CH2 (OVR_CALLBACK, AFE_CH2_RR); }
  }    
}

void AFE_CH3_isr (void)
{ 
  register uint32 tmp = AFE_SR;
  
  if (pCallbackAFE_CH3 != (AFE_CH_CALLBACK)NULL) 
  {       
    if (tmp & AFE_SR_COC3_MASK) { pCallbackAFE_CH3 (COC_CALLBACK, AFE_CH3_RR); }
    if (tmp & AFE_SR_OVR3_MASK) { pCallbackAFE_CH3 (OVR_CALLBACK, AFE_CH3_RR); }
  }    
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/