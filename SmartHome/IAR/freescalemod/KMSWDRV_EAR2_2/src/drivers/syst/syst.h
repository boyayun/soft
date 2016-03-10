/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      syst.h
 * @version   1.0.1.0
 * @date      May-14-2012
 * @brief     System Timer (SYST) driver header file.
 ******************************************************************************/
#ifndef __SYST_H
#define __SYST_H 
 
/******************************************************************************
 * definition configuration structures                                        *
 ******************************************************************************/
typedef struct { uint32 CSR, RVR; } tSYST;

/******************************************************************************
* SYSTICK default configuration used by SYST_Init() function
*
*//*! @addtogroup syst_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief Disables system Timer operation (default).
 *****************************************************************************/
#define SYST_MODULE_TIMER_DI_CONFIG                                           \
(tSYST){                                                                      \
/* CSR  */ CLR(SysTick_CSR_ENABLE_MASK)|CLR(SysTick_CSR_TICKINT_MASK)|        \
/* ...  */ SET(SysTick_CSR_CLKSOURCE_MASK),                                   \
/* RVR  */ SET(SysTick_RVR_RELOAD(0x00FFFFFF)),                               \
}

/***************************************************************************//*!
 * @brief Enables system Timer operation. The system Timer operates in a freerun
 *        24-bit Timer mode. Timer is clocked by the system clock. 
 *****************************************************************************/
#define SYST_MODULE_FREERUN_TIMER_EN_CONFIG                                   \
(tSYST){                                                                      \
/* CSR  */ SET(SysTick_CSR_ENABLE_MASK)|CLR(SysTick_CSR_TICKINT_MASK)|        \
/* ...  */ SET(SysTick_CSR_CLKSOURCE_MASK),                                   \
/* RVR  */ SET(SysTick_RVR_RELOAD(0x00FFFFFF)),                               \
}

/***************************************************************************//*!
 * @brief Enables system Timer operation. The system Timer operates in a reload 
 *        mode. Timer is clocked by the system clock.
 * @param reload  The reload value can be any value in the range 0x00000001 to 
 *                0x00FFFFFF (24-bit). 
 *****************************************************************************/
#define SYST_MODULE_RELOAD_TIMER_EN_CONFIG(reload)                            \
(tSYST){                                                                      \
/* CSR  */ SET(SysTick_CSR_ENABLE_MASK)|CLR(SysTick_CSR_TICKINT_MASK)|        \
/* ...  */ SET(SysTick_CSR_CLKSOURCE_MASK),                                   \
/* RVR  */ SET(SysTick_RVR_RELOAD((reload-1)&0x00FFFFFF)),                    \
}
/*! @} End of syst_config                                                     */

/******************************************************************************
* SYST callback registered by SYST_Init() function
*
*//*! @addtogroup syst_callback
* @{
*******************************************************************************/
/*! @brief SYST_CALLBACK function declaration.                           
    @details This callback is called by the System Timer module counts down to zero.             
*/
typedef void (*SYST_CALLBACK)(void);
/*! @} End of syst_callback                                                   */

/******************************************************************************
* SYSTICK function and macro definitions
*
*//*! @addtogroup syst_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Returns inverse value of the 24-bit System Timer.
 * @details This macro returns inverse value of the 24-bit System Timer. The 
 *          value equals to number of system clock ticks elapsed since Timer
 *          operation resumes after clearing.
 * @return  @ref uint32 24-bit Timer current value
 * @note    Implemented as an inline macro.
 * @see     @ref SYST_ClrTimer 
 ******************************************************************************/
#define SYST_GetTimer()     (uint32)~(SYST_CVR|0xff000000)

/***************************************************************************//*!
 * @brief   Clears 24-bit System Timer current value.
 * @details This macro clears 24-bit System Timer current value.
 * @note    Implemented as an inline macro.
 * @see     @ref SYST_GetTimer 
 ******************************************************************************/
#define SYST_ClrTimer()     SYST_CVR=0l;
                              
/***************************************************************************//*!
 * @brief   System Timer initialization.
 * @details This function initializes System Timer module. Function also installs 
 *          callback function for interrupt vector 15 (SysTick).
 * @param   cfg       Select one of @ref syst_config
 * @param   ip        @ref cm0plus_prilvl
 * @param   callback  Pointer to @ref syst_callback
 * @note    Implemented as a function call.
 * @warning Register interrupt handler for SYST module.
 *|Module|Appconfig.h                                                    |
 *|:----:|:--------------------------------------------------------------|
 *|SYST  |<c>extern void syst_isr(void);\n#define VECTOR_015 syst_isr</c>|
 ******************************************************************************/ 
#define SYST_Init(cfg,ip,callback)   SYST_Init(cfg,ip,callback)
/*! @} End of syst_macro                                                      */                              

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/ 
extern void SYST_Init (tSYST syst, uint8 ip, SYST_CALLBACK pCallback);

#endif /* __SYST_H */ 