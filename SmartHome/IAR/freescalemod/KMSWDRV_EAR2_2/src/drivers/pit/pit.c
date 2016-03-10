/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      pit.c
 * @version   1.0.3.0
 * @date      May-14-2012
 * @brief     Periodic Interrupt Timer (PIT) driver source code.
 ******************************************************************************/
#include "common.h"
#include "pit.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static PIT_CALLBACK pCallbackPIT0_PIT1=NULL;

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
void PIT0_PIT1_isr (void);

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
void PIT_InstallCallback (uint8 ip, PIT_CALLBACK pCallback)
{
  if (pCallback != NULL)  
  { 
    pCallbackPIT0_PIT1 = pCallback; 
    NVIC_SetIsr(INT_PIT0_PIT1,ip); 
  }
}

void PIT0_CH0_Init  (tPIT_CH ch, uint32 value)
{
  PIT0_MCR    &= ~0x0002;             /* clear MDIS bit to enables clock      */
  PIT0_LDVAL0  = value;
  PIT0_TFLG0   = PIT_TFLG_TIF_MASK;   /* clear interrupt flag                 */
  PIT0_TCTRL0  = ch.TCTRL;
}

void PIT0_CH1_Init  (tPIT_CH ch, uint32 value)
{
  PIT0_MCR    &= ~0x0002;             /* clear MDIS bit to enables clock      */
  PIT0_LDVAL1  = value;
  PIT0_TFLG1   = PIT_TFLG_TIF_MASK;   /* clear interrupt flag                 */
  PIT0_TCTRL1  = ch.TCTRL;
}

void PIT1_CH0_Init  (tPIT_CH ch, uint32 value)
{
  PIT1_MCR    &= ~0x0002;             /* clear MDIS bit to enables clock      */
  PIT1_LDVAL0  = value;
  PIT1_TFLG0   = PIT_TFLG_TIF_MASK;   /* clear interrupt flag                 */
  PIT1_TCTRL0  = ch.TCTRL;
}

void PIT1_CH1_Init  (tPIT_CH ch, uint32 value)
{
  PIT1_MCR    &= ~0x0002;             /* clear MDIS bit to enables clock      */
  PIT1_LDVAL1  = value;
  PIT1_TFLG1   = PIT_TFLG_TIF_MASK;   /* clear interrupt flag                 */
  PIT1_TCTRL1  = ch.TCTRL;
}

/******************************************************************************
 * interrupt functions definitions                                            *
 ******************************************************************************/
void PIT0_PIT1_isr (void) 
{ 
  /* process PIT0 module iterrupts                                            */
  if (SIM_SCGC6 & SIM_SCGC6_PIT0_MASK)
  {
    /* CH0 interrupt                                                          */
    if (PIT0_TFLG0 == PIT_TFLG_TIF_MASK)
    {
      PIT0_TFLG0 = PIT_TFLG_TIF_MASK;   /* clear interrupt flag               */
      if (pCallbackPIT0_PIT1 != (PIT_CALLBACK)NULL) 
        pCallbackPIT0_PIT1 (PIT0CH0_CALLBACK);
    }
  
    /* CH1 interrupt                                                          */
    if (PIT0_TFLG1 == PIT_TFLG_TIF_MASK)
    {
      PIT0_TFLG1 = PIT_TFLG_TIF_MASK;   /* clear interrupt flag               */
      if (pCallbackPIT0_PIT1 != (PIT_CALLBACK)NULL) 
        pCallbackPIT0_PIT1 (PIT0CH1_CALLBACK);
    }
  }
  
  /* process PIT1 module iterrupts                                            */
  if (SIM_SCGC6 & SIM_SCGC6_PIT1_MASK)
  {
    /* CH0 interrupt                                                          */
    if (PIT1_TFLG0 == PIT_TFLG_TIF_MASK)
    {
      PIT1_TFLG0 = PIT_TFLG_TIF_MASK;   /* clear interrupt flag               */
      if (pCallbackPIT0_PIT1 != (PIT_CALLBACK)NULL) 
        pCallbackPIT0_PIT1 (PIT1CH0_CALLBACK);
    }
    
    /* CH1 interrupt                                                          */
    if (PIT1_TFLG1 == PIT_TFLG_TIF_MASK)
    {
      PIT1_TFLG1 = PIT_TFLG_TIF_MASK;   /* clear interrupt flag               */
      if (pCallbackPIT0_PIT1 != (PIT_CALLBACK)NULL) 
        pCallbackPIT0_PIT1 (PIT1CH1_CALLBACK);
    }
  }
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/