/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      pit.h
 * @version   1.0.3.0
 * @date      Feb-24-2013
 * @brief     Periodic Interrupt Timer (PIT) driver header file.
 ******************************************************************************/
#ifndef __PIT_H
#define __PIT_H 
 
/******************************************************************************
 * definition configuration structures                                        *
 ******************************************************************************/
typedef struct { uint32 TCTRL;  } tPIT_CH;

/******************************************************************************
* PIT Timer channel default configurations used by PIT_init() function
*
*//*! @addtogroup pit_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief Timer channel enabled and Timer runs after initialization.
 * Interrupt enabled on the peripheral level.
 *****************************************************************************/
#define PIT_CH_TIMER_EN_CONFIG                                                \
(tPIT_CH){                                                                    \
/* TCTRL  */  CLR(PIT_TCTRL_CHN_MASK)|SET(PIT_TCTRL_TIE_MASK)|                \
/* .....  */  SET(PIT_TCTRL_TEN_MASK),                                        \
}

/***************************************************************************//*!
 * @brief Timer channel enabled but Timer stops. Use <c>PIT_Enable()</c> macro 
 * to start Timer operation. Interrupt enabled on the peripheral level.
 *****************************************************************************/
#define PIT_CH_TIMER_DI_CONFIG                                                \
(tPIT_CH){                                                                    \
/* TCTRL  */  CLR(PIT_TCTRL_CHN_MASK)|SET(PIT_TCTRL_TIE_MASK)|                \
/* .....  */  CLR(PIT_TCTRL_TEN_MASK),                                        \
}
/*! @} End of pit_config                                                      */

/******************************************************************************
* PIT callback registered by PIT_InstallCallback() function
*
*//*! @addtogroup pit_callback
* @{
*******************************************************************************/
/*! @brief PIT_CALLBACK type declaration                                      */
typedef enum 
{ 
  PIT0CH0_CALLBACK=1, ///< PIT0_CH0 interrupt
  PIT0CH1_CALLBACK=2, ///< PIT0_CH1 interrupt
  PIT1CH0_CALLBACK=4, ///< PIT1_CH0 interrupt 
  PIT1CH1_CALLBACK=8  ///< PIT1_CH1 interrupt 
} PIT_CALLBACK_TYPE;

/*! @brief PIT_CALLBACK function declaration                                  */
typedef void (*PIT_CALLBACK)(PIT_CALLBACK_TYPE type);
/*! @} End of pit_callback                                                    */

/******************************************************************************
* PIT function and macro definitions
*
*//*! @addtogroup pit_macro
* @{
*******************************************************************************/
#define PIT0CH0_SetLoadVal(value)   PIT0_LDVAL0 = value
#define PIT0CH1_SetLoadVal(value)   PIT0_LDVAL1 = value
#define PIT1CH0_SetLoadVal(value)   PIT1_LDVAL0 = value
#define PIT1CH1_SetLoadVal(value)   PIT1_LDVAL1 = value
/***************************************************************************//*!
 * @brief   Sets load value register.
 * @details This macro sets new load register value.
 * @param   module    PIT0,PIT1
 * @param   ch        CH0,CH1
 * @param   value     @ref uint32 load register value 
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define PIT_SetLoadVal(module,ch,value) module##ch##_SetLoadVal(value)

#define PIT0CH0_Enable              iosetw (PIT0_TCTRL0,PIT_TCTRL_TEN_MASK)
#define PIT0CH1_Enable              iosetw (PIT0_TCTRL1,PIT_TCTRL_TEN_MASK)
#define PIT1CH0_Enable              iosetw (PIT1_TCTRL0,PIT_TCTRL_TEN_MASK)
#define PIT1CH1_Enable              iosetw (PIT1_TCTRL1,PIT_TCTRL_TEN_MASK)
/***************************************************************************//*!
 * @brief   Enables PIT channel.
 * @details This macro enables PIT channel operation.
 * @param   module    PIT0,PIT1
 * @param   ch        CH0,CH1
 * @note    Implemented as an inline macro.
 * @see     @ref PIT_Disable
 ******************************************************************************/
#define PIT_Enable(module,ch)           module##ch##_Enable

#define PIT0CH0_Disable             ioclrw (PIT0_TCTRL0,PIT_TCTRL_TEN_MASK)
#define PIT0CH1_Disable             ioclrw (PIT0_TCTRL1,PIT_TCTRL_TEN_MASK)
#define PIT1CH0_Disable             ioclrw (PIT1_TCTRL0,PIT_TCTRL_TEN_MASK)
#define PIT1CH1_Disable             ioclrw (PIT1_TCTRL1,PIT_TCTRL_TEN_MASK)
/***************************************************************************//*!
 * @brief   Disables PIT channel.
 * @details This macro disables PIT channel operation.
 * @param   module    PIT0,PIT1
 * @param   ch        CH0,CH1
 * @note    Implemented as an inline macro.
 * @see     @ref PIT_Enable
 ******************************************************************************/
#define PIT_Disable(module,ch)          module##ch##_Disable

/***************************************************************************//*!
 * @brief   Controls PIT module operation in debug mode.
 * @details This macro controls operation mode of the PIT module in the debug
 *          mode.
 * @param   module    PIT0,PIT1
 * @param   ctrl      TRUE  (Timer module runs in Debug mode)\n
 *                    FALSE (Timer module stopped in Debug mode)\n
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define PIT_DebugFreezeOn(module,ctrl)                                        \
{                                                                             \
  if (ctrl == TRUE) iosetw (module##_MCR,PIT_MCR_FRZ_MASK);                   \
  else              ioclrw (module##_MCR,PIT_MCR_FRZ_MASK);                   \
}    

/***************************************************************************//*!
 * @brief   Installs callback function for interrupt vector 27.
 * @details This function install callback function for interrupt vector 27.
 * @param   ip        @ref cm0plus_prilvl
 * @param   callback  Pointer to @ref PIT_CALLBACK
 * @note    Implemented as a function call.
 * @warning PIT0 and PIT1 interrupt sources are OR'ed into single interrupt vector.
 *          Register interrupt handler for PIT modules:
 *|Module|Appconfig.h                                                              |
 *|:----:|:------------------------------------------------------------------------|
 *|PITx  |<c>extern void PIT0_PIT1_isr(void);\n#define VECTOR_027 PIT0_PIT1_isr</c>|
 * @see     @ref PIT_Init
 ******************************************************************************/
#define PIT_InstallCallback(ip,callback) PIT_InstallCallback (ip,callback)

/***************************************************************************//*!
 * @brief   PIT initialization function.
 * @details This function initializes selected Timer channel of the 
 *          Periodic Interrupt Timer (PIT) block.  
 * @param   module    PIT0,PIT1
 * @param   ch        CH0,CH1
 * @param   cfg       Select one of the following @ref pit_config
 * @param   value     @ref uint32 load register value  e
 * @note    Implemented as a function call.
 * @see     @ref PIT_InstallCallback
 ******************************************************************************/
#define PIT_Init(module,ch,cfg,value)       module##_##ch##_Init(cfg,value)
/*! @} End of pit_macro                                                       */
  
/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/    
extern void PIT_InstallCallback (uint8 ip, PIT_CALLBACK pCallback);
extern void PIT0_CH0_Init  (tPIT_CH ch, uint32 value);
extern void PIT0_CH1_Init  (tPIT_CH ch, uint32 value);
extern void PIT1_CH0_Init  (tPIT_CH ch, uint32 value);
extern void PIT1_CH1_Init  (tPIT_CH ch, uint32 value);

#endif /* __PIT_H */ 