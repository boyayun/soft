/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      swisr.c
 * @version   1.0.2.0
 * @date      May-14-2012
 * @brief     Software Interrupt (SWISR) driver source code.
 ******************************************************************************/
#include "common.h"
#include "swisr.h"

/******************************************************************************
 * public data type definitions                                               *
 ******************************************************************************/
uint16 swisrcnt[] = {0,0,0};

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static SWISR_CALLBACK pCallback0 = NULL;
static SWISR_CALLBACK pCallback1 = NULL;
static SWISR_CALLBACK pCallback2 = NULL;

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
void swisr0_isr  (void);
void swisr1_isr  (void);
void swisr2_isr  (void);

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
#pragma diag_suppress=Pa082
void SWISR0_HandlerInit  (uint16 ip, SWISR_CALLBACK pCallback)
{
  if (pCallback != NULL) { pCallback0 = pCallback; NVIC_SetIsr(SWISR_VECTOR_0,ip); }
}

void SWISR1_HandlerInit  (uint16 ip, SWISR_CALLBACK pCallback)
{
  if (pCallback != NULL) { pCallback1 = pCallback; NVIC_SetIsr(SWISR_VECTOR_1,ip); }
}

void SWISR2_HandlerInit  (uint16 ip, SWISR_CALLBACK pCallback)
{
  if (pCallback != NULL) { pCallback2 = pCallback; NVIC_SetIsr(SWISR_VECTOR_2,ip); }
}

/******************************************************************************
 * interrupt functions definitions                                            *
 ******************************************************************************/
void swisr0_isr (void) { if (pCallback0 != (SWISR_CALLBACK)NULL) pCallback0 (); } 
void swisr1_isr (void) { if (pCallback1 != (SWISR_CALLBACK)NULL) pCallback1 (); } 
void swisr2_isr (void) { if (pCallback2 != (SWISR_CALLBACK)NULL) pCallback2 (); } 

/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/