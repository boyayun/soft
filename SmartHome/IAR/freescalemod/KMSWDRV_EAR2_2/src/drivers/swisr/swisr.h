/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      swisr.h
 * @version   1.0.2.0
 * @date      May-14-2012
 * @brief     Software Interrupt (SWISR) driver source code.
 ******************************************************************************/
#ifndef __SWISR_H
#define __SWISR_H 

/******************************************************************************
* List of interrupt vectors redefined for use with SWISR driver.
*
*//*! @addtogroup swisr_vectors
* @{
*******************************************************************************/
#define SWISR_VECTOR_0  INT_FTFA      ///< SWISR driver handler 0 assigned to INT_FTFA
#define SWISR_VECTOR_1  INT_EWM       ///< SWISR driver handler 1 assigned to INT_EWM
#define SWISR_VECTOR_2	INT_Watchdog  ///< SWISR driver handler 2 assigned to INT_Watchdog
/*! @} End of swisr_vectors                                                   */

/******************************************************************************
 * exported definition prototypes                                             *
 ******************************************************************************/
extern uint16 swisrcnt[];

/******************************************************************************
* SWISR callback registered by SWISR_HandlerInit() function
*
*//*! @addtogroup swisr_callback
* @{
*******************************************************************************/
/*! @brief SWISR_CALLBACK function declaration                                */
typedef void (*SWISR_CALLBACK)(void);
/*! @} End of swisr_callback                                                  */
 
/******************************************************************************
* SWISR function and macro definitions
*
*//*! @addtogroup swisr_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Calls software interrupt every <c>count</c> period.
 * @details This macro calls software interrupt on given priority level every 
 *          <c>count</c> period. If macro is called from an interrupt routine 
 *          that executes on the higher interrupt level then the callback 
 *          function will execute afterwards.     
 * @param   hnd         0,1,2 (interrupt handler)
 * @param   count       Number of counts to skip 0..65535.
 * @note    Implemented as an inline macro.
 * @see     @ref SWISR_HandlerFreqCall
 ******************************************************************************/
#pragma diag_suppress=Pe186
#define SWISR_HandlerCall(hnd,count)    {                                     \
                                          if (swisrcnt[hnd]++ >= count)       \
                                          {                                   \
                                            swisrcnt[hnd]=0;                  \
                                            NVIC_CallIsr(SWISR_VECTOR_##hnd); \
                                          }                                   \
                                        }

/***************************************************************************//*!
 * @brief   Calls software interrupt at relative frequency defined by ratio 
 *          (<c>base_task_freq</c>/<c>new_task_freq</c>).
 * @details This macro calls software interrupt on given priority level at 
 *          relative frequency defined by ratio 
 *          (<c>base_task_freq</c>/<c>new_task_freq</c>). If macro is called 
 *          from an interrupt routine that executes on the higher interrupt level 
 *          then the callback function will execute afterwards.     
 * @param   hnd             0,1,2 (interrupt handler)
 * @param   base_task_freq  Frequency of base task calling this macro in Hz.
 * @param   new_task_freq   Frequency of new task called by the macro in Hz.
 * @note    Implemented as an inline macro.
 * @warning Frequency of base task must be integer multiply higher than frequency
 *          of new task being called.
 * @see     @ref SWISR_HandlerCall
 ******************************************************************************/
#pragma diag_suppress=Pe186
#define SWISR_HandlerFreqCall(hnd,base_task_freq,new_task_freq)               \
{                                                                             \
  if (++swisrcnt[hnd] >= (uint16)(base_task_freq/new_task_freq))              \
  {                                                                           \
    swisrcnt[hnd]=0;                                                          \
    NVIC_CallIsr(SWISR_VECTOR_##hnd);                                         \
  }                                                                           \
}

/***************************************************************************//*!
 * @brief   Installs callback function for @ref swisr_vectors. 
 * @details This function installs callback function for @ref swisr_vectors.
 *          Interrupts are invoked by the software using <c>SWISR_HandlerCall()</c> 
 *          and <c>SWISR_HandlerFreqCall()</c> macros.
 * @param   hnd       0,1,2 (interrupt handler)
 * @param   ip        @ref cm0plus_prilvl
 * @param   callback  Pointer to <c>SWISR_CALLBACK()</c>
 * @note    Implemented as a function call.
 * @warning The driver registers the following @ref swisr_vectors. User can 
 *          interrupt vector assignments if necessary. 
 *          Register interrupt handler for software handlers:
 *|Handler|Appconfig.h                                                        |
 *|:-----:|:------------------------------------------------------------------|
 *|0      |<c>extern void swisr0_isr(void);\n#define VECTOR_029 swisr0_isr</c>|
 *|1      |<c>extern void swisr1_isr(void);\n#define VECTOR_043 swisr1_isr</c>|
 *|2      |<c>extern void swisr2_isr(void);\n#define VECTOR_045 swisr2_isr</c>|
 ******************************************************************************/
#define SWISR_HandlerInit(hnd,ip,callback) SWISR##hnd##_HandlerInit(ip,callback)
/*! @} End of swisr_macro                                                     */
  
/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/    
extern void SWISR0_HandlerInit  (uint16 ip, SWISR_CALLBACK pCallback);
extern void SWISR1_HandlerInit  (uint16 ip, SWISR_CALLBACK pCallback);
extern void SWISR2_HandlerInit  (uint16 ip, SWISR_CALLBACK pCallback);

#endif /* __SWISR_H */ 