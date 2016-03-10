/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      llwu.h
 * @version   1.0.3.0
 * @date      Mar-10-2013
 * @brief     Low-Leakage Wakeup Unit (LLWU) driver header file.
 ******************************************************************************/
#ifndef __LLWU_H
#define __LLWU_H 
 
/******************************************************************************
 * definition configuration structures                                        *
 ******************************************************************************/
typedef struct { uint8 PE1, PE2, PE3, PE4, ME, FILT1, FILT2; } tLLWU;

/******************************************************************************
* LLWU wakeup sources used by LLWU_GetFlags() and LLWU_ClrFlags() macros
*
*//*! @addtogroup llwu_sources
* @{
*******************************************************************************/
#define LLWU_WUF0     (uint32)(1 <<  0)     ///< LLWU_P0 external input pin
#define LLWU_WUF1     (uint32)(1 <<  1)     ///< LLWU_P1 external input pin
#define LLWU_WUF2     (uint32)(1 <<  2)     ///< LLWU_P2 external input pin 
#define LLWU_WUF3     (uint32)(1 <<  3)     ///< LLWU_P3 external input pin
#define LLWU_WUF4     (uint32)(1 <<  4)     ///< LLWU_P4 external input pin
#define LLWU_WUF5     (uint32)(1 <<  5)     ///< LLWU_P5 external input pin
#define LLWU_WUF6     (uint32)(1 <<  6)     ///< LLWU_P6 external input pin
#define LLWU_WUF7     (uint32)(1 <<  7)     ///< LLWU_P7 external input pin
#define LLWU_WUF8     (uint32)(1 <<  8)     ///< LLWU_P8 external input pin
#define LLWU_WUF9     (uint32)(1 <<  9)     ///< LLWU_P9 external input pin
#define LLWU_WUF10    (uint32)(1 << 10)     ///< LLWU_P10 external input pin
#define LLWU_WUF11    (uint32)(1 << 11)     ///< LLWU_P11 external input pin
#define LLWU_WUF12    (uint32)(1 << 12)     ///< LLWU_P12 external input pin
#define LLWU_WUF13    (uint32)(1 << 13)     ///< LLWU_P13 external input pin
#define LLWU_WUF14    (uint32)(1 << 14)     ///< LLWU_P14 external input pin
#define LLWU_WUF15    (uint32)(1 << 15)     ///< LLWU_P15 external input pin
#define LLWU_MWUF0    (uint32)(1 << 16)     ///< LPTMR0  
#define LLWU_MWUF1    (uint32)(1 << 17)     ///< iRTC Alarm 
#define LLWU_MWUF2    (uint32)(1 << 18)     ///< CMP0 
#define LLWU_MWUF3    (uint32)(1 << 19)     ///< iRTC Interrupt 
#define LLWU_MWUF4    (uint32)(1 << 20)     ///< CMP1 
#define LLWU_FILTF1   (uint32)(1 << 24)     ///< LLWU_FILT1
#define LLWU_FILTF2   (uint32)(1 << 25)     ///< LLWU_FILT2
#define LLWU_ALL      (uint32)(0xffffffff)  ///< All sources for use with @ref 
                                            ///  LLWU_ClrFlags() macro
/*! @} End of llwu_sources                                                    */

/******************************************************************************
* LLWU default configuration used by LLWU_init() function
*
*//*! @addtogroup llwu_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief All LLWU direct and filtered wakeup sources disabled.
 * @details The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer   
 *****************************************************************************/
#define LLWU_MODULE_DISABLE_ALL_CONFIG                                        \
(tLLWU){                                                                      \
/* PE1  */ SET(LLWU_PE1_WUPE0 (0))|SET(LLWU_PE1_WUPE1 (0))  |                 \
/* ...  */ SET(LLWU_PE1_WUPE2 (0))|SET(LLWU_PE1_WUPE3 (0)),                   \
/* PE2  */ SET(LLWU_PE2_WUPE4 (0))|SET(LLWU_PE2_WUPE5 (0))  |                 \
/* ...  */ SET(LLWU_PE2_WUPE6 (0))|SET(LLWU_PE2_WUPE7 (0)),                   \
/* PE3  */ SET(LLWU_PE3_WUPE8 (0))|SET(LLWU_PE3_WUPE9 (0))  |                 \
/* ...  */ SET(LLWU_PE3_WUPE10(0))|SET(LLWU_PE3_WUPE11(0)),                   \
/* PE4  */ SET(LLWU_PE4_WUPE12(0))|SET(LLWU_PE4_WUPE13(0))  |                 \
/* ...  */ SET(LLWU_PE4_WUPE14(0))|SET(LLWU_PE4_WUPE15(0)),                   \
/* ME   */ CLR(LLWU_ME_WUME0_MASK)|CLR(LLWU_ME_WUME1_MASK)  |                 \
/* ..   */ CLR(LLWU_ME_WUME2_MASK)|CLR(LLWU_ME_WUME3_MASK)  |                 \
/* ..   */ CLR(LLWU_ME_WUME4_MASK),                                           \
/* FILT1*/ SET(LLWU_FILT1_FILTE(0))|SET(LLWU_FILT1_FILTSEL(0)),               \
/* FILT1*/ SET(LLWU_FILT2_FILTE(0))|SET(LLWU_FILT2_FILTSEL(0)),               \
}
/*! @} End of llwu_config                                                     */

/******************************************************************************
* LLWU callback registered by LLWU_Init() function
*
*//*! @addtogroup llwu_callback
* @{
*******************************************************************************/
/*! @brief LLWU_CALLBACK function declaration.                           
    @details This callback is called by the LLWU module upon recovery from VLLS 
             modes.
             The service routine shall restore I/O and release latch states by 
             <c>PMC_SetACKISO()</c> macro.
    @param   flags  @ref llwu_sources
*/
typedef void (*LLWU_CALLBACK)(uint32 flags);
/*! @} End of llwu_callback                                                   */

/******************************************************************************
* LLWU function and macro definitions
*
*//*! @addtogroup llwu_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Returns flag indicating which internal wakeup source caused 
 *          the MCU to exit VLLS mode.
 * @details This macro returns flag indicating which internal wakeup source 
 *          caused the MCU to exit VLLS mode.
 * @return  @ref uint32 wakeup source @ref llwu_sources
 * @note    Implemented as an inline macro. Flags associated with on-chip 
 *          peripherals, external input pins, filtered and unfiltered, must be 
 *          cleared by software prior to entry to VLLSx mode.
 * @see     @ref LLWU_ClrFlags()
 ******************************************************************************/
#define LLWU_GetFlags()   (((uint32)(LLWU_FILT2&0x7f)<<18) |                  \
                           ((uint32)(LLWU_FILT1&0x7f)<<17) |                  \
                           ((uint32)LLWU_F3          <<16) |                  \
                           ((uint32)LLWU_F2          << 8) |                  \
                            (uint32)LLWU_F1)

/***************************************************************************//*!
 * @brief   Clears wakeup flag indicated by an input mask.
 * @details This macro clears wakeup flag indicated by an input mask.
 * @param   mask  Select one or more OR'ed @ref llwu_sources.  
 * @note    Implemented as an inline macro.
 * @see     @ref LLWU_GetFlags
 ******************************************************************************/
#define LLWU_ClrFlags(mask)   {                                               \
                                LLWU_F2     = (uint8)((mask>> 8)&0xff);       \
                                LLWU_F1     = (uint8)((mask>> 0)&0xff);       \
                                LLWU_FILT1 |= (uint8)((mask>>17)&0xff);       \
                                LLWU_FILT2 |= (uint8)((mask>>18)&0xff);       \
                              }
                              
/***************************************************************************//*!
 * @brief   LLWU initialization function.
 * @details This function initializes LLWU module. Wakeup sources shall be 
 *          configured by the user in configuration structure @ref llwu_config.
 *          Function also installs callback function for interrupt vector 28 
 *          (LLWU).
 * @param   cfg       Select one of the following @ref llwu_config
 * @param   ip        @ref cm0plus_prilvl
 * @param   callback  Pointer to @ref llwu_callback
 * @note    Implemented as a function call. 
 * @warning After enabling an external pin filter or changing the source pin, 
 * wait at least 5 LPO clock cycles before entering VLLSx mode to allow the 
 * filter to initialize.
 * @warning Register interrupt handler for LLWU module:
 *|Module|Appconfig.h                                                    |
 *|:----:|:--------------------------------------------------------------|
 *|LLWU  |<c>extern void llwu_isr(void);\n#define VECTOR_028 llwu_isr</c>|
 ******************************************************************************/ 
#define LLWU_Init(cfg,ip,callback)      LLWU_Init(cfg,ip,callback)
/*! @} End of llwu_macro                                                      */                              

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/ 
extern void LLWU_Init (tLLWU llwu, uint8 ip, LLWU_CALLBACK pCallback);

#endif /* __LLWU_H */ 