/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      cmp.c
 * @version   1.0.4.0
 * @date      Feb-13-2013
 * @brief     Analog comparator (CMP) driver source code.
 ******************************************************************************/
#include "common.h"
#include "cmp.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static CMP_CALLBACK volatile pCallbackCMP0_CMP1=NULL;

/******************************************************************************
 * interrupt functions prototype                                              *
 ******************************************************************************/
void CMP0_CMP1_isr (void);

/******************************************************************************
 * Public functions definitions                                               *
 ******************************************************************************/
#pragma diag_suppress=Pa082
void CMP_InstallCallback (uint8 ip, CMP_CALLBACK pCallback)
{
  if (pCallback != NULL) 
  { 
    pCallbackCMP0_CMP1 = pCallback;
    NVIC_SetIsr(INT_CMP0_CMP1,ip);
  }  
}

void CMP0_Init (tCMP cmp, uint8 psel, uint8 msel)
{
  CMP0_CR0   = cmp.CR0;
  CMP0_FPR   = cmp.FPR;
  CMP0_SCR   = cmp.SCR;
  CMP0_DACCR = cmp.DACCR;
  CMP0_MUXCR = (uint8)((psel<<3)|msel);
  CMP0_CR1   = cmp.CR1;
}

void CMP1_Init (tCMP cmp, uint8 psel, uint8 msel)
{
  CMP1_CR0   = cmp.CR0;
  CMP1_FPR   = cmp.FPR;
  CMP1_SCR   = cmp.SCR;
  CMP1_DACCR = cmp.DACCR;
  CMP1_MUXCR = (uint8)((psel<<3)|msel);
  CMP1_CR1   = cmp.CR1;
}

/******************************************************************************
 * interrupt functions definitions                                            *
 ******************************************************************************/
void CMP0_CMP1_isr (void) 
{ 
  if (pCallbackCMP0_CMP1 != (CMP_CALLBACK)NULL) 
  {
    /* process CMP0 module interrupt                                          */
    if (SIM_SCGC4 & SIM_SCGC4_CMP0_MASK)
    {
      /* process CMP0 CFR or CFF interrupt                                    */      
      pCallbackCMP0_CMP1 (CMP0_CALLBACK, CMP0_SCR);
      iosetb (CMP0_SCR,CMP_SCR_CFF_MASK|CMP_SCR_CFR_MASK);                            
    }
    
    if (SIM_SCGC4 & SIM_SCGC4_CMP1_MASK)
    {
      /* process CMP1 CFR or CFF interrupt                                    */      
      pCallbackCMP0_CMP1 (CMP1_CALLBACK, CMP1_SCR);
      iosetb (CMP1_SCR,CMP_SCR_CFF_MASK|CMP_SCR_CFR_MASK);                            
    }
  } 
}

/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/

