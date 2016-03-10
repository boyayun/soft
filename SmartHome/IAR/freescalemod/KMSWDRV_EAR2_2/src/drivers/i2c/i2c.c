/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      i2c.c
 * @version   1.0.2.0
 * @date      Dec-20-2011
 * @brief     I2C bus controller (I2C) driver source code.
******************************************************************************/
#include "common.h"
#include "i2c.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static I2C_CALLBACK volatile pCallbackI2C0_I2C1=NULL;

/******************************************************************************
 * interrupt functions prototype                                              *
 ******************************************************************************/
void I2C0_I2C1_isr (void);

/******************************************************************************
 * Public functions definitions                                               *
 ******************************************************************************/
#pragma diag_suppress=Pa082
void I2C_InstallCallback (uint8 ip, I2C_CALLBACK pCallback)
{
  if (pCallback != NULL)  
  { 
    pCallbackI2C0_I2C1 = pCallback; 
    NVIC_SetIsr(INT_I2C0_I2C1,ip); 
  }
}

void I2C0_Init (tI2C i2c)
{
  I2C0_A1    = i2c.A1;
  I2C0_F     = i2c.F;
  I2C0_C1    = i2c.C1;
  I2C0_C2    = i2c.C2;
  I2C0_FLT   = i2c.FLT;
  I2C0_RA    = i2c.RA;
  I2C0_SMB   = i2c.SMB;
  I2C0_A2    = i2c.A2;
  I2C0_SLTH  = i2c.SLTH;
  I2C0_SLTL  = i2c.SLTL;
}

void I2C1_Init (tI2C i2c)
{       
  I2C1_A1    = i2c.A1;
  I2C1_F     = i2c.F;
  I2C1_C1    = i2c.C1;
  I2C1_C2    = i2c.C2;
  I2C1_FLT   = i2c.FLT;
  I2C1_RA    = i2c.RA;
  I2C1_SMB   = i2c.SMB;
  I2C1_A2    = i2c.A2;
  I2C1_SLTH  = i2c.SLTH;
  I2C1_SLTL  = i2c.SLTL;
}

/******************************************************************************
 * interrupt functions definitions                                            *
 ******************************************************************************/
void I2C0_I2C1_isr (void) 
{ 
  /* process I2C0 interrupt                                                   */
  if (SIM_SCGC4 & SIM_SCGC4_I2C0_MASK)
  {
    if (I2C_GetIsrFlag(I2C0))
    {
      I2C_ClrIsrFlag(I2C0);
      pCallbackI2C0_I2C1 (I2C0_CALLBACK);
    }
  }
  
  /* process I2C1 interrupt                                                   */
  if (SIM_SCGC4 & SIM_SCGC4_I2C1_MASK)
  {
    if (I2C_GetIsrFlag(I2C1))
    {
      I2C_ClrIsrFlag(I2C1);
      pCallbackI2C0_I2C1 (I2C1_CALLBACK);
    }
  }
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/