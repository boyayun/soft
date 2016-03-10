/******************************************************************************
 *
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 *
 * ALL RIGHTS RESERVED.
 *
 ***************************************************************************//*!
 *
 * @file      FRACLIB_Mul32.h
 *
 * @author    R55013
 *
 * @version   1.0.1.0
 *
 * @date      Sep-12-2011
 *
 * @brief     Header file containing 32x32=64 multiplication function prototypes 
 *            coded in FRACLIB_Mul32.s assembler source file.
 *
 ******************************************************************************/
#ifndef __FRACLIB_MUL32_H
#define __FRACLIB_MUL32_H

/****************************************************************************//*!
 *
 * @brief   Multiply two 32-bit fractional values generating 64-bit fractional 
 *          result.
 *
 * @param   lsrc1   - Input 32-bit fractional value.
 * @param   lsrc2   - Input 32-bit fractional value.         
 *
 * @return  Function returns 64-bit value in fractional format.
 *
 * @remarks Implementation in ASM-language. 
 *
 *******************************************************************************/
extern Frac64 FRACLIB_FFMUL32 (register Frac32 lsrc1, register Frac32 lsrc2);       

/****************************************************************************//*!
 *
 * @brief   Multiply two 32-bit signed values generating 64-bit signed result.
 *
 * @param   lsrc1   - Input 32-bit signed value.
 * @param   lsrc2   - Input 32-bit signed value.         
 *
 * @return  Function returns 64-bit value in signed integer format.
 *
 * @remarks Implementation in ASM-language. 
 *
 *******************************************************************************/
extern signed long long FRACLIB_SSMUL32 (register long lsrc1, register long lsrc2);

/****************************************************************************//*!
 *
 * @brief   Multiply 32-bit unsigned value with 32-bit signed value generating 
 *          64-bit signed result.
 *
 * @param   lsrc1   - Input 32-bit unsigned value.
 * @param   lsrc2   - Input 32-bit signed value.         
 *
 * @return  Function returns 64-bit value in signed integer format.
 *
 * @remarks Implementation in ASM-language. 
 *
 *******************************************************************************/
extern signed long long FRACLIB_USMUL32 (register unsigned long lsrc1, 
                                         register long          lsrc2);

/****************************************************************************//*!
 *
 * @brief   Multiply 32-bit signed value with 32-bit unsigned value generating 
 *          64-bit signed result.
 *
 * @param   lsrc1   - Input 32-bit signed value.
 * @param   lsrc2   - Input 32-bit unsigned value.         
 *
 * @return  Function returns 64-bit value in signed integer format.
 *
 * @remarks Implementation in ASM-language. 
 *
 *******************************************************************************/
extern signed long long FRACLIB_SUMUL32 (register          long lsrc1, 
                                         register unsigned long lsrc2);

/****************************************************************************//*!
 *
 * @brief   Multiply two 32-bit unsigned values generating 64-bit unsigned result.
 *
 * @param   lsrc1   - Input 32-bit unsigned value.
 * @param   lsrc2   - Input 32-bit unsigned value.         
 *
 * @return  Function returns 64-bit value in unsigned integer format.
 *
 * @remarks Implementation in ASM-language. 
 *
 *******************************************************************************/
extern unsigned long long FRACLIB_UUMUL32 (register unsigned long lsrc1, 
                                           register unsigned long lsrc2);

#endif /* __FRACLIB_MUL32_H */
			   
