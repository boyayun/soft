/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      rnga.h
 * @version   1.0.3.0
 * @date      Feb-24-2013
 * @brief     Random Number Generator Accelerator (RNGA) driver header file.
 ******************************************************************************/
#ifndef __RNGA_H
#define __RNGA_H 

/******************************************************************************
 * definition configuration structures                                        *
 ******************************************************************************/
typedef struct { uint32 CR; } tRNGA;

/******************************************************************************
* RNGA default configurations used by RNGA_init() function
*
*//*! @addtogroup rnga_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Normal mode configuration.
 * @details Initializes RNGA module with following characteristics:
 *          - Initiated random number generation. 
 *          - Normal mode (new random word is available every 256 system clock 
 *            cycles).
 *          - High Assurance mode (reading random word while not ready sets 
 *            security violation flag).
 * @details The initialization value of the configuration structure split by 
 *          peripheral registers is shown above. 
 * @showinitializer
 *****************************************************************************/
#define RNGA_MODULE_NORMAL_MODE_CONFIG                                        \
(tRNGA){                                                                      \
/* CR   */ CLR(RNG_CR_SLP_MASK)|SET(RNG_CR_CLRI_MASK)|CLR(RNG_CR_INTM_MASK)|  \
/* ..   */ SET(RNG_CR_INTM_MASK)|SET(RNG_CR_GO_MASK),                         \
}
/*! @} End of rnga_config                                                     */

/******************************************************************************
* RNGA callback registered by RNGA_Init() function. This callback is called by
* driver when RNGA Output Register has been read while empty.
*
*//*! @addtogroup rnga_callback
* @{
*******************************************************************************/
/*! @brief RNGA_ERROR_CALLBACK function declaration.                           
    @details This error callback is called by the driver when random number has been 
             read by the <c>RNGA_GetValue()</c> macro while new value not available. 
             A new random number values will be available within 256 system clock cycles.
*/
typedef void (*RNGA_ERROR_CALLBACK)(void);
/*! @} End of rnga_callback                                                   */

/******************************************************************************
* RNGA function and macro definitions
*
*//*! @addtogroup rnga_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Returns queue status.
 * @details This macro returns TRUE when a new random value is ready in the queue
 *          for reading. Otherwise macro returns FALSE.
 * @return  TRUE    new random value is ready in the queue and can be read\n
 * @return  FALSE   random not yet available\n
 * @note    Implemented as an inline macro.
 * @see     @ref RNGA_GetValue
 ******************************************************************************/
#define RNGA_QueueFull()          ((RNG_SR & RNG_SR_OREG_LVL_MASK)?TRUE:FALSE)

/***************************************************************************//*!
 * @brief   Returns a new random value.
 * @details This macro returns a new random value from RNGA Output Register.
 * @return  @ref uint32 random number. 
 * @note    Implemented as an inline macro.
 * @see     @ref RNGA_QueueFull 
 ******************************************************************************/
#define RNGA_GetValue()           RNG_OR

/***************************************************************************//*!
 * @brief   Returns lower 8-bits of the status register.
 * @details This macro returns lower 8-bits of the status register.
 * @return  @ref uint8 (refer to RNG_SR [7:0] description for more details). 
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define RNGA_GetStatus()          (uint8)RNG_SR

/***************************************************************************//*!
 * @brief   Waits until a new random is generated and then value is returned.
 * @details This macro waits until a new random number is generated. Afterwards 
 *          random value is copied to the memory pointed by ptr. 
 * @param   ptr       Pointer to 32-bit random value.
 * @note    Implemented as an inline macro.
 * @warning Calling this macro results in programme stall for up to 256 system 
 *          clock cycles.
 * @see     @ref RNGA_GetValue 
 ******************************************************************************/
#define RNGA_WaitAndGetValue(ptr)                                             \
                              {                                               \
                                while(!RNGA_QueueFull());                     \
                                *ptr=RNGA_GetValue();                         \
                              }

/***************************************************************************//*!
 * @brief   Inserts entropy into the RNGA.
 * @details This macro insert entropy into the RNGA. It allows an external user 
 *          to continually seed the RNGA with externally generated random data. 
 * @param   ptr       Pointer to memory location with eight 32-bit entropy values.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define RNGA_SetEntropy(ptr)  {                                               \
                                register uint32 *p = ptr;                     \
                                RNG_ER=*p++; RNG_ER=*p++; RNG_ER=*p++;        \
                                RNG_ER=*p++; RNG_ER=*p++; RNG_ER=*p++;        \
                                RNG_ER=*p++; RNG_ER=*p++;                     \
                              }   

/***************************************************************************//*!
 * @brief   Places RNGA into sleep mode.
 * @details This macro switches RNGA into sleep mode. If module is placed in 
 *          sleep mode the oscillators are disabled.
 * @note    Implemented as an inline macro.
 * @see     @ref RNGA_WakeUp
 ******************************************************************************/
#define RNGA_Sleep()          iosetw (RNG_CR,RNG_CR_SLP_MASK)

/***************************************************************************//*!
 * @brief   Wakes up RNGA from sleep mode.
 * @details This macro transitions RNGA from sleep mode to normal mode. The 
 *          oscillators will be enabled and a new random values will be generated.
 * @note    Implemented as an inline macro.
 * @see     @ref RNGA_Sleep
 ******************************************************************************/
#define RNGA_WakeUp()         ioclrw (RNG_CR,RNG_CR_SLP_MASK)

/***************************************************************************//*!
 * @brief   RNGA initialization.
 * @details This function initializes Random Number Generator Accelerator (RNGA).
 *          It also installs callback function for interrupt vector 34.
 * @param   cfg       Select one of the following @ref rnga_config
 * @param   ip        @ref cm0plus_prilvl
 * @param   callback  Pointer to @ref RNGA_ERROR_CALLBACK
 * @note    Implemented as a function call.
 * @warning Register interrupt handler for RNGA module:
 *|Module|Appconfig.h                                                    |
 *|:----:|:--------------------------------------------------------------|
 *|RNGA  |<c>extern void rnga_isr(void);\n#define VECTOR_034 rnga_isr</c>|
 ******************************************************************************/
#define RNGA_Init(cfg,ip,callback)    RNGA_Init(cfg,ip,callback)
/*! @} End of rnga_macro                                                      */
                                                                  
/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/ 
extern void RNGA_Init (tRNGA rnga, uint8 ip, RNGA_ERROR_CALLBACK pCallback);

#endif /* __RNGA_H */ 