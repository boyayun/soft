/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      ftfa.c
 * @version   1.0.3.0
 * @date      October-06-2013
 * @brief     Flash Memory Module (FTFA) driver source code.
 ******************************************************************************/
#include "common.h"
#include "ftfa.h"

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
int _FTFA_WriteArray (const void *pDst, void *pSrc, int size)
{
  register int n = 0;
  register uint32 *pUlongDst = (uint32*)pDst; 
  register uint32 *pUlongSrc = (uint32*)pSrc;
  
  while ((size-n) > 0) { n+=_FTFA_WriteW32 (pUlongDst++, *(pUlongSrc++)); }

  return n;
} 

int _FTFA_ReadArray (void *pSrc, void *pDst, int size)
{
  register uint8 *src = (uint8*)pSrc;
  register uint8 *dst = (uint8*)pDst;
  register int n = size;
  
  while (n--) { *dst++ = *src++; }
  
  return size;
}

#if defined (__ICCARM__)      /* IAR                                          */
  __ramfunc int _FTFA_WriteW32 (const uint32 *pDst, uint32 val)
#elif defined(__GNUC__)       /* CW GCC                                       */  
            int _FTFA_WriteW32 (const uint32 *pDst, uint32 val)
#endif 
{
  if ((FTFA_FSTAT & FTFA_FSTAT_FPVIOL_MASK) || 
      (FTFA_FSTAT & FTFA_FSTAT_ACCERR_MASK)) { FTFA_FSTAT |= 0x30; }  
  FTFA_FCCOB0 = 0x06;
  FTFA_FCCOB1 = (uint8)((uint32)pDst>> 16);
  FTFA_FCCOB2 = (uint8)((uint32)pDst>>  8);
  FTFA_FCCOB3 = (uint8)((uint32)pDst>>  0);
  FTFA_FCCOB4 = (uint8)(val >> 24);
  FTFA_FCCOB5 = (uint8)(val >> 16);
  FTFA_FCCOB6 = (uint8)(val >>  8);
  FTFA_FCCOB7 = (uint8)(val >>  0);    
  FTFA_FSTAT |= FTFA_FSTAT_CCIF_MASK;    
  while (!(FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK));  
  
  return sizeof(uint32);
}

#if defined (__ICCARM__)      /* IAR                                          */
  __ramfunc void _FTFA_EraseSector (const void *pDst)
#elif defined(__GNUC__)       /* CW GCC                                       */  
            void _FTFA_EraseSector (const void *pDst) 
#endif 
{
  if ((FTFA_FSTAT & FTFA_FSTAT_FPVIOL_MASK) || 
      (FTFA_FSTAT & FTFA_FSTAT_ACCERR_MASK)) { FTFA_FSTAT |= 0x30; }
  FTFA_FCCOB0 = 0x09;
  FTFA_FCCOB1 = (uint8)((uint32)pDst>> 16);
  FTFA_FCCOB2 = (uint8)((uint32)pDst>>  8);
  FTFA_FCCOB3 = (uint8)((uint32)pDst>>  0); 
  FTFA_FSTAT |= FTFA_FSTAT_CCIF_MASK;  
  while (!(FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK));
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/