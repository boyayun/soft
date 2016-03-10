/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      common.h 
 * @version   1.0.1.0
 * @date      Aug-10-2012   
 * @brief     Header file.
 ******************************************************************************/
#ifndef __COMMON_H
#define __COMMON_H

/******************************************************************************
* List of the basic data types
*
*//*! @addtogroup basic_data_types
* @{
 ******************************************************************************/
typedef unsigned char       uint8;    ///< unsigned  8 bits
typedef unsigned short int  uint16;   ///< unsigned 16 bits
typedef unsigned long int   uint32;   ///< unsigned 32 bits
typedef unsigned long long  uint64;   ///< unsigned 64 bits

typedef signed char         int8;     ///< signed  8 bits
typedef short int           int16;    ///< signed 16 bits
typedef int                 int32;    ///< signed 32 bits
typedef long long           int64;    ///< signed 64 bits

typedef volatile int8       vint8;    ///< volatile signed  8 bits
typedef volatile int16      vint16;   ///< volatile signed 16 bits
typedef volatile int32      vint32;   ///< volatile signed 32 bits
typedef volatile int64      vint64;   ///< volatile signed 64 bits

typedef volatile uint8      vuint8;   ///< volatile unsigned  8 bits
typedef volatile uint16     vuint16;  ///< volatile unsigned 16 bits
typedef volatile uint32     vuint32;  ///< volatile unsigned 32 bits
typedef volatile uint64     vuint64;  ///< volatile unsigned 64 bits
/*! @} End of basic_data_types                                                */

/******************************************************************************
 * Common macros and defines                                                  *
 ******************************************************************************/
#ifndef NULL
  #define NULL    (void*)0
#endif
#define TRUE    1
#define FALSE   0
/******************************************************************************
* List of the basic configuration structure macros
*
*//*! @addtogroup config_struct_macros
* @{
 ******************************************************************************/
/***************************************************************************//*!
 * @brief   Sets register field in peripheral configuration structure.
 * @details This macro sets register field <c>mask</c> in the peripheral 
 *          configuration structure. 
 * @param   mask    register field to be set.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define SET(mask)   (mask)

/***************************************************************************//*!
 * @brief   Clears register field in peripheral configuration structure.
 * @details This macro clears register field <c>mask</c> in the peripheral 
 *          configuration structure.
 * @param   mask     register field to be cleared. 
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define CLR(mask)   0
/*! @} End of config_struct_macros                                            */

/******************************************************************************
 * Include common header file                                                 *
 ******************************************************************************/
#include "appconfig.h"
#if defined(MCU_MKM34Z50) 
  #if defined (__ICCARM__)  /* IAR                                            */
    #include "headers/MKM34Z5.h"
  #elif defined(__GNUC__)   /* CW GCC                                         */
    #include "headers/MKM34Z5.h"
  #else
    #error "Toolchain not supported."
  #endif  
#else  
  #error "No valid CPU defined."
#endif

#include "iohw.h"
#include "startup.h"
#include "arm_cm0.h"
#include "arch_delay.h"

#endif /* __COMMON_H */