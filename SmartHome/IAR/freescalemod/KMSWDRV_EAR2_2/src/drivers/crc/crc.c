/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      crc.c
 * @version   1.0.1.0
 * @date      May-4-2012
 * @brief     Cyclic Redundancy Check (CRC) driver source code.
 ******************************************************************************/
#include "common.h"
#include "crc.h"

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
#pragma diag_suppress=Pa082
void* CRC_INIT (tCRC crc)
{    
  CRC_CTRL  = (crc.CTRL & ~CRC_CTRL_WAS_MASK);
  CRC_GPOLY = crc.GPOLY;
  CRC_CTRL |= CRC_CTRL_WAS_MASK;
  CRC_DATA  = crc.DATA;
  CRC_CTRL &= ~CRC_CTRL_WAS_MASK;
  return (void*)CRC_BASE_PTR;
}

uint32 CRC_CALC (const uint8 *ptr, uint32 len)
{    
  register uint32 i;

  for (i = 0; i < len; i++) { (*(vuint8*)(CRC_BASE_PTR+0x0000)) = (*(ptr+i)); }  
  if (CRC_CTRL & CRC_CTRL_TCRC_MASK) { return (uint32)(CRC_DATA & 0xFFFFFFFF); }
  else                               { return (uint32)(CRC_DATA & 0x0000FFFF); }
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/