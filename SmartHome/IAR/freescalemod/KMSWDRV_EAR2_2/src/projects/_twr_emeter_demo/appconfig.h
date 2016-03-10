/*****************************************************************************
 * (c) Copyright 2010, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      appconfig.h
 * @version   1.0.4.0
 * @date      May-14-2012
 * @brief     Application configuration and interrupt vector definitions.
 ******************************************************************************/
#ifndef __APPCONFIG_H
#define __APPCONFIG_H

/***************************************************************************//*!
 * @brief   Define processor device.
 * @param   Supported devices - MCU_MKM34Z50  
 ******************************************************************************/
#define MCU_MKM34Z50

/***************************************************************************//*!
 * @brief   Reserve trace buffer in RAM for Micro Trace Buffer (MTB) trace 
 *          capabilities.  
 * @param   MTB_RAM_RESERVE - this define reserves trace buffer in RAM for use 
 *          by Micro Trace Buffer (MTB). The trace buffer starts at 0x1FFFF000 
 *          and it has size of 64-bytes. You can modify size of the trace buffer 
 *          for example to 256-bytes by the following definition: 
 *          #define MTB_RAM_SIZE 0x100
 * @note    Tracing capabilities reduces performance of the application.
 ******************************************************************************/
//#define MTB_RAM_RESERVE

/***************************************************************************//*!
 * @brief   Force read-by-write accesses to on-chip peripheral registers via 
 *	        core's instruction set instead of using Bit Manipulation Engine (BME). 
 *	        Using BME results in a better code density.
 * @param   EMULATE_BME_IO_ACCESS - define this parameter in order to force 
 *          read-by-write accesses to on-chip peripheral registers via core's 
 *	        instruction set (BME read-by-write accesses enabled by default)
 ******************************************************************************/
//#define EMULATE_BME_IO_ACCESS

/***************************************************************************//*!
 * @brief   Modify default boot options. The boot options allows the MCU to 
 *          modify default operations from NVM at reset. 
 *          Supported options:
 * @param   BOOT_FLASH_SECURE - MCU Secured (MCU is unsecured by default; secure
 *                              command is blocked in IAR 6.40.2 flash loader)
 * @param   BOOT_DIV_CLK_BY_8 - Divide clock by 8 (MCU boots up with system clock 
 *                              divide by 1 by default).   
 * @param   BOOT_NMI_DISABLE  - Disable NMI (NMI is enabled by default)
 * @param   BOOT_EXT_CLK      - Device uses external slower clock for flash  
 *                              calibration after reset. Internal 25 MHz flash 
 *                              clock is disabled.
 * @param   BOOT_HWVLPR_MODE  - Device boots in VLPR mode starting next VDD power 
 *                              cycle provided VBAT power is retained (forced by HW).
 * @param   BOOT_SWVLPR_MODE  - Device boots in RUN mode however device enters 
 *                              VLPR by first software instructions of the device
 *                              startup (forced by SW).
 * @note    If neither BOOT_EXE_MODE, BOOT_VLPR_MODE nor BOOT_EXT_FLASH_CLK 
 *          constants are defined then device boots in default mode which is RUN 
 *          mode with internal 25 MHz clock enabled and used for flash calibration.
 ******************************************************************************/
#define BOOT_NMI_DISABLE  
#define BOOT_EXT_CLK
//#define BOOT_FLASH_SECURE
   
/***************************************************************************//*!
 * @brief   Define specific peripheral configuration structures.
 * @example 
 *          #define PIT_CH_CONFIG                                              \
 *          (tPIT_CH){                                                         \
 *            SET(PIT_TCTRL_TIE_MASK)|SET(PIT_TCTRL_TEN_MASK),                 \
 *          } 
 ******************************************************************************/
#define ADC_MODULE_16B_SWTRG_BGAP_LP_INTERNAL_CLK_CONFIG                      \
(tADC){                                                                       \
/* CFG1   */  SET(ADC_CFG1_ADLPC_MASK)|SET(ADC_CFG1_ADIV(0))|                 \
              CLR(ADC_CFG1_ADLSMP_MASK)|SET(ADC_CFG1_MODE(3))|                \
              SET(ADC_CFG1_ADICLK(3)),                                        \
/* CFG2   */  CLR(ADC_CFG2_MUXSEL_MASK)|SET(ADC_CFG2_ADACKEN_MASK)|           \
              CLR(ADC_CFG2_ADHSC_MASK)|SET(ADC_CFG2_ADLSTS(3)),               \
/* CV1    */  0l,                                                             \
/* CV2    */  0l,                                                             \
/* SC2    */  CLR(ADC_SC2_ADTRG_MASK)|CLR(ADC_SC2_ACFE_MASK)|                 \
              CLR(ADC_SC2_ACFGT_MASK)|CLR(ADC_SC2_ACREN_MASK)|                \
              CLR(ADC_SC2_DMAEN_MASK)|SET(ADC_SC2_REFSEL(2)),                 \
/* SC3    */  CLR(ADC_SC3_CAL_MASK)|CLR(ADC_SC3_ADCO_MASK)|                   \
              CLR(ADC_SC3_AVGE_MASK)|SET(ADC_SC3_AVGS(0)),                    \
}

/***************************************************************************//*!
 * @brief   LCD driver setting.
 * @details Configures LCD driver with following characteristics:
 *  - Regulated voltage disabled,
 *  - Regulated Voltage Trim se to 8 (default),
 *  - Resistor network is enabled and charge pump is disabled.
 *  - Resistor bias network for low load LCD glass capacitance 2000 pF or lower
 *    selected,
 *  - Drive VLL3 internally from VDD,
 *  - LCD Frame Frequency Interrupt disabled,
 *  - LCD Fault Detection Complete Interrupt disabled,
 *  - LCD AlternateClock Divider set to 1 (no divide),
 *  - Disable the LCD driver, charge pump, resistor bias network, and voltage 
 *    regulator to continue running during Doze mode,
 *  - Disable the LCD driver, charge pump, resistor bias network, and voltage 
 *    regulator to continue running during Stop mode,
 *  - LCD Driver Enabled
 *  - Default clock selected as the LCD clock source - see SIM_SOPT1[OSC32KSEL]
 *  - Frame frequency set to 36.67 Hz (LCD Clock Prescaler LCLK = 3), 
 *  - 4 BP (1/4 duty cycle) (LCD Duty DUTY = 3).
 ******************************************************************************/
#define YDDC41919AATDRPN_VSUPPLY_OFF_CPSEL_OFF_RVEN_OFF_BACKPLANES_4           \
(tLCD){                                                                        \
/* GCR    */    CLR(LCD_GCR_RVEN_MASK)    |SET(LCD_GCR_RVTRIM(0x8)) |          \
                CLR(LCD_GCR_CPSEL_MASK)   |SET(LCD_GCR_LADJ(0x0))   |          \
                CLR(LCD_GCR_VSUPPLY_MASK) |CLR(LCD_GCR_LCDIEN_MASK) |          \
                CLR(LCD_GCR_FDCIEN_MASK)  |SET(LCD_GCR_ALTDIV(0x0)) |          \
                SET(LCD_GCR_LCDDOZE_MASK) |SET(LCD_GCR_LCDSTP_MASK) |          \
                SET(LCD_GCR_LCDEN_MASK)   |CLR(LCD_GCR_SOURCE_MASK) |          \
                SET(LCD_GCR_LCLK(0x3))    |SET(LCD_GCR_DUTY(0x3))   | (1l<<16),\
/* AR    */     CLR(LCD_AR_LCDIF_MASK)    |CLR(LCD_AR_BLINK_MASK)   |          \
                CLR(LCD_AR_ALT_MASK)      |CLR(LCD_AR_BLANK_MASK)   |          \
                CLR(LCD_AR_BMODE_MASK)    |CLR(LCD_AR_BRATE(0x0)),             \
/* FDCR  */     CLR(LCD_FDCR_FDPRS(0x0))  |CLR(LCD_FDCR_FDSWW(0x0)) |          \
                CLR(LCD_FDCR_FDEN_MASK)   |CLR(LCD_FDCR_FDBPEN_MASK)|          \
                CLR(LCD_FDCR_FDPINID(0x00)),                                   \
}

#define YDDC41919AATDRPN_LCD_FRONT_PLANE_PINS                                  \
(tLCD_FP){                                                                     \
/* PENL  */     CLR(LCD_00) |CLR(LCD_01) |CLR(LCD_02) |CLR(LCD_03)|            \
                CLR(LCD_04) |CLR(LCD_05) |CLR(LCD_06) |CLR(LCD_07)|            \
                CLR(LCD_08) |CLR(LCD_09) |CLR(LCD_10) |CLR(LCD_11)|            \
                CLR(LCD_12) |SET(LCD_13) |SET(LCD_14) |SET(LCD_15)|            \
                SET(LCD_16) |SET(LCD_17) |SET(LCD_18) |SET(LCD_19)|            \
                SET(LCD_20) |SET(LCD_21) |SET(LCD_22) |SET(LCD_23)|            \
                SET(LCD_24) |SET(LCD_25) |SET(LCD_26) |SET(LCD_27)|            \
                SET(LCD_28) |SET(LCD_29) |SET(LCD_30) |SET(LCD_31),            \
/* PENH  */     SET(LCD_00) |SET(LCD_01) |SET(LCD_02) |SET(LCD_03)|            \
                SET(LCD_04) |SET(LCD_05) |SET(LCD_06) |CLR(LCD_07)|            \
                CLR(LCD_08) |CLR(LCD_09) |CLR(LCD_10) |CLR(LCD_11)|            \
                CLR(LCD_12) |CLR(LCD_13) |CLR(LCD_14) |CLR(LCD_15)|            \
                CLR(LCD_16) |CLR(LCD_17) |CLR(LCD_18) |CLR(LCD_19)|            \
                CLR(LCD_20) |CLR(LCD_21) |CLR(LCD_22) |CLR(LCD_23)|            \
                CLR(LCD_24) |CLR(LCD_25) |CLR(LCD_26) |CLR(LCD_27)|            \
                CLR(LCD_28) |CLR(LCD_29) |CLR(LCD_30) |CLR(LCD_31),            \
}

#define YDDC41919AATDRPN_LCD_BACK_PLANE_PINS                                   \
(tLCD_BP){                                                                     \
/* BPENL  */    CLR(LCD_00) |CLR(LCD_01) |CLR(LCD_02) |CLR(LCD_03)|            \
                CLR(LCD_04) |CLR(LCD_05) |CLR(LCD_06) |CLR(LCD_07)|            \
                CLR(LCD_08) |CLR(LCD_09) |CLR(LCD_10) |CLR(LCD_11)|            \
                CLR(LCD_12) |SET(LCD_13) |SET(LCD_14) |SET(LCD_15)|            \
                SET(LCD_16) |CLR(LCD_17) |CLR(LCD_18) |CLR(LCD_19)|            \
                CLR(LCD_20) |CLR(LCD_21) |CLR(LCD_22) |CLR(LCD_23)|            \
                CLR(LCD_24) |CLR(LCD_25) |CLR(LCD_26) |CLR(LCD_27)|            \
                CLR(LCD_28) |CLR(LCD_29) |CLR(LCD_30) |CLR(LCD_31),            \
/* BPENH  */    CLR(LCD_00) |CLR(LCD_01) |CLR(LCD_02) |CLR(LCD_03)|            \
                CLR(LCD_04) |CLR(LCD_05) |CLR(LCD_06) |CLR(LCD_07)|            \
                CLR(LCD_08) |CLR(LCD_09) |CLR(LCD_10) |CLR(LCD_11)|            \
                CLR(LCD_12) |CLR(LCD_13) |CLR(LCD_14) |CLR(LCD_15)|            \
                CLR(LCD_16) |CLR(LCD_17) |CLR(LCD_18) |CLR(LCD_19)|            \
                CLR(LCD_20) |CLR(LCD_21) |CLR(LCD_22) |CLR(LCD_23)|            \
                CLR(LCD_24) |CLR(LCD_25) |CLR(LCD_26) |CLR(LCD_27)|            \
                CLR(LCD_28) |CLR(LCD_29) |CLR(LCD_30) |CLR(LCD_31),            \
}

#define YDDC41919AATDRPN_LCD_ASSIGN_BACK_PLANES                                \
(tLCD_CP){                                                                     \
/* WF3TO0   */  LCD_WF_WF3 (LCD_OFF) |LCD_WF_WF2 (LCD_OFF)|                    \
                LCD_WF_WF1 (LCD_OFF) |LCD_WF_WF0 (LCD_OFF),                    \
/* WF7TO4   */  LCD_WF_WF7 (LCD_OFF) |LCD_WF_WF6 (LCD_OFF)|                    \
                LCD_WF_WF5 (LCD_OFF) |LCD_WF_WF4 (LCD_OFF),                    \
/* WF11TO8  */  LCD_WF_WF11(LCD_OFF) |LCD_WF_WF10(LCD_OFF)|                    \
                LCD_WF_WF9 (LCD_OFF) |LCD_WF_WF8 (LCD_OFF),                    \
/* WF15TO12 */  LCD_WF_WF15(LCD_BP2) |LCD_WF_WF14(LCD_BP3)|                    \
                LCD_WF_WF13(LCD_BP4) |LCD_WF_WF12(LCD_OFF),                    \
/* WF19TO16 */  LCD_WF_WF19(LCD_OFF) |LCD_WF_WF18(LCD_OFF)|                    \
                LCD_WF_WF17(LCD_OFF) |LCD_WF_WF16(LCD_BP1),                    \
/* WF23TO20 */  LCD_WF_WF23(LCD_OFF) |LCD_WF_WF22(LCD_OFF)|                    \
                LCD_WF_WF21(LCD_OFF) |LCD_WF_WF20(LCD_OFF),                    \
/* WF27TO24 */  LCD_WF_WF27(LCD_OFF) |LCD_WF_WF26(LCD_OFF)|                    \
                LCD_WF_WF25(LCD_OFF) |LCD_WF_WF24(LCD_OFF),                    \
/* WF31TO28 */  LCD_WF_WF31(LCD_OFF) |LCD_WF_WF30(LCD_OFF)|                    \
                LCD_WF_WF29(LCD_OFF) |LCD_WF_WF28(LCD_OFF),                    \
/* WF35TO32 */  LCD_WF_WF35(LCD_OFF) |LCD_WF_WF34(LCD_OFF)|                    \
                LCD_WF_WF33(LCD_OFF) |LCD_WF_WF32(LCD_OFF),                    \
/* WF39TO36 */  LCD_WF_WF39(LCD_OFF) |LCD_WF_WF38(LCD_OFF)|                    \
                LCD_WF_WF37(LCD_OFF) |LCD_WF_WF36(LCD_OFF),                    \
/* WF43TO40 */  LCD_WF_WF43(LCD_OFF) |LCD_WF_WF42(LCD_OFF)|                    \
                LCD_WF_WF41(LCD_OFF) |LCD_WF_WF40(LCD_OFF),                    \
/* WF47TO44 */  LCD_WF_WF47(LCD_OFF) |LCD_WF_WF46(LCD_OFF)|                    \
                LCD_WF_WF45(LCD_OFF) |LCD_WF_WF44(LCD_OFF),                    \
/* WF51TO48 */  LCD_WF_WF51(LCD_OFF) |LCD_WF_WF50(LCD_OFF)|                    \
                LCD_WF_WF49(LCD_OFF) |LCD_WF_WF48(LCD_OFF),                    \
/* WF55TO52 */  LCD_WF_WF55(LCD_OFF) |LCD_WF_WF54(LCD_OFF)|                    \
                LCD_WF_WF53(LCD_OFF) |LCD_WF_WF52(LCD_OFF),                    \
/* WF59TO56 */  LCD_WF_WF59(LCD_OFF) |LCD_WF_WF58(LCD_OFF)|                    \
                LCD_WF_WF57(LCD_OFF) |LCD_WF_WF56(LCD_OFF),                    \
/* WF63TO60 */  LCD_WF_WF63(LCD_OFF) |LCD_WF_WF62(LCD_OFF)|                    \
                LCD_WF_WF61(LCD_OFF) |LCD_WF_WF60(LCD_OFF),                    \
}

/***************************************************************************//*!
 * @brief   Normal mode configuration.
 *****************************************************************************/
#define SIM_MODULE_NORM_MODE_SYSCLKMODE_SYSCLK_DIV1_CONFIG                      \
(tSIM){                                                                         \
/* SOPT1    */ SET(SIM_SOPT1_OSC32KSEL(0)),                                     \
/* SOPT1_CFG*/ CLR(SIM_SOPT1_CFG_RAMBPEN_MASK)|CLR(SIM_SOPT1_CFG_RAMSBDIS_MASK)|\
               CLR(SIM_SOPT1_CFG_CMPOLPTMR0SEL_MASK)|                           \
               SET(SIM_SOPT1_CFG_LPTMR3SEL(0))|SET(SIM_SOPT1_CFG_LPTMR2SEL(0))| \
               SET(SIM_SOPT1_CFG_LPTMR1SEL(0)),                                 \
/* CTRL_REG */ SET(SIM_CTRL_REG_CLKOUTSEL(5))|                                  \
               SET(SIM_CTRL_REG_SAR_TRG_CLK_SEL(0))|                            \
               CLR(SIM_CTRL_REG_PTC2_HD_EN_MASK)|                               \
               CLR(SIM_CTRL_REG_PLL_VLP_EN_MASK)|CLR(SIM_CTRL_REG_NMIDIS_MASK), \
/* SCGC4    */ CLR(SIM_SCGC4_SPI1_MASK)|CLR(SIM_SCGC4_SPI0_MASK)|               \
               SET(SIM_SCGC4_CMP1_MASK)|CLR(SIM_SCGC4_CMP0_MASK)|               \
               SET(SIM_SCGC4_VREF_MASK)|SET(SIM_SCGC4_UART3_MASK)|              \
               CLR(SIM_SCGC4_UART2_MASK)|SET(SIM_SCGC4_UART1_MASK)|             \
               SET(SIM_SCGC4_UART0_MASK)|CLR(SIM_SCGC4_I2C1_MASK)|              \
               CLR(SIM_SCGC4_I2C0_MASK)|CLR(SIM_SCGC4_OSC_MASK)|                \
               SET(SIM_SCGC4_MCG_MASK)|CLR(SIM_SCGC4_EWM_MASK),                 \
/* SCGC5    */ CLR(SIM_SCGC5_TMR3_MASK)|SET(SIM_SCGC5_TMR2_MASK)|               \
               SET(SIM_SCGC5_TMR1_MASK)|SET(SIM_SCGC5_TMR0_MASK)|               \
               SET(SIM_SCGC5_XBAR_MASK)|SET(SIM_SCGC5_WDOG_MASK)|               \
               CLR(SIM_SCGC5_IRTCREGFILE_MASK)|SET(SIM_SCGC5_IRTC_MASK)|        \
               SET(SIM_SCGC5_PORTI_MASK)|SET(SIM_SCGC5_PORTH_MASK)|             \
               SET(SIM_SCGC5_PORTG_MASK)|SET(SIM_SCGC5_PORTF_MASK)|             \
               SET(SIM_SCGC5_PORTE_MASK)|SET(SIM_SCGC5_PORTD_MASK)|             \
               SET(SIM_SCGC5_PORTC_MASK)|SET(SIM_SCGC5_PORTB_MASK)|             \
               SET(SIM_SCGC5_PORTA_MASK)|SET(SIM_SCGC5_SLCD_MASK),              \
/* SCGC6    */ SET(SIM_SCGC6_SIM_LP_MASK)|                                      \
               SET(SIM_SCGC6_LPTMR_MASK)|CLR(SIM_SCGC6_CRC_MASK)|               \
               SET(SIM_SCGC6_AFE_MASK)|CLR(SIM_SCGC6_PIT1_MASK)|                \
               CLR(SIM_SCGC6_PIT0_MASK)|SET(SIM_SCGC6_ADC_MASK)|                \
               CLR(SIM_SCGC6_RNGA_MASK)|CLR(SIM_SCGC6_DMAMUX3_MASK)|            \
               CLR(SIM_SCGC6_DMAMUX2_MASK)|CLR(SIM_SCGC6_DMAMUX1_MASK)|         \
               CLR(SIM_SCGC6_DMAMUX0_MASK)|SET(SIM_SCGC6_FTFA_MASK),            \
/* SCGC7    */ CLR(SIM_SCGC7_DMA_MASK)|CLR(SIM_SCGC7_MPU_MASK),                 \
/* CLKDIV1  */ SET(SIM_CLKDIV1_SYSDIV(SYSCLK_DIV1))|                            \
               SET(SIM_CLKDIV1_SYSCLKMODE_MASK),                                \
/* FCCFG1   */ SET(SIM_FCFG1_FLASHDOZE_MASK)|CLR(SIM_FCFG1_FLASHDIS_MASK),      \
/* MISC_CTL */ SET(SIM_MISC_CTL_VREFBUFPD_MASK)|                                \
               CLR(SIM_MISC_CTL_VREFBUFINSEL_MASK)|                             \
               CLR(SIM_MISC_CTL_VREFBUFOUTEN_MASK)|                             \
               CLR(SIM_MISC_CTL_RTCCLKSEL_MASK)|                                \
               CLR(SIM_MISC_CTL_TMR3PCSSEL(0))|                                 \
               CLR(SIM_MISC_CTL_TMR2PCSSEL(0))|                                 \
               CLR(SIM_MISC_CTL_TMR1PCSSEL(0))|                                 \
               CLR(SIM_MISC_CTL_TMR0PCSSEL(0))|                                 \
               CLR(SIM_MISC_CTL_TMR3SCSSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_TMR2SCSSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_TMR1SCSSEL_MASK)|                               \
               SET(SIM_MISC_CTL_TMR0SCSSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_TMR0PLLCLKSEL_MASK)|                            \
               CLR(SIM_MISC_CTL_EWMINSEL_MASK)|                                 \
               CLR(SIM_MISC_CTL_XBARPITOUTSEL(0))|                              \
               CLR(SIM_MISC_CTL_UART3IRSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_UART2IRSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_UART1IRSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_UART0IRSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_UARTMODTYPE_MASK)|                              \
               CLR(SIM_MISC_CTL_AFECLKPADDIR_MASK)|                             \
               SET(SIM_MISC_CTL_AFECLKSEL(0))|                                  \
               SET(SIM_MISC_CTL_DMADONESEL(0))|                                 \
               SET(SIM_MISC_CTL_XBARAFEMODOUTSEL(0)),                           \
}

/***************************************************************************//*!
 * @brief   Standby mode configuration.
 *****************************************************************************/
#define SIM_MODULE_STBY_MODE_SYSCLKMODE_SYSCLK_DIV16_CONFIG                     \
(tSIM){                                                                         \
/* SOPT1    */ SET(SIM_SOPT1_OSC32KSEL(0)),                                     \
/* SOPT1_CFG*/ CLR(SIM_SOPT1_CFG_RAMBPEN_MASK)|CLR(SIM_SOPT1_CFG_RAMSBDIS_MASK)|\
               CLR(SIM_SOPT1_CFG_CMPOLPTMR0SEL_MASK)|                           \
               SET(SIM_SOPT1_CFG_LPTMR3SEL(0))|SET(SIM_SOPT1_CFG_LPTMR2SEL(0))| \
               SET(SIM_SOPT1_CFG_LPTMR1SEL(0)),                                 \
/* CTRL_REG */ SET(SIM_CTRL_REG_CLKOUTSEL(0))|                                  \
               SET(SIM_CTRL_REG_SAR_TRG_CLK_SEL(0))|                            \
               CLR(SIM_CTRL_REG_PTC2_HD_EN_MASK)|                               \
               CLR(SIM_CTRL_REG_PLL_VLP_EN_MASK)|CLR(SIM_CTRL_REG_NMIDIS_MASK), \
/* SCGC4    */ CLR(SIM_SCGC4_SPI1_MASK)|CLR(SIM_SCGC4_SPI0_MASK)|               \
               CLR(SIM_SCGC4_CMP1_MASK)|CLR(SIM_SCGC4_CMP0_MASK)|               \
               CLR(SIM_SCGC4_VREF_MASK)|CLR(SIM_SCGC4_UART3_MASK)|              \
               CLR(SIM_SCGC4_UART2_MASK)|CLR(SIM_SCGC4_UART1_MASK)|             \
               CLR(SIM_SCGC4_UART0_MASK)|CLR(SIM_SCGC4_I2C1_MASK)|              \
               CLR(SIM_SCGC4_I2C0_MASK)|CLR(SIM_SCGC4_OSC_MASK)|                \
               CLR(SIM_SCGC4_MCG_MASK)|CLR(SIM_SCGC4_EWM_MASK),                 \
/* SCGC5    */ CLR(SIM_SCGC5_TMR3_MASK)|CLR(SIM_SCGC5_TMR2_MASK)|               \
               CLR(SIM_SCGC5_TMR1_MASK)|CLR(SIM_SCGC5_TMR0_MASK)|               \
               CLR(SIM_SCGC5_XBAR_MASK)|SET(SIM_SCGC5_WDOG_MASK)|               \
               CLR(SIM_SCGC5_IRTCREGFILE_MASK)|SET(SIM_SCGC5_IRTC_MASK)|        \
               CLR(SIM_SCGC5_PORTI_MASK)|CLR(SIM_SCGC5_PORTH_MASK)|             \
               SET(SIM_SCGC5_PORTG_MASK)|SET(SIM_SCGC5_PORTF_MASK)|             \
               CLR(SIM_SCGC5_PORTE_MASK)|SET(SIM_SCGC5_PORTD_MASK)|             \
               SET(SIM_SCGC5_PORTC_MASK)|CLR(SIM_SCGC5_PORTB_MASK)|             \
               SET(SIM_SCGC5_PORTA_MASK)|SET(SIM_SCGC5_SLCD_MASK),              \
/* SCGC6    */ SET(SIM_SCGC6_SIM_LP_MASK)|                                      \
               SET(SIM_SCGC6_LPTMR_MASK)|CLR(SIM_SCGC6_CRC_MASK)|               \
               CLR(SIM_SCGC6_AFE_MASK)|CLR(SIM_SCGC6_PIT1_MASK)|                \
               CLR(SIM_SCGC6_PIT0_MASK)|SET(SIM_SCGC6_ADC_MASK)|                \
               CLR(SIM_SCGC6_RNGA_MASK)|CLR(SIM_SCGC6_DMAMUX3_MASK)|            \
               CLR(SIM_SCGC6_DMAMUX2_MASK)|CLR(SIM_SCGC6_DMAMUX1_MASK)|         \
               CLR(SIM_SCGC6_DMAMUX0_MASK)|SET(SIM_SCGC6_FTFA_MASK),            \
/* SCGC7    */ CLR(SIM_SCGC7_DMA_MASK)|CLR(SIM_SCGC7_MPU_MASK),                 \
/* CLKDIV1  */ SET(SIM_CLKDIV1_SYSDIV(SYSCLK_DIV16))|                           \
               SET(SIM_CLKDIV1_SYSCLKMODE_MASK),                                \
/* FCCFG1   */ SET(SIM_FCFG1_FLASHDOZE_MASK)|CLR(SIM_FCFG1_FLASHDIS_MASK),      \
/* MISC_CTL */ SET(SIM_MISC_CTL_VREFBUFPD_MASK)|                                \
               CLR(SIM_MISC_CTL_VREFBUFINSEL_MASK)|                             \
               CLR(SIM_MISC_CTL_VREFBUFOUTEN_MASK)|                             \
               CLR(SIM_MISC_CTL_RTCCLKSEL_MASK)|                                \
               CLR(SIM_MISC_CTL_TMR3PCSSEL(0))|                                 \
               CLR(SIM_MISC_CTL_TMR2PCSSEL(0))|                                 \
               CLR(SIM_MISC_CTL_TMR1PCSSEL(0))|                                 \
               CLR(SIM_MISC_CTL_TMR0PCSSEL(0))|                                 \
               CLR(SIM_MISC_CTL_TMR3SCSSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_TMR2SCSSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_TMR1SCSSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_TMR0SCSSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_TMR0PLLCLKSEL_MASK)|                            \
               CLR(SIM_MISC_CTL_EWMINSEL_MASK)|                                 \
               CLR(SIM_MISC_CTL_XBARPITOUTSEL(0))|                              \
               CLR(SIM_MISC_CTL_UART3IRSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_UART2IRSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_UART1IRSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_UART0IRSEL_MASK)|                               \
               CLR(SIM_MISC_CTL_UARTMODTYPE_MASK)|                              \
               CLR(SIM_MISC_CTL_AFECLKPADDIR_MASK)|                             \
               SET(SIM_MISC_CTL_AFECLKSEL(0))|                                  \
               SET(SIM_MISC_CTL_DMADONESEL(0))|                                 \
               SET(SIM_MISC_CTL_XBARAFEMODOUTSEL(0)),                           \
}

/***************************************************************************//*!
 * @brief Wakes up the device if PTC5 rising and PTD7 falling edges occur. 
 * Digital filter enabled.
 *****************************************************************************/
#define LLWU_MODULE_PTC5_RISING_EDGE_PTD7_FALLING_EDGE_FILT_ENABLED           \
(tLLWU){                                                                      \
/* PE1  */ SET(LLWU_PE1_WUPE0 (0))|SET(LLWU_PE1_WUPE1 (0))  |                 \
           SET(LLWU_PE1_WUPE2 (0))|SET(LLWU_PE1_WUPE3 (0)),                   \
/* PE2  */ SET(LLWU_PE2_WUPE4 (0))|SET(LLWU_PE2_WUPE5 (0))  |                 \
           SET(LLWU_PE2_WUPE6 (0))|SET(LLWU_PE2_WUPE7 (0)),                   \
/* PE3  */ SET(LLWU_PE3_WUPE8 (0))|SET(LLWU_PE3_WUPE9 (0))  |                 \
           SET(LLWU_PE3_WUPE10(0))|SET(LLWU_PE3_WUPE11(0)),                   \
/* PE4  */ SET(LLWU_PE4_WUPE12(0))|SET(LLWU_PE4_WUPE13(0))  |                 \
           SET(LLWU_PE4_WUPE14(0))|SET(LLWU_PE4_WUPE15(0)),                   \
/* ME   */ CLR(LLWU_ME_WUME0_MASK)|CLR(LLWU_ME_WUME1_MASK)  |                 \
           CLR(LLWU_ME_WUME2_MASK)|CLR(LLWU_ME_WUME3_MASK)  |                 \
           CLR(LLWU_ME_WUME4_MASK),                                           \
/* FILT1*/ SET(LLWU_FILT1_FILTE(2))|SET(LLWU_FILT1_FILTSEL(7)),               \
/* FILT2*/ SET(LLWU_FILT2_FILTE(1))|SET(LLWU_FILT2_FILTSEL(12)),              \
}

/***************************************************************************//*!
  * @brief Comparator Frequency Detection Setting:
  * - filter disabled, 
  * - hysteresis 5mv,
  * - sample, windowing and trigger modes disabled,
  * - high speed mode,
  * - non-inverting polarity,
  * - unfiltered signal propagated to output (disabled),
  * - comparator enabled,
  * - CMP filter disabled when CR1[SE]=0, 
  * - IRQ and DMA disabled,
  * - DAC disabled.
  *****************************************************************************/
#define CMP_FREQ_DETECT_MODE_CONFIG                                           \
(tCMP){                                                                       \
/* CR0    */  SET(CMP_CR0_FILTER_CNT(0)) |SET(CMP_CR0_HYSTCTR(0)),            \
/* CR1    */  CLR(CMP_CR1_SE_MASK)       |CLR(CMP_CR1_WE_MASK)|               \
              CLR(CMP_CR1_TRIGM_MASK)    |SET(CMP_CR1_PMODE_MASK)|            \
              CLR(CMP_CR1_INV_MASK)      |CLR(CMP_CR1_COS_MASK)|              \
              CLR(CMP_CR1_OPE_MASK)      |SET(CMP_CR1_EN_MASK),               \
/* FPR    */  SET(CMP_FPR_FILT_PER(0x00)),                                    \
/* SCR    */  CLR(CMP_SCR_DMAEN_MASK)    |CLR(CMP_SCR_IER_MASK)|              \
              CLR(CMP_SCR_IEF_MASK),                                          \
/* DACCR  */  CLR(CMP_DACCR_DACEN_MASK)  |CLR(CMP_DACCR_VRSEL_MASK)           \
}

/***************************************************************************//*!
 * @brief   Input Capture Mode Frequency Detection Setting
 * Other characteristics:
 * - Count rising edges
 * - Rollover enabled
 * - One shot Timer mode disabled
 * - Overflow interrupt disabled
 * - Co-channel initialization disabled
 * - Compare interrupt disabled
 * - Input capture interrupt disabled
 * - Input signal polarity not inverted
 * - Continue with normal operation during debug mode (default)
 * - Reload counter on capture event
 * @param   prm_clk_src     Use one of the following @ref tmr_prm_src
 * @param   sec_clk_src     Use one of the following @ref tmr_sec_src 
 * @param   input_mode      Use one of the following @ref tmr_capt_mode
 *****************************************************************************/
#define TMR_FREQ_DETECT_MODE(prm_clk_src,sec_clk_src,input_mode)              \
(tTMR_CH){                                                                    \
/* CTRL   */    SET(TMR_CTRL_CM(COUNT_RISING_EDGE))|                          \
                SET(TMR_CTRL_PCS(prm_clk_src))|SET(TMR_CTRL_SCS(sec_clk_src))|\
                CLR(TMR_CTRL_DIR_MASK)|CLR(TMR_CTRL_LENGTH_MASK)|             \
                CLR(TMR_CTRL_ONCE_MASK)|CLR(TMR_CTRL_COINIT_MASK)|            \
                SET(TMR_CTRL_OUTMODE(ASSERT_ON_CNTR_ACTIVE)),                 \
/* SCTRL    */  CLR(TMR_SCTRL_TCFIE_MASK)|CLR(TMR_SCTRL_TOFIE_MASK)|          \
                CLR(TMR_SCTRL_IEFIE_MASK)|CLR(TMR_SCTRL_IPS_MASK)|            \
                SET(TMR_SCTRL_CAPTURE_MODE(input_mode))|                      \
                CLR(TMR_SCTRL_MSTR_MASK)|CLR(TMR_SCTRL_EEOF_MASK)|            \
                CLR(TMR_SCTRL_VAL_MASK)|CLR(TMR_SCTRL_TOF_MASK)|              \
                CLR(TMR_SCTRL_OPS_MASK)|CLR(TMR_SCTRL_OEN_MASK)|              \
                CLR(TMR_SCTRL_FORCE_MASK),                                    \
 /* CSCTRL  */  SET(TMR_CSCTRL_DBG_EN(0))|CLR(TMR_CSCTRL_FAULT_MASK)|         \
                CLR(TMR_CSCTRL_ALT_LOAD_MASK)|SET(TMR_CSCTRL_ROC_MASK)|       \
                CLR(TMR_CSCTRL_TCI_MASK)|CLR(TMR_CSCTRL_TCF2EN_MASK)|         \
                CLR(TMR_CSCTRL_TCF1EN_MASK)|SET(TMR_CSCTRL_CL2(0))|           \
                SET(TMR_CSCTRL_CL1(0)),                                       \
}

/***************************************************************************//*!
 * @brief   Timer Pulse Output Control One Shoot Mode
 * @details This use case will configure the Quad Timer channel in one shoot 
 * counting mode with the following initial characteristics:
 * - Counter operation disabled by default (will be controlled by 
 *   @ref TMR_SetCountMode() macro) 
 * - Primary count source = IP bus clock divide by 1 prescaler
 * - Secondary count source = passed by sec_clk_src input argument
 * - Count until compare and then stop
 * - Count until compare, then re-initialize
 * - Count up
 * - Co-channel counter/timers cannot force a Timer/counter re-initialization
 * - Output Mode = passed by output_mode input argument
 * - All interrupts disabled
 * - True polarity of input signal
 * - Capture function is disabled
 * - External OFLAG force enabled and logic 1 is forced to OFLAG output pin
 * - True output polarity
 * - OFLAG output signal is driven on the external pin.
 * - Continue with normal operation during debug mode (default)
 * - Fault function disabled
 * - Counter is re-initialized only with the LOAD register
 * - Do not reload the counter on a capture event
 * - Timer Compare 1 and Compare 2 interrupt disabled
 * - COMP1 and CMP2 are never preloaded
 * @param   sec_clk_src     Use one of the following @ref tmr_sec_src 
 * @param   output_mode     Use one of the following @ref tmr_out_mode 
 *****************************************************************************/
#define TMR_PULSE_OUTPUT_ONE_SHOOT_MODE_CONFIG(sec_clk_src,output_mode)       \
(tTMR_CH){                                                                    \
/* CTRL     */  SET(TMR_CTRL_CM(COUNT_DISABLED))|                             \
                SET(TMR_CTRL_PCS(BUS_CLK_DIV1))|                              \
                SET(TMR_CTRL_SCS(sec_clk_src))|                               \
                CLR(TMR_CTRL_DIR_MASK)|SET(TMR_CTRL_LENGTH_MASK)|             \
                SET(TMR_CTRL_ONCE_MASK)|CLR(TMR_CTRL_COINIT_MASK)|            \
                SET(TMR_CTRL_OUTMODE(output_mode)),                           \
/* SCTRL    */  CLR(TMR_SCTRL_TCFIE_MASK)|CLR(TMR_SCTRL_TOFIE_MASK)|          \
                CLR(TMR_SCTRL_IEFIE_MASK)|CLR(TMR_SCTRL_IPS_MASK)|            \
                SET(TMR_SCTRL_CAPTURE_MODE(CAPTURE_DISABLED))|                \
                CLR(TMR_SCTRL_MSTR_MASK)|CLR(TMR_SCTRL_EEOF_MASK)|            \
                SET(TMR_SCTRL_VAL_MASK)|CLR(TMR_SCTRL_TOF_MASK)|              \
                CLR(TMR_SCTRL_OPS_MASK)|SET(TMR_SCTRL_OEN_MASK)|              \
                SET(TMR_SCTRL_FORCE_MASK),                                    \
 /* CSCTRL  */  SET(TMR_CSCTRL_DBG_EN(0))|CLR(TMR_CSCTRL_FAULT_MASK)|         \
                CLR(TMR_CSCTRL_ALT_LOAD_MASK)|CLR(TMR_CSCTRL_ROC_MASK)|       \
                CLR(TMR_CSCTRL_TCI_MASK)|CLR(TMR_CSCTRL_TCF2EN_MASK)|         \
                CLR(TMR_CSCTRL_TCF1EN_MASK)|SET(TMR_CSCTRL_CL2(0))|           \
                SET(TMR_CSCTRL_CL1(0)),                                       \
}

/**************************************************************************/ /*!
  * @brief Comparator Frequency Detection Setting:
  * - filter disabled, 
  * - hysteresis 5mv,
  * - sample, windowing and trigger modes disabled,
  * - high speed mode,
  * - non-inverting polarity,
  * - unfiltered signal propagated to output (disabled),
  * - comparator enabled,
  * - CMP filter disabled when CR1[SE]=0, 
  * - IRQ and DMA disabled,
  * - DAC disabled.
  *****************************************************************************/
#define CMP_FREQ_DETECT_MODE_CONFIG_EN_DAC(dac)                               \
(tCMP){                                                                       \
/* CR0    */  SET(CMP_CR0_FILTER_CNT(7)) |SET(CMP_CR0_HYSTCTR(3)),            \
/* CR1    */  CLR(CMP_CR1_SE_MASK)       |CLR(CMP_CR1_WE_MASK)|               \
              CLR(CMP_CR1_TRIGM_MASK)    |SET(CMP_CR1_PMODE_MASK)|            \
              CLR(CMP_CR1_INV_MASK)      |CLR(CMP_CR1_COS_MASK)|              \
              CLR(CMP_CR1_OPE_MASK)      |SET(CMP_CR1_EN_MASK),               \
/* FPR    */  SET(CMP_FPR_FILT_PER(0xFF)),                                     \
/* SCR    */  CLR(CMP_SCR_DMAEN_MASK)    |CLR(CMP_SCR_IER_MASK)|              \
              CLR(CMP_SCR_IEF_MASK),                                          \
/* DACCR  */  SET(CMP_DACCR_DACEN_MASK)  |CLR(CMP_DACCR_VRSEL_MASK)| dac      \
}

/***************************************************************************//*!
 * @brief   Voltage reference switch setting.
 * @details Configures voltage reference switches:
 *          - Buffer enable,
 *          - Buffer drive voltage on PAD,   S1 = H,
 *          - Internal reference select,     S2 = L,
 *          - AFE Internal reference select, S3 = L,
 * @details The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 ******************************************************************************/
#define VREF_SWITCH_S1_H_S2_L_S3_L                                             \
(tVREF_S){                                                                     \
/* SIM_MISC  */  CLR(SIM_MISC_CTL_VREFBUFPD_MASK)|                             \
/* ......... */  CLR(SIM_MISC_CTL_VREFBUFINSEL_MASK)|                          \
/* ......... */  SET(SIM_MISC_CTL_VREFBUFOUTEN_MASK),                          \
/* VERFL_TRM */  CLR(VREF_VREFL_TRM_VREFL_SEL_MASK),                           \
}

                  
/***************************************************************************//*!
 * @brief   Define interrupt vectors.
 * @param   Supported devices - MCU_MKM34Z5
 * @example 
 *          extern void UART2_UART3_isr (void);
 *          #define VECTOR_036 UART2_UART3_isr
 ******************************************************************************/
extern void PTx_isr         (void);
#define VECTOR_033 PTx_isr

extern void UART0_UART1_isr (void);
#define VECTOR_035 UART0_UART1_isr

extern void UART2_UART3_isr (void);
#define VECTOR_036 UART2_UART3_isr

extern void AFE_CH2_isr     (void);
#define VECTOR_039 AFE_CH2_isr

extern void swisr0_isr      (void);
#define VECTOR_029 swisr0_isr

extern void swisr1_isr      (void);
#define VECTOR_043 swisr1_isr

extern void swisr2_isr      (void);
#define VECTOR_045 swisr2_isr

extern void hardfault_isr (void);
#define VECTOR_003 hardfault_isr

/******************************************************************************/
#endif /* __APPCONFIG_ */
