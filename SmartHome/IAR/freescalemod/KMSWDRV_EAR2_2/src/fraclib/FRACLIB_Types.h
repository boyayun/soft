/******************************************************************************
 *
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 *
 * ALL RIGHTS RESERVED.
 *
 ***************************************************************************//*!
 *
 * @file      FRACLIB_Types.h
 *
 * @author    R55013
 *
 * @version   1.0.1.0
 *
 * @date      Sep-12-2011
 *
 * @brief     Header file containing common data types and macro definitions 
 *            required for 32-bit fractional math and metering libraries.
 *
 ******************************************************************************/
#ifndef __FRACLIB_TYPES_H
#define __FRACLIB_TYPES_H

#ifndef NULL
  #define NULL ((void*)0)
#endif

/******************************************************************************
 * common data type definitions															                  *
 ******************************************************************************/
typedef long long     Frac64; /*!< User defined 64-bit fractional data type.  */
typedef long long     Word64; /*!< User defined 64-bit integer data type.     */
typedef long 	        Frac32; /*!< User defined 32-bit fractional data type.  */
typedef long          Word32; /*!< User defined 32-bit integer data type.     */
typedef long 	        Frac24; /*!< User defined 24-bit fractional data type.  */
typedef short int     Frac16; /*!< User defined 16-bit fractional data type.  */
typedef short int     Word16; /*!< User defined 16-bit integer data type.     */	

/******************************************************************************
 * common macro defintions											                              *
 ******************************************************************************/
/*!
 * Macro for conversion double precission 64-bit floating point value into 
 * 16-bit fractional value.                                                
 */ 
#define FRAC16(x)   (Frac64)((x)*(((x)>0)?0x7fff:0x8000)+                     \
                            (double)(((x)>0)?(double)0.5:-0.5))
#define F16TODBL(x) (double)(((double)(x))/(double)0x8000)

/*!
 * Macro for conversion double precission 64-bit floating point value into 
 * 24-bit fractional value.                                                
 */ 
#define FRAC24(x)   (Frac64)((x)*(((x)>0)?0x7fffff:0x800000)+                 \
                            (double)(((x)>0)?0.5:-0.5))
#define F24TODBL(x) (double)(((double)(x))/(double)0x800000)

/*!
 * Macro for conversion double precission 64-bit floating point value into 
 * 32-bit fractional value.                                                
 */ 
#define FRAC32(x)   (Frac64)((x)*(((x)>0)?0x7fffffff:0x80000000)+             \
                            (double)(((x)>0)?0.5:-0.5))
#define F32TODBL(x) (double)(((double)(x))/(double)0x80000000)

/*!
 * Macro for conversion double precission 64-bit floating point value into 
 * 48-bit fractional value.                                                
 */ 
#define FRAC48(x)   (Frac64)((x)*(((x)>0)?0x7fffffffffff:0x800000000000)+     \
                            (double)(((x)>0)?0.5:-0.5))
#define F48TODBL(x) (double)(((double)(x))/(double)0x800000000000)

/*!
 * Macro for conversion double precission 64-bit floating point value into 
 * 64-bit fractional value.                                                
 */
#define FRAC64(x)   (Frac64)((x)*                                             \
                            (((x)>0)?0x7fffffffffffffff:0x8000000000000000)+  \
                            (double)(((x)>0)?0.5:-0.5))
#define F64TODBL(x) (double)(((double)(x))/(double)0x8000000000000000)

#endif /* __FRACLIB_TYPES_H */
			   
