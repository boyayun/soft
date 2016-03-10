/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      rnga.c
 * @version   1.0.3.0
 * @date      May-14-2012
 * @brief     Random Number Generator Accelerator (RNGA) source code.
 ******************************************************************************/
#include "common.h"
#include "rnga.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static RNGA_ERROR_CALLBACK volatile pCallbackRNGA=NULL;

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
void rnga_isr (void);

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
void RNGA_Init (tRNGA rnga, uint8 ip, RNGA_ERROR_CALLBACK pCallback)
{  
  if (pCallback != NULL) 
  { 
    RNG_CR = (rnga.CR | RNG_CR_INTM_MASK);
    pCallbackRNGA = pCallback; 
    NVIC_SetIsr(INT_RNGA,ip);
  }
  else
    RNG_CR = rnga.CR;  
}

/******************************************************************************
 * interrupt functions definitions                                            *
 ******************************************************************************/
void rnga_isr (void) 
{   
  iosetw (RNG_CR,RNG_CR_CLRI_MASK);   /* clear ERRI interrupt flag            */
  if (pCallbackRNGA != (RNGA_ERROR_CALLBACK)NULL) { pCallbackRNGA ();  }
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/