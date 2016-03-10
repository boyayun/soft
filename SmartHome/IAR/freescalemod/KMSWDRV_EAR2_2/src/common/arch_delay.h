/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      arch_delay.h
 * @version   1.0.1.0
 * @date      Aug-10-2012      
 * @brief     Header file.
 ******************************************************************************/
#ifndef __ARCH_DELAY_H
#define __ARCH_DELAY_H

/******************************************************************************
* Cortex-M0+ function and macro definitions
*
*//*! @addtogroup cm0plus_macros
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Function delays software execution by number of core clock cycles.
 * @details This function delays software execution by specified number of core 
 *          clock cycles. 
 * @param   ticks Number of core clock cycles (unsigned integer 0 to 4294967296)
 * @note    Implemented as a function call.
 ******************************************************************************/
extern void arch_delay (long ticks);
/*! @} End of cm0plus_macros                                                  */

#endif /* __ARCH_DELAY_H */
