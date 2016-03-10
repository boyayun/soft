/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      rcm.h
 * @version   1.0.1.0
 * @date      Dec-22-2012
 * @brief     Reset Control Module (RCM) driver header file.
 ******************************************************************************/
#ifndef __RCM_H
#define __RCM_H 

/******************************************************************************
 * definition configuration structures                                        *
 ******************************************************************************/
typedef struct { uint8 RPFC, RPFW; } tRCM;

/******************************************************************************
* RCM sources of the most recent reset. The reset sources are returned by 
* RTC_GetResetSrc macro.
*
*//*! @addtogroup rcm_rst_sources
* @{
*******************************************************************************/
#define RCM_SACKERR (uint16)(1<<13) ///< Reset has been caused by a failure of 
                                    ///  one or more peripherals to acknowledge 
                                    ///  within approximately one second to enter 
                                    ///  stop mode after an attempt to enter Stop 
                                    ///  mode.
#define RCM_MDM_AP  (uint16)(1<<11) ///< Reset has been caused by the host 
                                    ///  debugger system setting of the System 
                                    ///  Reset Request bit in the MDM-AP Control 
                                    ///  Register.
#define RCM_SW      (uint16)(1<<10) ///< Reset has been caused by software 
                                    ///  setting of SYSRESETREQ bit in 
                                    ///  Application Interrupt and Reset Control 
                                    ///  Register in the ARM core.
#define RCM_LOCKUP  (uint16)(1<< 9) ///< Reset has been caused by the ARM core 
                                    ///  indication of a LOCKUP event.
#define RCM_POR     (uint16)(1<< 7) ///< Reset has been caused by the power-on 
                                    ///  detection logic. Because the internal 
                                    ///  supply voltage was ramping up at the 
                                    ///  time, the low-voltage reset (LVD) status 
                                    ///  bit is also set to indicate that the 
                                    ///  reset occurred while the internal supply 
                                    ///  was below the LVD threshold.
#define RCM_PIN     (uint16)(1<< 6) ///< Reset has been caused by an active-low 
                                    ///  level on the external /RESET pin.
#define RCM_WDOG    (uint16)(1<< 5) ///< Reset has been caused by the watchdog 
                                    ///  Timer timing out. This reset source can 
                                    ///  be blocked by disabling the watchdog.
#define RCM_LOL     (uint16)(1<< 3) ///< Reset has been caused by a loss of lock 
                                    ///  in the MCG PLL.
#define RCM_LOC     (uint16)(1<< 2) ///< Reset has been caused by a loss of 
                                    ///  external clock. The MCG clock monitor 
                                    ///  must be enabled for a loss of clock to 
                                    ///  be detected.
#define RCM_LVD     (uint16)(1<< 1) ///< If the LVDRE bit is set and the supply 
                                    ///  drops below the LVD trip voltage, an LVD 
                                    ///  reset occurs. This bit is also set by POR.
#define RCM_WAKEUP  (uint16)(1<< 0) ///< Reset has been caused by an enabled 
                                    ///  wakeup source while the chip was in a low 
                                    ///  leakage mode. Any enabled wakeup source 
                                    ///  in a VLLSx mode causes a reset. This bit 
                                    ///  is cleared by any reset except WAKEUP.
/*! @} End of rcm_rst_sources                                                 */ 

/******************************************************************************
* RCM default configurations used by RCM_Init() macro.
*
*//*! @addtogroup rcm_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief Reset pin filter disabled in all modes (default).
 * @details Configuration structure disables reset pin filter in Run, Wait, Stop, 
 * VLPS and VLLSx modes (default). The initialization value of the configuration 
 * structure split by peripheral registers is shown above.
 * @details The initialization value of the configuration structure split by 
 * peripheral registers is shown above.
 * @showinitializer
 *****************************************************************************/
#define RCM_MODULE_FILT_DI_IN_ALL_MODES_CONFIG                                \
(tRCM){                                                                       \
/* RPFC   */ CLR(RCM_RPFC_RSTFLTSS_MASK)|SET(RCM_RPFC_RSTFLTSRW(0)),          \
/* RPFW   */ SET(RCM_RPFW_RSTFLTSEL(0))                                       \
}

/***************************************************************************//*!
 * @brief Reset pin filter enabled and clocked by LPO in all modes.
 * @details Configuration structure enables reset pin filter in Run, Wait Stop, 
 * VLPS and VLLSx modes (except VLLS0). Filter is clocked by LPO.
 * @details The initialization value of the configuration structure split by 
 * peripheral registers is shown above.
 * @warning LPO filter has a fixed filter value of 3. Due to a synchronizer on 
 * the input data, there is also some associated latency (2 cycles). As a result, 
 * 5 cycles are required to complete a transition from low to high or high to low.
 * @showinitializer
 *****************************************************************************/
#define RCM_MODULE_FILT_CLOCKED_BY_LPO_EN_IN_ALL_MODES_CONFIG                 \
(tRCM){                                                                       \
/* RPFC   */ SET(RCM_RPFC_RSTFLTSS_MASK)|SET(RCM_RPFC_RSTFLTSRW(2)),          \
/* RPFW   */ SET(RCM_RPFW_RSTFLTSEL(0))                                       \
}
/*! @} End of rcm_config                                                      */

/******************************************************************************
* RCM function and macro definitions
*
*//*! @addtogroup rcm_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Returns status flags indicating the source of reset.
 * @details This macro returns status flags indicating the source of the most 
 *          recent reset.
 * @return  @ref rcm_rst_sources
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define RCM_GetResetSrc() ((uint16)RCM_SRS0|(((uint16)RCM_SRS1)<<8)) 

/***************************************************************************//*!
 * @brief   RCM initialization function.
 * @details This function initializes reset pin filter.
 * @param   cfg   Select one of the following @ref rcm_config
 * @note    Implemented as an inline macro. 
 ******************************************************************************/ 
#define RCM_Init(cfg)     RCM_RPFC = cfg.RPFC; RCM_RPFW = cfg.RPFW
/*! @} End of rcm_macro                                                       */

#endif /* __RCM_H */ 