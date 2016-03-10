/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      xbar.c
 * @version   1.0.2.0
 * @date      May-14-2012
 * @brief     Inter-Peripheral Crossbar Switch (XBAR) driver source code.
 ******************************************************************************/
#include "common.h"
#include "xbar.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static XBAR_CALLBACK pCallbackXBAR=NULL;

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
void XBAR_isr (void);

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
void XBAR_Init (tXBAR xbar, uint16 ip, XBAR_CALLBACK pCallback)
{
  if (pCallback != NULL) { pCallbackXBAR = pCallback; NVIC_SetIsr(INT_PXBAR,ip); }
  XBAR_CTRL0 = xbar.CTRL0;
}

/******************************************************************************
 * interrupt functions definitions                                            *
 ******************************************************************************/
void XBAR_isr (void) 
{ 
  ioseth (XBAR_CTRL0,XBAR_CTRL0_STS0_MASK); /* clear interrupt flag           */
  if (pCallbackXBAR != (XBAR_CALLBACK)NULL)  { pCallbackXBAR (); }
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/