/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      llwu.c
 * @version   1.0.3.0
 * @date      Mar-10-2013
 * @brief     Low-Leakage Wakeup Unit (LLWU) driver source code.
 ******************************************************************************/
#include "common.h"
#include "llwu.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static LLWU_CALLBACK pCallbackLLWU=NULL;

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
void llwu_isr (void);

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
#pragma diag_suppress=Pa082
void LLWU_Init (tLLWU llwu, uint8 ip, LLWU_CALLBACK pCallback)
{
  if (pCallback != NULL) { pCallbackLLWU = pCallback; NVIC_SetIsr(INT_LLWU,ip); }
  LLWU_PE1    = llwu.PE1;
  LLWU_PE2    = llwu.PE2;
  LLWU_PE3    = llwu.PE3;
  LLWU_PE4    = llwu.PE4;
  LLWU_ME     = llwu.ME;
  LLWU_FILT1  = llwu.FILT1;
  LLWU_FILT2  = llwu.FILT2;
  
  /* clear flags                                                              */
  LLWU_F1    = LLWU_F1;
  LLWU_F2    = LLWU_F2;
  LLWU_FILT1 = LLWU_FILT1;
  LLWU_FILT2 = LLWU_FILT2;
}

/******************************************************************************
 * interrupt functions definitions                                            *
 ******************************************************************************/
void llwu_isr (void) 
{ 
  register uint32 tmp = ((uint32)(LLWU_FILT2&0x7f)<<18) |
                        ((uint32)(LLWU_FILT1&0x7f)<<17) |
                        ((uint32)LLWU_F3          <<16) | 
                        ((uint32)LLWU_F2          << 8) | 
                        (uint32)LLWU_F1;
  /* clear flags                                                              */
  LLWU_F1     = LLWU_F1;
  LLWU_F2     = LLWU_F2;
  LLWU_FILT1  = LLWU_FILT1;
  LLWU_FILT2  = LLWU_FILT2;
  
  /* call callback function                                                   */
  if (pCallbackLLWU != (LLWU_CALLBACK)NULL) { pCallbackLLWU (tmp); }
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/