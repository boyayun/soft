/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      pmc.h
 * @version   1.0.3.0
 * @date      Feb-24-2013
 * @brief     Power Management Controller (PMC) driver header file.
 ******************************************************************************/
#ifndef __PMC_H
#define __PMC_H 
 
/******************************************************************************
 * definition configuration structures                                        *
 ******************************************************************************/
typedef struct { uint8 LVDSC1, LVDSC2; } tLVD;
typedef struct { uint8 REGSC; } tREG;

/***************************************************************************//*!
 *//*! @addtogroup pmc_lvdv
 * @{
 * @details Supported Low-Voltage Detect Voltages
 ******************************************************************************/
#define PMC_LVDL (uint8)0 ///< Falling low-voltage detect low range (1.6 V).
#define PMC_LVDH (uint8)1 ///< Falling low-voltage detect high range (2.56 V).
/*! @} End of pmc_lvdv                                                        */

/***************************************************************************//*!
 *//*! @addtogroup pmc_lvwv
 * @{
 * @details Supported Low-Voltage Warning Voltages
 ******************************************************************************/
#define PMC_LVW1 (uint8)0 ///< Low trip point is 2.7 V if PMC_LVDH (low-voltage 
                          ///  detect high range) selected otherwise it is 1.8 V. 
#define PMC_LVW2 (uint8)1 ///< Mid1 trip point is 2.8 V if PMC_LVDH (low-voltage 
                          ///  detect high range) selected otherwise it is 1.9 V.
#define PMC_LVW3 (uint8)2 ///< Mid2 trip point is 2.9 V if PMC_LVDH (low-voltage 
                          ///  detect high range) selected otherwise it is 2.0 V.
#define PMC_LVW4 (uint8)3 ///< High trip point is 3.0 V if PMC_LVDH (low-voltage 
                          ///  detect high range) selected otherwise it is 2.1V.
/*! @} End of pmc_lvwv                                                        */

/******************************************************************************
* PMC configuration used by PMC_init() function
*
*//*! @addtogroup pmc_module_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @details Sets PMC block as follows:
 *  - Low-Voltage Detect Interrupt Disabled (use polling).
 *  - Low-Voltage Detect Reset Disabled.
 *  - Low-Voltage Detect Voltage Selected by <c>lvdv</c> input argument.
 *  - Low-Voltage Warning Interrupt Disabled (use polling).
 *  - Low-Voltage Warning Voltage Selected by <c>lvwv</c> input argument.
 * @param   lvdv  @ref pmc_lvdv Threshold Select.
 * @param   lvwv  @ref pmc_lvwv Threshold Select. 
 *****************************************************************************/
#define PMC_MODULE_LVDRE_OFF_LVDINT_OFF_LVWINT_OFF_CONFIG(lvdv,lvwv)          \
(tLVD){                                                                       \
/* LVDSC1 */  SET(PMC_LVDSC1_LVDACK_MASK)|CLR(PMC_LVDSC1_LVDIE_MASK)|         \
/* ...... */  CLR(PMC_LVDSC1_LVDRE_MASK)|SET(PMC_LVDSC1_LVDV(lvdv)),          \
/* LVDSC2 */  SET(PMC_LVDSC2_LVWACK_MASK)|CLR(PMC_LVDSC2_LVWIE_MASK)|         \
/* ...... */  SET(PMC_LVDSC2_LVWV(lvwv))                                      \
}

/***************************************************************************//*!
 * @details Sets PMC block as follows:
 *  - Low-Voltage Detect Interrupt Disabled (use polling).
 *  - Low-Voltage Detect Reset Disabled.
 *  - Low-Voltage Detect Voltage Selected by <c>lvdv</c> input argument.
 *  - Low-Voltage Warning Interrupt Enabled.
 *  - Low-Voltage Warning Voltage Selected by <c>lvwv</c> input argument.
 * @param   lvdv  @ref pmc_lvdv Threshold Select.
 * @param   lvwv  @ref pmc_lvwv Threshold Select. 
 *****************************************************************************/
#define PMC_MODULE_LVDRE_OFF_LVDINT_OFF_LVWINT_ON_CONFIG(lvdv,lvwv)           \
(tLVD){                                                                       \
/* LVDSC1 */  SET(PMC_LVDSC1_LVDACK_MASK)|CLR(PMC_LVDSC1_LVDIE_MASK)|         \
/* ...... */  CLR(PMC_LVDSC1_LVDRE_MASK)|SET(PMC_LVDSC1_LVDV(lvdv)),          \
/* LVDSC2 */  SET(PMC_LVDSC2_LVWACK_MASK)|SET(PMC_LVDSC2_LVWIE_MASK)|         \
/* ...... */  SET(PMC_LVDSC2_LVWV(lvwv))                                      \
}

/***************************************************************************//*!
 * @details Sets PMC block as follows:
 *  - Low-Voltage Detect Interrupt Enabled.
 *  - Low-Voltage Detect Reset Disabled.
 *  - Low-Voltage Detect Voltage Selected by <c>lvdv</c> input argument.
 *  - Low-Voltage Warning Interrupt Disabled (use polling).
 *  - Low-Voltage Warning Voltage Selected by <c>lvwv</c> input argument.
 * @param   lvdv  @ref pmc_lvdv Threshold Select.
 * @param   lvwv  @ref pmc_lvwv Threshold Select. 
 *****************************************************************************/
#define PMC_MODULE_LVDRE_OFF_LVDINT_ON_LVWINT_OFF_CONFIG(lvdv,lvwv)           \
(tLVD){                                                                       \
/* LVDSC1 */  SET(PMC_LVDSC1_LVDACK_MASK)|SET(PMC_LVDSC1_LVDIE_MASK)|         \
/* ...... */  CLR(PMC_LVDSC1_LVDRE_MASK)|SET(PMC_LVDSC1_LVDV(lvdv)),          \
/* LVDSC2 */  SET(PMC_LVDSC2_LVWACK_MASK)|CLR(PMC_LVDSC2_LVWIE_MASK)|         \
/* ...... */  SET(PMC_LVDSC2_LVWV(lvwv))                                      \
}

/***************************************************************************//*!
 * @details Sets PMC block as follows:
 *  - Low-Voltage Detect Interrupt Enabled.
 *  - Low-Voltage Detect Reset Disabled.
 *  - Low-Voltage Detect Voltage Selected by <c>lvdv</c> input argument.
 *  - Low-Voltage Warning Interrupt Enabled.
 *  - Low-Voltage Warning Voltage Selected by <c>lvwv</c> input argument.
 * @param   lvdv  @ref pmc_lvdv Threshold Select.
 * @param   lvwv  @ref pmc_lvwv Threshold Select. 
 *****************************************************************************/
#define PMC_MODULE_LVDRE_OFF_LVDINT_ON_LVWINT_ON_CONFIG(lvdv,lvwv)            \
(tLVD){                                                                       \
/* LVDSC1 */  SET(PMC_LVDSC1_LVDACK_MASK)|SET(PMC_LVDSC1_LVDIE_MASK)|         \
/* ...... */  CLR(PMC_LVDSC1_LVDRE_MASK)|SET(PMC_LVDSC1_LVDV(lvdv)),          \
/* LVDSC2 */  SET(PMC_LVDSC2_LVWACK_MASK)|SET(PMC_LVDSC2_LVWIE_MASK)|         \
/* ...... */  SET(PMC_LVDSC2_LVWV(lvwv))                                      \
}

/***************************************************************************//*!
 * @details Sets PMC block as follows:
 *  - Low-Voltage Detect Interrupt Disabled (use polling).
 *  - Low-Voltage Detect Reset Enabled.
 *  - Low-Voltage Detect Voltage Selected by <c>lvdv</c> input argument.
 *  - Low-Voltage Warning Interrupt Disabled (use polling).
 *  - Low-Voltage Warning Voltage Selected by <c>lvwv</c> input argument.
 * @param   lvdv  @ref pmc_lvdv Threshold Select.
 * @param   lvwv  @ref pmc_lvwv Threshold Select. 
 *****************************************************************************/
#define PMC_MODULE_LVDRE_ON_LVDINT_OFF_LVWINT_OFF_CONFIG(lvdv,lvwv)           \
(tLVD){                                                                       \
/* LVDSC1 */  SET(PMC_LVDSC1_LVDACK_MASK)|CLR(PMC_LVDSC1_LVDIE_MASK)|         \
/* ...... */  SET(PMC_LVDSC1_LVDRE_MASK)|SET(PMC_LVDSC1_LVDV(lvdv)),          \
/* LVDSC2 */  SET(PMC_LVDSC2_LVWACK_MASK)|CLR(PMC_LVDSC2_LVWIE_MASK)|         \
/* ...... */  SET(PMC_LVDSC2_LVWV(lvwv))                                      \
}

/***************************************************************************//*!
 * @details Sets PMC block as follows:
 *  - Low-Voltage Detect Interrupt Disabled (use polling).
 *  - Low-Voltage Detect Reset Enabled.
 *  - Low-Voltage Detect Voltage Selected by <c>lvdv</c> input argument.
 *  - Low-Voltage Warning Interrupt Enabled.
 *  - Low-Voltage Warning Voltage Selected by <c>lvwv</c> input argument.
 * @param   lvdv  @ref pmc_lvdv Threshold Select.
 * @param   lvwv  @ref pmc_lvwv Threshold Select. 
 *****************************************************************************/
#define PMC_MODULE_LVDRE_ON_LVDINT_OFF_LVWINT_ON_CONFIG(lvdv,lvwv)            \
(tLVD){                                                                       \
/* LVDSC1 */  SET(PMC_LVDSC1_LVDACK_MASK)|CLR(PMC_LVDSC1_LVDIE_MASK)|         \
/* ...... */  SET(PMC_LVDSC1_LVDRE_MASK)|SET(PMC_LVDSC1_LVDV(lvdv)),          \
/* LVDSC2 */  SET(PMC_LVDSC2_LVWACK_MASK)|SET(PMC_LVDSC2_LVWIE_MASK)|         \
/* ...... */  SET(PMC_LVDSC2_LVWV(lvwv))                                      \
}
/*! @} End of pmc_module_config                                               */

/******************************************************************************
* PMC's internal regulator configuration used by PMC_init() function
*
*//*! @addtogroup pmc_intreg_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @details Configures internal regulator of the PMC block as follows:
 *  - Bandgap voltage reference is disabled in VLPx and VLLSx modes
 *  - Bandgap buffer disabled.
 *  - ACKISO flag cleared (use <c>PMC_SetACKISO()</c> macro to release I/O 
 *    held state)
 *****************************************************************************/
#define PMC_INTREG_BGEN_OFF_BGBE_OFF_CONFIG                                   \
(tREG){                                                                       \
/* REGSC  */  CLR(PMC_REGSC_BGEN_MASK)|CLR(PMC_REGSC_ACKISO_MASK)|            \
/* .....  */  CLR(PMC_REGSC_BGBDS_MASK)|CLR(PMC_REGSC_BGBE_MASK)              \
}

/***************************************************************************//*!
 * @details Configures internal regulator of the PMC block as follows:
 *  - Bandgap voltage reference is disabled in VLPx and VLLSx modes
 *  - Bandgap buffer enabled and set to low drive mode.
 *  - ACKISO flag cleared (use <c>PMC_SetACKISO()</c> macro to release I/O 
 *    held state)
 *****************************************************************************/
#define PMC_INTREG_BGEN_OFF_BGBE_LDON_CONFIG                                  \
(tREG){                                                                       \
/* REGSC  */  CLR(PMC_REGSC_BGEN_MASK)|CLR(PMC_REGSC_ACKISO_MASK)|            \
/* .....  */  CLR(PMC_REGSC_BGBDS_MASK)|SET(PMC_REGSC_BGBE_MASK)              \
}

/***************************************************************************//*!
 * @details Configures internal regulator of the PMC block as follows:
 *  - Bandgap voltage reference is disabled in VLPx and VLLSx modes
 *  - Bandgap buffer enabled and set to high drive mode.
 *  - ACKISO flag cleared (use <c>PMC_SetACKISO()</c> macro to release I/O 
 *    held state)
 *****************************************************************************/
#define PMC_INTREG_BGEN_OFF_BGBE_HDON_CONFIG                                  \
(tREG){                                                                       \
/* REGSC  */  CLR(PMC_REGSC_BGEN_MASK)|CLR(PMC_REGSC_ACKISO_MASK)|            \
/* .....  */  SET(PMC_REGSC_BGBDS_MASK)|SET(PMC_REGSC_BGBE_MASK)              \
}

/***************************************************************************//*!
 * @details Configures internal regulator of the PMC block as follows:
 *  - Bandgap voltage reference is enabled in VLPx and VLLSx modes
 *  - Bandgap buffer enabled and set to low drive mode.
 *  - ACKISO flag cleared (use <c>PMC_SetACKISO()</c> macro to release I/O 
 *    held state)
 *****************************************************************************/
#define PMC_INTREG_BGEN_ON_BGBE_LDON_CONFIG                                   \
(tREG){                                                                       \
/* REGSC  */  SET(PMC_REGSC_BGEN_MASK)|CLR(PMC_REGSC_ACKISO_MASK)|            \
/* .....  */  CLR(PMC_REGSC_BGBDS_MASK)|SET(PMC_REGSC_BGBE_MASK)              \
}

/***************************************************************************//*!
 * @details Configures internal regulator of the PMC block as follows:
 *  - Bandgap voltage reference is enabled in VLPx and VLLSx modes
 *  - Bandgap buffer enabled and set to high drive mode.
 *  - ACKISO flag cleared (use <c>PMC_SetACKISO()</c> macro to release I/O 
 *    held state)
 *****************************************************************************/
#define PMC_INTREG_BGEN_ON_BGBE_HDON_CONFIG                                   \
(tREG){                                                                       \
/* REGSC  */  SET(PMC_REGSC_BGEN_MASK)|CLR(PMC_REGSC_ACKISO_MASK)|            \
/* .....  */  SET(PMC_REGSC_BGBDS_MASK)|SET(PMC_REGSC_BGBE_MASK)              \
}
/*! @} End of pmc_intreg_config                                               */

/******************************************************************************
* PMC callback registered by PMC_Init() function
*
*//*! @addtogroup pmc_callback
* @{
*******************************************************************************/
/*! @brief PMC_CALLBACK type declaration                                      */
typedef enum 
{
  LVD_CALLBACK = 1,       ///< LVD interrupt
  LVW_CALLBACK = 2        ///< LVW interrupt
} PMC_CALLBACK_TYPE;

/*! @brief PMC_CALLBACK function declaration                                  */
typedef void (*PMC_CALLBACK)(PMC_CALLBACK_TYPE type);
/*! @} End of pmc_callback                                                    */

/******************************************************************************
* PMC function and macro definitions
*
*//*! @addtogroup pmc_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Returns LVDF status.
 * @details This macro returns LVDF status.
 * @return  TRUE  internal supply voltage falls below the selected internal 
 *                monitor trip point (VLVD)\n
 * @return  FALSE normal operation\n
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define PMC_GetLVDF()       ((PMC_LVDSC1 & PMC_LVDSC1_LVDF_MASK )?TRUE:FALSE)

/***************************************************************************//*!
 * @brief   Returns LVWF status.
 * @details This macro returns LVWF status.
 * @return  TRUE  internal supply voltage falls below the selected internal 
 *                monitor trip point (VLVW)\n
 * @return  FALSE normal operation\n
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define PMC_GetLVWF()       ((PMC_LVDSC2 & PMC_LVDSC2_LVWF_MASK )?TRUE:FALSE)

/***************************************************************************//*!
 * @brief   Returns regulator status.
 * @details This macro returns regulator status (PMC_REGSC_REGSON).
 * @return  TRUE  regulator is in stop regulation or in transition to/from it\n
 * @return  FALSE regulator is in run regulation\n
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define PMC_GetREGONS()     ((PMC_REGSC  & PMC_REGSC_REGONS_MASK)?TRUE:FALSE)

/***************************************************************************//*!
 * @brief   Returns state indicating whether certain peripherals and the 
 *          I/O pads are in a latched state.
 * @details This macro returns state indicating whether certain peripherals and 
 *          the I/O pads are in a latched state as a result of having been in a 
 *          VLLS mode. Writing one to this bit when it is set releases the 
 *          I/O pads and certain peripherals to their normal run mode state.
 * @return  TRUE  peripherals and I/O pads are in an isolated and latched state\n
 * @return  FALSE peripherals and I/O pads are in normal run state\n
 * @note    Implemented as an inline macro.
 * @see     @ref PMC_SetACKISO 
 ******************************************************************************/
#define PMC_GetACKISO()      ((PMC_REGSC & PMC_REGSC_ACKISO_MASK)?TRUE:FALSE)

/***************************************************************************//*!
 * @brief   Releases I/O held state.
 * @details This macro releases I/O held state (the I/O states are held on a wakeup 
 *          event until the wakeup has been acknowledged via a write to ACKISO bit).
 * @note    Implemented as an inline macro.
 * @see     @ref PMC_GetACKISO 
 ******************************************************************************/
#define PMC_SetACKISO()       iosetb (PMC_REGSC,PMC_REGSC_ACKISO_MASK)

/***************************************************************************//*!
 * @brief   Enables bangap reference in VLPx and VLLSx modes.
 * @details This macro enables bangap reference in VLPx and VLLSx modes.
 * @note    Implemented as an inline macro.
 * @see     @ref PMC_ClrBGEN 
 ******************************************************************************/
#define PMC_SetBGEN()         iosetb (PMC_REGSC,PMC_REGSC_BGEN_MASK)

/***************************************************************************//*!
 * @brief   Disables bangap reference in VLPx and VLLSx modes.
 * @details This macro disables bangap reference in VLPx and VLLSx modes.
 * @note    Implemented as an inline macro.
 * @see     @ref PMC_SetBGEN 
 ******************************************************************************/
#define PMC_ClrBGEN()         ioclrb (PMC_REGSC,PMC_REGSC_BGEN_MASK)

/***************************************************************************//*!
 * @brief   Enables bangap buffer.
 * @details This macro enables bangap buffer.
 * @note    Implemented as an inline macro.
 * @warning Enable bangap buffer if you want to measure bandgap voltage by ADC 
 *          or using 1.0V Internal bandgap as the ADC reference.
 * @see     @ref PMC_CtrlBGBDS
 ******************************************************************************/
#define PMC_SetBGBE()         iosetb (PMC_REGSC,PMC_REGSC_BGBE_MASK)

/***************************************************************************//*!
 * @brief   Disables bandgap buffer.
 * @details This macro disables bandgap buffer.
 * @note    Implemented as an inline macro. This function also switches bandgap 
 *          buffer to low drive mode. 
 ******************************************************************************/
#define PMC_ClrBGBE()         ioclrb (PMC_REGSC,PMC_REGSC_BGBE_MASK);         \
                              ioclrb (PMC_REGSC,PMC_REGSC_BGBDS_MASK)

/***************************************************************************//*!
 * @brief   Selects the bandgap buffer low/high drive mode. 
 * @details This macro Selects the bandgap buffer to low/high drive mode.
 * @param   x   TRUE  (HIGH drive)\n
 *              FALSE (LOW drive-default)\n
 * @note    Implemented as an inline macro.
 * @see     @ref PMC_SetBGBE
 ******************************************************************************/
#define PMC_CtrlBGBDS(x)      iobfib (PMC_REGSC,PMC_REGSC_BGBDS_SHIFT,1,x)

/***************************************************************************//*!
 * @brief   PMC initialization.
 * @details This function initializes Power Management Controller (PMC) block.
 *          Function installs callback function for interrupt vector 22 (PMC).
 * @param   cfg1      Select one of the following @ref pmc_module_config
 * @param   cfg2      Select one of the following @ref pmc_intreg_config
 * @param   ip        @ref cm0plus_prilvl
 * @param   callback  Pointer to @ref PMC_CALLBACK
 * @note    Implemented as a function call.
 * @warning Register interrupt handler for PMC module:
 *|Module|Appconfig.h                                                   |
 *|:----:|:-------------------------------------------------------------|
 *|PMC   |<c>extern void pmc_isr(void);\n#define VECTOR_022 pmc_isr</c> |
 ******************************************************************************/
#define PMC_Init(cfg1,cfg2,ip,callback)   PMC_Init(cfg1,cfg2,ip,callback)
/*! @} End of pmc_macro                                                       */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/ 
extern void PMC_Init (tLVD lvd, tREG reg, uint8 ip, PMC_CALLBACK pCallback);

#endif /* __PMC_H */