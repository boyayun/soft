/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      lptmr.c
 * @version   1.0.3.0
 * @date      May-14-2012
 * @brief     Low-Power Timer (LPTMR) driver source code.
 ******************************************************************************/
#include "common.h"
#include "lptmr.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static LPTMR_CALLBACK pCallbackLPTMR=NULL;

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
void lptmr_isr (void);

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
void LPTMR_InstallCallback (uint8 ip, LPTMR_CALLBACK pCallback)
{ 
  if (pCallback != NULL) { pCallbackLPTMR = pCallback; NVIC_SetIsr(INT_LPTMR,ip); }
}

void LPTMR_Init (tLPTMR lptmr, uint16 compValue)
{ 
  LPTMR_PSR = lptmr.PSR; 
  LPTMR_CMR = compValue;
  LPTMR_CSR = (lptmr.CSR|LPTMR_CSR_TCF_MASK); /* Timer Compare Flag           */   
}

/******************************************************************************
 * interrupt functions definitions                                            *
 ******************************************************************************/
void lptmr_isr (void) 
{ 
  iosetw (LPTMR_CSR,LPTMR_CSR_TCF_MASK);      /* clear interrupt flag         */
  /* call interrupt function                                                  */
  if (pCallbackLPTMR != (LPTMR_CALLBACK)NULL) { pCallbackLPTMR (); }
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/