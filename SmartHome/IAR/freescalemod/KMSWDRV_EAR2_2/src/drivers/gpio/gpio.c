/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      gpio.c
 * @version   1.0.4.0
 * @date      Mar-10-2013
 * @brief     General-Purpose Input/Output (GPIO) driver source code.
 ******************************************************************************/
#include "common.h"
#include "gpio.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static PORT_CALLBACK pCallbackPTx=NULL;

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
void PTx_isr (void);

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
void PORT_InstallCallback (uint8 ip, PORT_CALLBACK pCallback)
{
  if (pCallback != NULL) { pCallbackPTx = pCallback; NVIC_SetIsr(INT_PTx,ip); }
}

#pragma diag_suppress=Pa082
void PORTA_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTA_BASE_PTR->PCR[i] = port.PCR; } 
  PORTA_ISFR &= ~pin_mask;  
}

void PORTB_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;

  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTB_BASE_PTR->PCR[i] = port.PCR; }
  PORTB_ISFR &= ~pin_mask;  
}

void PORTC_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTC_BASE_PTR->PCR[i] = port.PCR; }
  PORTC_ISFR &= ~pin_mask;
}

void PORTD_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;

  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTD_BASE_PTR->PCR[i] = port.PCR; }
  PORTD_ISFR &= ~pin_mask;  
}

void PORTE_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTE_BASE_PTR->PCR[i] = port.PCR; } 
  PORTE_DFER |= (port.DFER & pin_mask); /* DFER content OR-ed with pin_mask   */    
  PORTE_ISFR &= ~pin_mask;  
}

void PORTF_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTF_BASE_PTR->PCR[i] = port.PCR; } 
  PORTF_ISFR &= ~pin_mask;  
}

void PORTG_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTG_BASE_PTR->PCR[i] = port.PCR; } 
  PORTG_ISFR &= ~pin_mask;  
}

void PORTH_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTH_BASE_PTR->PCR[i] = port.PCR; } 
  PORTH_ISFR &= ~pin_mask;  
}

void PORTI_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTI_BASE_PTR->PCR[i] = port.PCR; } 
  PORTI_ISFR &= ~pin_mask;  
}

/******************************************************************************
 * interrupt function definitions                                             *
 ******************************************************************************/
void PTx_isr (void) 
{ 
  register uint8 tmp;
  
  /* process interrupt callback function for PORTA                            */
  if (SIM_SCGC5 & SIM_SCGC5_PORTA_MASK)
  {  
    tmp = (uint8)PORTA_ISFR;
    iosetw (PORTA_ISFR,tmp);        /* clear peripheral interrupt flags       */
    if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
      pCallbackPTx (PORTA_CALLBACK, tmp);
  }
  
  /* process interrupt callback function for PORTB                            */
  if (SIM_SCGC5 & SIM_SCGC5_PORTB_MASK)
  {   
    tmp = (uint8)PORTB_ISFR;
    iosetw (PORTB_ISFR,tmp);        /* clear peripheral interrupt flags       */
    if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
      pCallbackPTx (PORTB_CALLBACK, tmp);
  }
  
  /* process interrupt callback function for PORTC                            */ 
  if (SIM_SCGC5 & SIM_SCGC5_PORTC_MASK)
  {   
    tmp = (uint8)PORTC_ISFR;
    iosetw (PORTC_ISFR,tmp);        /* clear peripheral interrupt flags       */
    if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
      pCallbackPTx (PORTC_CALLBACK, tmp);
  }
  
  /* process interrupt callback function for PORTD                            */
  if (SIM_SCGC5 & SIM_SCGC5_PORTD_MASK)
  {  
    tmp = (uint8)PORTD_ISFR;
    iosetw (PORTD_ISFR,tmp);        /* clear peripheral interrupt flags       */
    if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
      pCallbackPTx (PORTD_CALLBACK, tmp);
  }
  
  /* process interrupt callback function for PORTE                            */
  if (SIM_SCGC5 & SIM_SCGC5_PORTE_MASK)
  {    
    tmp = (uint8)PORTE_ISFR;
    iosetw (PORTE_ISFR,tmp);        /* clear peripheral interrupt flags       */
    if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
      pCallbackPTx (PORTE_CALLBACK, tmp);
  }
  
  /* process interrupt callback function for PORTF                            */
  if (SIM_SCGC5 & SIM_SCGC5_PORTF_MASK)
  {      
    tmp = (uint8)PORTF_ISFR;
    iosetw (PORTF_ISFR,tmp);        /* clear peripheral interrupt flags       */
    if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
      pCallbackPTx (PORTF_CALLBACK, tmp);
  }
  
  /* process interrupt callback function for PORTG                            */
  if (SIM_SCGC5 & SIM_SCGC5_PORTG_MASK)
  {        
    tmp = (uint8)PORTG_ISFR;
    iosetw (PORTG_ISFR,tmp);        /* clear peripheral interrupt flags       */
    if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
      pCallbackPTx (PORTG_CALLBACK, tmp);
  }
  
  /* process interrupt callback function for PORTH                            */
  if (SIM_SCGC5 & SIM_SCGC5_PORTH_MASK)
  {        
    tmp = (uint8)PORTH_ISFR;
    iosetw (PORTH_ISFR,tmp);        /* clear peripheral interrupt flags       */
    if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
      pCallbackPTx (PORTH_CALLBACK, tmp);
  }
  
  /* process interrupt callback function for PORTI                            */
  if (SIM_SCGC5 & SIM_SCGC5_PORTI_MASK)
  {          
    tmp = (uint8)PORTI_ISFR;
    iosetw (PORTI_ISFR,tmp);        /* clear peripheral interrupt flags       */
    if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
      pCallbackPTx (PORTI_CALLBACK, tmp);
  }
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/