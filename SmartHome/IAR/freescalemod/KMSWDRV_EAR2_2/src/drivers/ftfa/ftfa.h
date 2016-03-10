/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      ftfa.h
 * @version   1.0.3.0
 * @date      Feb-24-2013
 * @brief     Flash Memory Module (FTFA) driver header file.
 ******************************************************************************/
#ifndef __FTFA_H
#define __FTFA_H 

/******************************************************************************
* FTFA function and macro definitions
*
*//*! @addtogroup ftfa_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Writes array into flash memory.
 * @details This function writes array into flash memory starting at address 
 *          specified by pDst parameter.
 * @param   pDst      pointer to the starting address of the flash memory. The 
 *                    supplied address must be longword aligned.
 * @param   pSrc      pointer to the source array in RAM to be written 
 * @param   size      size of the array in number of bytes (must be aligned to
 *                    long words)
 * @return  number of bytes written into flash memory
 * @note    Implemented as a function call.
 * @warning Re-programming of existing 0s to 0 is not allowed as this 
 *          overstresses the device. This function executes from RAM.
 * @see     @ref FTFA_WriteW32, @ref FTFA_EraseSector, @ref FTFA_ReadArray 
 ******************************************************************************/
#define FTFA_WriteArray(pDst,pSrc,size)  _FTFA_WriteArray (pDst,pSrc,size)

/***************************************************************************//*!
 * @brief   Reads array from flash memory.
 * @details This function reads array from flash memory starting at address 
 *          specified by pSrc parameter.
 * @param   pSrc      pointer to the starting of the flash memory. The supplied 
 *                    address must be longword aligned.
 * @param   pDst      pointer to the destination array in RAM
 * @param   size      size of the array in number of bytes (must be aligned to
 *                    long words)
 * @return  number of bytes read from flash memory
 * @note    Implemented as a function call. You can use also memcpy() function.
 * @see     @ref FTFA_WriteArray
 ******************************************************************************/
#define FTFA_ReadArray(pSrc,pDst,size)  _FTFA_ReadArray (pSrc,pDst,size)

/***************************************************************************//*!
 * @brief   Write long word into flash memory.
 * @details This function writes long word into flash memory starting at address 
 *          specified by pDst parameter.
 * @param   pDst      pointer to the starting address of the flash memory. The 
 *                    supplied address must be longword aligned.
 * @param   val       @ref uint32 value to be written into flash memory
 * @return  number of bytes written into flash memory
 * @note    Implemented as a function call.
 * @warning This function executes from RAM.
 * @see     @ref FTFA_WriteArray, @ref FTFA_EraseSector
 ******************************************************************************/
#define FTFA_WriteW32(pDst,val)         _FTFA_WriteW32(pDst,val)

/***************************************************************************//*!
 * @brief   Erases all bytes in the 1 KB sector.
 * @details This function erases all bytes in the 1 KB sector of the flash memory 
 *          starting at address in the flash sector to be erased.
 * @param   pDst      pointer to the starting address of the flash memory. The 
 *                    supplied address must be longword aligned.
 * @note    Implemented as a function call.
 * @warning Use erase operation to restore bits from '0' to '1'; bits cannot be 
 *          programmed from a '0' to a '1'. This function executes from RAM.
 * @see     @ref FTFA_WriteW32, @ref FTFA_WriteArray 
 ******************************************************************************/
#define FTFA_EraseSector(pDst)          _FTFA_EraseSector(pDst)
/*! @} End of ftfa_macro                                                      */
  
/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/    
extern int _FTFA_WriteArray (const void *pDst, void *pSrc, int size);
extern int _FTFA_ReadArray (void *pSrc, void *pDst, int size);
#if defined (__ICCARM__)      /* IAR                                          */
  extern __ramfunc int _FTFA_WriteW32 (const uint32 *pDst, uint32 val);
  extern __ramfunc void _FTFA_EraseSector (const void *pDst);
#elif defined(__GNUC__)       /* CW GCC                                       */  
  extern int _FTFA_WriteW32 (const uint32 *pDst, uint32 val) __attribute__ ((section(".fast")));
  extern void _FTFA_EraseSector (const void *pDst) __attribute__ ((section(".fast")));
#endif  

#endif /* __FTFA_H */ 