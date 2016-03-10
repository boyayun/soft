/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      uart.h
 * @version   1.0.5.0
 * @date      Nov-02-2013
 * @brief     Universal Asynchronous Receiver/Transmitter (UART) driver source 
 *            code.
 ******************************************************************************/
#ifndef __UART_H
#define __UART_H 
 
/******************************************************************************
 * definition configuration structures                                        *
 ******************************************************************************/
typedef struct { uint8 BDH, BDL, C1, C2, S2, C3, MA1, MA2, C4, C5, C7816,     \
                       IE7816, IS7816, WP7816T0, WP7816T1, WN7816, WF7816,    \
                       ET7816, TL7816; } tUART;

/******************************************************************************
 * macro definitions                                                          *
 ******************************************************************************/
//br=57600; clk=96e6;
//sbr=floor((clk/(16*br)))
//brfa=round(((clk/(16*br))-floor((clk/(16*br))))*32)
#define CALC_SBR(brate,bclk)  (uint16)((double)(bclk)/(16.0*(double)(brate)))
#define CALC_BRFA(brate,bclk) (uint16)(((((double)(bclk)/(16.0*(double)(brate)))- \
                                      (double)CALC_SBR(brate,bclk))*32.0)+0.5)

/******************************************************************************
* UART default configurations used by UART_init() function
*
*//*! @addtogroup uart_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief UART configured to operate in polling mode. Receive and transmit data 
 *        is not inverted.
 * @param   brate     Baud rate.
 * @param   bclk      Buss clock in Hz.
 *****************************************************************************/
#define UART_MODULE_POLLMODE_CONFIG(brate,bclk)                               \
(tUART){                                                                      \
/* BDH      */ CLR(UART_BDH_RXEDGIE_MASK)|((CALC_SBR(brate,bclk)>>8)&0x1f),   \
/* BDL      */ ((CALC_SBR(brate,bclk)>>0)&0xff),                              \
/* C1       */ CLR(UART_C1_LOOPS_MASK)|CLR(UART_C1_RSRC_MASK)|                \
/* ..       */ CLR(UART_C1_M_MASK)|CLR(UART_C1_WAKE_MASK)|                    \
/* ..       */ CLR(UART_C1_ILT_MASK)|CLR(UART_C1_PE_MASK)|                    \
/* ..       */ CLR(UART_C1_PT_MASK),                                          \
/* C2       */ CLR(UART_C2_TIE_MASK)|CLR(UART_C2_TCIE_MASK)|                  \
/* ..       */ CLR(UART_C2_RIE_MASK)|CLR(UART_C2_ILIE_MASK)|                  \
/* ..       */ SET(UART_C2_TE_MASK)|SET(UART_C2_RE_MASK)|                     \
/* ..       */ CLR(UART_C2_RWU_MASK)|CLR(UART_C2_SBK_MASK),                   \
/* S2       */ CLR(UART_S2_RXEDGIF_MASK)|CLR(UART_S2_MSBF_MASK)|              \
/* ..       */ CLR(UART_S2_RXINV_MASK)|CLR(UART_S2_RWUID_MASK)|               \
/* ..       */ CLR(UART_S2_BRK13_MASK),                                       \
/* C3       */ CLR(UART_C3_T8_MASK)|CLR(UART_C3_TXDIR_MASK)|                  \
/* ..       */ CLR(UART_C3_TXINV_MASK)|CLR(UART_C3_ORIE_MASK)|                \
/* ..       */ CLR(UART_C3_NEIE_MASK)|CLR(UART_C3_FEIE_MASK)|                 \
/* ..       */ CLR(UART_C3_PEIE_MASK),                                        \
/* MA1      */ SET(UART_MA1_MA(0x00)),                                        \
/* MA2      */ SET(UART_MA2_MA(0x00)),                                        \
/* C4       */ CLR(UART_C4_MAEN1_MASK)|CLR(UART_C4_MAEN2_MASK)|               \
/* ..       */ CLR(UART_C4_M10_MASK)|((CALC_BRFA(brate,bclk)>>0)&0x1f),       \
/* C5       */ CLR(UART_C5_TDMAS_MASK)|CLR(UART_C5_RDMAS_MASK),               \
/* C7816    */ CLR(UART_C7816_ONACK_MASK)|CLR(UART_C7816_ANACK_MASK)|         \
/* .....    */ CLR(UART_C7816_INIT_MASK)|CLR(UART_C7816_TTYPE_MASK)|          \
/* .....    */ CLR(UART_C7816_ISO_7816E_MASK),                                \
/* IE7816   */ CLR(UART_IE7816_WTE_MASK)|CLR(UART_IE7816_CWTE_MASK)|          \
/* ......   */ CLR(UART_IE7816_BWTE_MASK)|CLR(UART_IE7816_INITDE_MASK)|       \
/* ......   */ CLR(UART_IE7816_GTVE_MASK)|CLR(UART_IE7816_TXTE_MASK)|         \
/* ......   */ CLR(UART_IE7816_RXTE_MASK),                                    \
/* IS7816   */ CLR(UART_IS7816_WT_MASK)|CLR(UART_IS7816_CWT_MASK)|            \
/* ......   */ CLR(UART_IS7816_BWT_MASK)|CLR(UART_IS7816_INITD_MASK)|         \
/* ......   */ CLR(UART_IS7816_GTV_MASK)|CLR(UART_IS7816_TXT_MASK)|           \
/* ......   */ CLR(UART_IS7816_RXT_MASK),                                     \
/* WP7816T0 */ SET(UART_WP7816_T_TYPE0_WI(0x0a)),                             \
/* WP7816T1 */ SET(UART_WP7816_T_TYPE1_CWI(0x00))|                            \
/* ........ */ SET(UART_WP7816_T_TYPE1_BWI(0x0a)),                            \
/* WN7816   */ SET(UART_WN7816_GTN(0x00)),                                    \
/* WF7816   */ SET(UART_WF7816_GTFD(0x00)),                                   \
/* ET7816   */ SET(UART_ET7816_TXTHRESHOLD(0x00))|                            \
/* ......   */ SET(UART_ET7816_RXTHRESHOLD(0x00)),                            \
/* TL7816   */ SET(UART_TL7816_TLEN(0x00))                                    \
}

/***************************************************************************//*!
 * @brief UART configured to operate in polling mode. Receive data is not 
 *        inverted. Transmit data is inverted.
 * @param   brate     Baud rate.
 * @param   bclk      Buss clock in Hz.
 *****************************************************************************/
#define UART_MODULE_POLLMODE_TXINV_CONFIG(brate,bclk)                         \
(tUART){                                                                      \
/* BDH      */ CLR(UART_BDH_RXEDGIE_MASK)|((CALC_SBR(brate,bclk)>>8)&0x1f),   \
/* BDL      */ ((CALC_SBR(brate,bclk)>>0)&0xff),                              \
/* C1       */ CLR(UART_C1_LOOPS_MASK)|CLR(UART_C1_RSRC_MASK)|                \
/* ..       */ CLR(UART_C1_M_MASK)|CLR(UART_C1_WAKE_MASK)|                    \
/* ..       */ CLR(UART_C1_ILT_MASK)|CLR(UART_C1_PE_MASK)|                    \
/* ..       */ CLR(UART_C1_PT_MASK),                                          \
/* C2       */ CLR(UART_C2_TIE_MASK)|CLR(UART_C2_TCIE_MASK)|                  \
/* ..       */ CLR(UART_C2_RIE_MASK)|CLR(UART_C2_ILIE_MASK)|                  \
/* ..       */ SET(UART_C2_TE_MASK)|SET(UART_C2_RE_MASK)|                     \
/* ..       */ CLR(UART_C2_RWU_MASK)|CLR(UART_C2_SBK_MASK),                   \
/* S2       */ CLR(UART_S2_RXEDGIF_MASK)|CLR(UART_S2_MSBF_MASK)|              \
/* ..       */ CLR(UART_S2_RXINV_MASK)|CLR(UART_S2_RWUID_MASK)|               \
/* ..       */ CLR(UART_S2_BRK13_MASK),                                       \
/* C3       */ CLR(UART_C3_T8_MASK)|CLR(UART_C3_TXDIR_MASK)|                  \
/* ..       */ SET(UART_C3_TXINV_MASK)|CLR(UART_C3_ORIE_MASK)|                \
/* ..       */ CLR(UART_C3_NEIE_MASK)|CLR(UART_C3_FEIE_MASK)|                 \
/* ..       */ CLR(UART_C3_PEIE_MASK),                                        \
/* MA1      */ SET(UART_MA1_MA(0x00)),                                        \
/* MA2      */ SET(UART_MA2_MA(0x00)),                                        \
/* C4       */ CLR(UART_C4_MAEN1_MASK)|CLR(UART_C4_MAEN2_MASK)|               \
/* ..       */ CLR(UART_C4_M10_MASK)|((CALC_BRFA(brate,bclk)>>0)&0x1f),       \
/* C5       */ CLR(UART_C5_TDMAS_MASK)|CLR(UART_C5_RDMAS_MASK),               \
/* C7816    */ CLR(UART_C7816_ONACK_MASK)|CLR(UART_C7816_ANACK_MASK)|         \
/* .....    */ CLR(UART_C7816_INIT_MASK)|CLR(UART_C7816_TTYPE_MASK)|          \
/* .....    */ CLR(UART_C7816_ISO_7816E_MASK),                                \
/* IE7816   */ CLR(UART_IE7816_WTE_MASK)|CLR(UART_IE7816_CWTE_MASK)|          \
/* ......   */ CLR(UART_IE7816_BWTE_MASK)|CLR(UART_IE7816_INITDE_MASK)|       \
/* ......   */ CLR(UART_IE7816_GTVE_MASK)|CLR(UART_IE7816_TXTE_MASK)|         \
/* ......   */ CLR(UART_IE7816_RXTE_MASK),                                    \
/* IS7816   */ CLR(UART_IS7816_WT_MASK)|CLR(UART_IS7816_CWT_MASK)|            \
/* ......   */ CLR(UART_IS7816_BWT_MASK)|CLR(UART_IS7816_INITD_MASK)|         \
/* ......   */ CLR(UART_IS7816_GTV_MASK)|CLR(UART_IS7816_TXT_MASK)|           \
/* ......   */ CLR(UART_IS7816_RXT_MASK),                                     \
/* WP7816T0 */ SET(UART_WP7816_T_TYPE0_WI(0x0a)),                             \
/* WP7816T1 */ SET(UART_WP7816_T_TYPE1_CWI(0x00))|                            \
/* ........ */ SET(UART_WP7816_T_TYPE1_BWI(0x0a)),                            \
/* WN7816   */ SET(UART_WN7816_GTN(0x00)),                                    \
/* WF7816   */ SET(UART_WF7816_GTFD(0x00)),                                   \
/* ET7816   */ SET(UART_ET7816_TXTHRESHOLD(0x00))|                            \
/* ......   */ SET(UART_ET7816_RXTHRESHOLD(0x00)),                            \
/* TL7816   */ SET(UART_TL7816_TLEN(0x00))                                    \
}

/***************************************************************************//*!
 * @brief UART configured to operate in interrupt mode. Receive and transmit data 
 *        is not inverted.
 * @param   brate     Baud rate.
 * @param   bclk      Buss clock in Hz.
 *****************************************************************************/
#define UART_MODULE_INTRMODE_CONFIG(brate,bclk)                               \
(tUART){                                                                      \
/* BDH      */ CLR(UART_BDH_RXEDGIE_MASK)|((CALC_SBR(brate,bclk)>>8)&0x1f),   \
/* BDL      */ ((CALC_SBR(brate,bclk)>>0)&0xff),                              \
/* C1       */ CLR(UART_C1_LOOPS_MASK)|CLR(UART_C1_RSRC_MASK)|                \
/* ..       */ CLR(UART_C1_M_MASK)|CLR(UART_C1_WAKE_MASK)|                    \
/* ..       */ CLR(UART_C1_ILT_MASK)|CLR(UART_C1_PE_MASK)|                    \
/* ..       */ CLR(UART_C1_PT_MASK),                                          \
/* C2       */ CLR(UART_C2_TIE_MASK)|CLR(UART_C2_TCIE_MASK)|                  \
/* ..       */ SET(UART_C2_RIE_MASK)|CLR(UART_C2_ILIE_MASK)|                  \
/* ..       */ SET(UART_C2_TE_MASK)|SET(UART_C2_RE_MASK)|                     \
/* ..       */ CLR(UART_C2_RWU_MASK)|CLR(UART_C2_SBK_MASK),                   \
/* S2       */ CLR(UART_S2_RXEDGIF_MASK)|CLR(UART_S2_MSBF_MASK)|              \
/* ..       */ CLR(UART_S2_RXINV_MASK)|CLR(UART_S2_RWUID_MASK)|               \
/* ..       */ CLR(UART_S2_BRK13_MASK),                                       \
/* C3       */ CLR(UART_C3_T8_MASK)|CLR(UART_C3_TXDIR_MASK)|                  \
/* ..       */ CLR(UART_C3_TXINV_MASK)|CLR(UART_C3_ORIE_MASK)|                \
/* ..       */ CLR(UART_C3_NEIE_MASK)|CLR(UART_C3_FEIE_MASK)|                 \
/* ..       */ CLR(UART_C3_PEIE_MASK),                                        \
/* MA1      */ SET(UART_MA1_MA(0x00)),                                        \
/* MA2      */ SET(UART_MA2_MA(0x00)),                                        \
/* C4       */ CLR(UART_C4_MAEN1_MASK)|CLR(UART_C4_MAEN2_MASK)|               \
/* ..       */ CLR(UART_C4_M10_MASK)|((CALC_BRFA(brate,bclk)>>0)&0x1f),       \
/* C5       */ CLR(UART_C5_TDMAS_MASK)|CLR(UART_C5_RDMAS_MASK),               \
/* C7816    */ CLR(UART_C7816_ONACK_MASK)|CLR(UART_C7816_ANACK_MASK)|         \
/* .....    */ CLR(UART_C7816_INIT_MASK)|CLR(UART_C7816_TTYPE_MASK)|          \
/* .....    */ CLR(UART_C7816_ISO_7816E_MASK),                                \
/* IE7816   */ CLR(UART_IE7816_WTE_MASK)|CLR(UART_IE7816_CWTE_MASK)|          \
/* ......   */ CLR(UART_IE7816_BWTE_MASK)|CLR(UART_IE7816_INITDE_MASK)|       \
/* ......   */ CLR(UART_IE7816_GTVE_MASK)|CLR(UART_IE7816_TXTE_MASK)|         \
/* ......   */ CLR(UART_IE7816_RXTE_MASK),                                    \
/* IS7816   */ CLR(UART_IS7816_WT_MASK)|CLR(UART_IS7816_CWT_MASK)|            \
/* ......   */ CLR(UART_IS7816_BWT_MASK)|CLR(UART_IS7816_INITD_MASK)|         \
/* ......   */ CLR(UART_IS7816_GTV_MASK)|CLR(UART_IS7816_TXT_MASK)|           \
/* ......   */ CLR(UART_IS7816_RXT_MASK),                                     \
/* WP7816T0 */ SET(UART_WP7816_T_TYPE0_WI(0x0a)),                             \
/* WP7816T1 */ SET(UART_WP7816_T_TYPE1_CWI(0x00))|                            \
/* ........ */ SET(UART_WP7816_T_TYPE1_BWI(0x0a)),                            \
/* WN7816   */ SET(UART_WN7816_GTN(0x00)),                                    \
/* WF7816   */ SET(UART_WF7816_GTFD(0x00)),                                   \
/* ET7816   */ SET(UART_ET7816_TXTHRESHOLD(0x00))|                            \
/* ......   */ SET(UART_ET7816_RXTHRESHOLD(0x00)),                            \
/* TL7816   */ SET(UART_TL7816_TLEN(0x00))                                    \
}

/***************************************************************************//*!
 * @brief UART configured to operate in interrupt mode. Receive data is not 
 *        inverted. Transmit data is inverted.
 * @param   brate     Baud rate.
 * @param   bclk      Buss clock in Hz.
 *****************************************************************************/
#define UART_MODULE_INTRMODE_TXINV_CONFIG(brate,bclk)                         \
(tUART){                                                                      \
/* BDH      */ CLR(UART_BDH_RXEDGIE_MASK)|((CALC_SBR(brate,bclk)>>8)&0x1f),   \
/* BDL      */ ((CALC_SBR(brate,bclk)>>0)&0xff),                              \
/* C1       */ CLR(UART_C1_LOOPS_MASK)|CLR(UART_C1_RSRC_MASK)|                \
/* ..       */ CLR(UART_C1_M_MASK)|CLR(UART_C1_WAKE_MASK)|                    \
/* ..       */ CLR(UART_C1_ILT_MASK)|CLR(UART_C1_PE_MASK)|                    \
/* ..       */ CLR(UART_C1_PT_MASK),                                          \
/* C2       */ CLR(UART_C2_TIE_MASK)|CLR(UART_C2_TCIE_MASK)|                  \
/* ..       */ SET(UART_C2_RIE_MASK)|CLR(UART_C2_ILIE_MASK)|                  \
/* ..       */ SET(UART_C2_TE_MASK)|SET(UART_C2_RE_MASK)|                     \
/* ..       */ CLR(UART_C2_RWU_MASK)|CLR(UART_C2_SBK_MASK),                   \
/* S2       */ CLR(UART_S2_RXEDGIF_MASK)|CLR(UART_S2_MSBF_MASK)|              \
/* ..       */ CLR(UART_S2_RXINV_MASK)|CLR(UART_S2_RWUID_MASK)|               \
/* ..       */ CLR(UART_S2_BRK13_MASK),                                       \
/* C3       */ CLR(UART_C3_T8_MASK)|CLR(UART_C3_TXDIR_MASK)|                  \
/* ..       */ SET(UART_C3_TXINV_MASK)|CLR(UART_C3_ORIE_MASK)|                \
/* ..       */ CLR(UART_C3_NEIE_MASK)|CLR(UART_C3_FEIE_MASK)|                 \
/* ..       */ CLR(UART_C3_PEIE_MASK),                                        \
/* MA1      */ SET(UART_MA1_MA(0x00)),                                        \
/* MA2      */ SET(UART_MA2_MA(0x00)),                                        \
/* C4       */ CLR(UART_C4_MAEN1_MASK)|CLR(UART_C4_MAEN2_MASK)|               \
/* ..       */ CLR(UART_C4_M10_MASK)|((CALC_BRFA(brate,bclk)>>0)&0x1f),       \
/* C5       */ CLR(UART_C5_TDMAS_MASK)|CLR(UART_C5_RDMAS_MASK),               \
/* C7816    */ CLR(UART_C7816_ONACK_MASK)|CLR(UART_C7816_ANACK_MASK)|         \
/* .....    */ CLR(UART_C7816_INIT_MASK)|CLR(UART_C7816_TTYPE_MASK)|          \
/* .....    */ CLR(UART_C7816_ISO_7816E_MASK),                                \
/* IE7816   */ CLR(UART_IE7816_WTE_MASK)|CLR(UART_IE7816_CWTE_MASK)|          \
/* ......   */ CLR(UART_IE7816_BWTE_MASK)|CLR(UART_IE7816_INITDE_MASK)|       \
/* ......   */ CLR(UART_IE7816_GTVE_MASK)|CLR(UART_IE7816_TXTE_MASK)|         \
/* ......   */ CLR(UART_IE7816_RXTE_MASK),                                    \
/* IS7816   */ CLR(UART_IS7816_WT_MASK)|CLR(UART_IS7816_CWT_MASK)|            \
/* ......   */ CLR(UART_IS7816_BWT_MASK)|CLR(UART_IS7816_INITD_MASK)|         \
/* ......   */ CLR(UART_IS7816_GTV_MASK)|CLR(UART_IS7816_TXT_MASK)|           \
/* ......   */ CLR(UART_IS7816_RXT_MASK),                                     \
/* WP7816T0 */ SET(UART_WP7816_T_TYPE0_WI(0x0a)),                             \
/* WP7816T1 */ SET(UART_WP7816_T_TYPE1_CWI(0x00))|                            \
/* ........ */ SET(UART_WP7816_T_TYPE1_BWI(0x0a)),                            \
/* WN7816   */ SET(UART_WN7816_GTN(0x00)),                                    \
/* WF7816   */ SET(UART_WF7816_GTFD(0x00)),                                   \
/* ET7816   */ SET(UART_ET7816_TXTHRESHOLD(0x00))|                            \
/* ......   */ SET(UART_ET7816_RXTHRESHOLD(0x00)),                            \
/* TL7816   */ SET(UART_TL7816_TLEN(0x00))                                    \
}
/*! @} End of uart_config                                                     */

/******************************************************************************
* UART callback registered by UART_InstallCallback() function
*
*//*! @addtogroup uart_callback
* @{
*******************************************************************************/
/*! @brief UART_CALLBACK source declaration                                   */
typedef enum 
{
    UART0_CALLBACK=1, ///< UART0 module
    UART1_CALLBACK=2, ///< UART1 module
    UART2_CALLBACK=4, ///< UART2 module
    UART3_CALLBACK=8  ///< UART3 module
} UART_CALLBACK_SRC;

/*! @brief UART_CALLBACK type declaration                                     */
typedef enum 
{
    TX_CALLBACK =1,   ///< Transmit Data Register Empty Flag (TDRE), 
                      //   Transmit Complete (TC)
    RX_CALLBACK =2,   ///< Receive Data Register Full (RDRF)
    ER_CALLBACK =4,   ///< Receiver Overrun (OR), Framing Error (FE), 
                      //   Noise Flag (NF), Parity Error Flag (PF)
    EDG_CALLBACK=8    ///< RxD Input Active Edge Interrupt Flag (RXEDGIF)
} UART_CALLBACK_TYPE;

/*! UART_CALLBACK function declaration                                        */
typedef void (*UART_CALLBACK)(UART_CALLBACK_SRC module, UART_CALLBACK_TYPE type, 
                              int8 status);
/*! @} End of uart_callback                                                   */                              

/******************************************************************************
* UART function and macro definitions
*
*//*! @addtogroup uart_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Disables transmitter interrupt or DMA transfer.
 * @details This macro disables transmitter interrupt or DMA transfer for 
 *          specified module.
 * @param   module    UART0,UART1,UART2,UART3
 * @note    Implemented as an inline macro.
 * @see     @ref UART_TxIsrEnable, @ref UART_TxCompIsrDisable, 
 *          @ref UART_TxCompIsrEnable
 ******************************************************************************/
#define UART_TxIsrDisable(module)   ioclrb (module##_C2,UART_C2_TIE_MASK)

/***************************************************************************//*!
 * @brief   Enables transmitter interrupt or DMA transfer.
 * @details This macro enables transmitter interrupt or DMA transfer for 
 *          specified module.
 * @param   module    UART0,UART1,UART2,UART3
 * @note    Implemented as an inline macro.
 * @see     @ref UART_TxIsrDisable, @ref UART_TxCompIsrDisable, 
 *          @ref UART_TxCompIsrEnable
 ******************************************************************************/
#define UART_TxIsrEnable(module)    iosetb (module##_C2,UART_C2_TIE_MASK)

/***************************************************************************//*!
 * @brief   Disables transmission complete interrupt.
 * @details This macro disables transmission complete interrupt request for 
 *          specified module.
 * @param   module    UART0,UART1,UART2,UART3
 * @note    Implemented as an inline macro.
 * @see     @ref UART_TxCompIsrEnable, @ref UART_TxIsrDisable, 
 *          @ref UART_TxIsrEnable
 ******************************************************************************/
#define UART_TxCompIsrDisable(module) ioclrb (module##_C2,UART_C2_TCIE_MASK)

/***************************************************************************//*!
 * @brief   Enables transmission complete interrupt.
 * @details This macro enables transmission complete interrupt request for 
 *          specified module.
 * @param   module    UART0,UART1,UART2,UART3
 * @note    Implemented as an inline macro.
 * @see     @ref UART_TxCompIsrDisable, @ref UART_TxIsrDisable, 
 *          @ref UART_TxIsrEnable
 ******************************************************************************/
#define UART_TxCompIsrEnable(module)  iosetb (module##_C2,UART_C2_TCIE_MASK)

/***************************************************************************//*!
 * @brief   Disables receiver full interrupt or DMA transfer.
 * @details This macro disables receiver full interrupt or DMA transfer for 
 *          specified module.
 * @param   module    UART0,UART1,UART2,UART3
 * @note    Implemented as an inline macro.
 * @see     @ref UART_RxIsrEnable
 ******************************************************************************/
#define UART_RxIsrDisable(module)   ioclrb (module##_C2,UART_C2_RIE_MASK)

/***************************************************************************//*!
 * @brief   Enables rreceiver full interrupt or DMA transfer.
 * @details This macro enables receiver full interrupt or DMA transfer for 
 *          specified module.
 * @param   module    UART0,UART1,UART2,UART3
 * @note    Implemented as an inline macro.
 * @see     @ref UART_RxIsrDisable 
 ******************************************************************************/
#define UART_RxIsrEnable(module)    iosetb (module##_C2,UART_C2_RIE_MASK) 

/***************************************************************************//*!
 * @brief   Enables RxD input active edge interrupt.
 * @details This macro enables RxD input active edge interrupt for specified 
 *          module.
 * @param   module    UART0,UART1,UART2,UART3
 * @note    Implemented as an inline macro.
 * @see     @ref UART_EdgIsrDisable, @ref UART_EdgIsrClr, 
 *          @ref UART_EdgIsrClrEnable
 ******************************************************************************/
#define UART_EdgIsrEnable(module)   iosetb (module##_BDH,UART_BDH_RXEDGIE_MASK)

/***************************************************************************//*!
 * @brief   Clears RxD input active edge interrupt flag.
 * @details This macro clears RxD input active edge interrupt flag for specified 
 *          module.
 * @param   module    UART0,UART1,UART2,UART3
 * @note    Implemented as an inline macro.
 * @see     @ref UART_EdgIsrDisable, @ref UART_EdgIsrEnable, 
 *          @ref UART_EdgIsrClrEnable 
 ******************************************************************************/
#define UART_EdgIsrClr(module)      iosetb (module##_S2,UART_S2_RXEDGIF_MASK)

/***************************************************************************//*!
 * @brief   Clears RxD input active edge interrupt flag and enables RxD input 
 *          active edge interrupt.
 * @details This macro clears RxD input active edge interrupt flag and it also 
 *          enables RxD input active edge interrupt for specified module.
 * @param   module    UART0,UART1,UART2,UART3
 * @note    Implemented as an inline macro.
 * @see     @ref UART_EdgIsrDisable, @ref UART_EdgIsrEnable, @ref UART_EdgIsrClr 
 ******************************************************************************/
#define UART_EdgIsrClrEnable(module)                                          \
                                    iosetb (module##_S2,UART_S2_RXEDGIF_MASK);\
                                    iosetb (module##_BDH,UART_BDH_RXEDGIE_MASK)  
                                      
/***************************************************************************//*!
 * @brief   Disables RxD input active edge interrupt.
 * @details This macro disables RxD input active edge interrupt for specified 
 *          module.
 * @param   module    UART0,UART1,UART2,UART3
 * @note    Implemented as an inline macro.
 * @see     @ref UART_EdgIsrEnable, @ref UART_EdgIsrClrEnable
 ******************************************************************************/
#define UART_EdgIsrDisable(module) ioclrb (module##_BDH,UART_BDH_RXEDGIE_MASK)

/***************************************************************************//*!
 * @brief   Reads and returns transmitter state.
 * @details This macro returns transmitter state of the specified module.
 * @param   module    UART0,UART1,UART2,UART3
 * @return  FALSE     transmitter active\n 
 * @return  TRUE      transmitter idle\n
 * @note    Implemented as an inline macro.
 * @see     @ref UART_RxFull
 ******************************************************************************/
#define UART_TxIdle(module)       ((module##_S1 & UART_S1_TC_MASK)?TRUE:FALSE)

/***************************************************************************//*!
 * @brief   Reads and returns receiver state.
 * @details This macro returns receiver state of the specified module.
 * @param   module    UART0,UART1,UART2,UART3
 * @return  FALSE     receiver busy\n 
 * @return  TRUE      character ready\n
 * @note    Implemented as an inline macro.
 * @see     @ref UART_TxIdle
 ******************************************************************************/
#define UART_RxFull(module)       ((module##_S1 & UART_S1_RDRF_MASK)?TRUE:FALSE)

/***************************************************************************//*!
 * @brief   Reads and returns received character.
 * @details This macro reads character received by the specified module.
 * @param   module      UART0,UART1,UART2,UART3
 * @return  @ref int8   received character.
 * @note    Implemented as an inline macro.
 * @warning Doesn't wait for a new byte (until RDRF is set). Insert   
 *          <c>while(!UART_RxFull(UART?));</c> statement prior this macro 
 *          to ensure new byte is returned.
 * @see     @ref UART_PutChar, @ref UART_PutStr, @ref UART_Rd, @ref UART_Wr
 ******************************************************************************/
#define UART_GetChar(module)      module##_D

/***************************************************************************//*!
 * @brief   Sends character.
 * @details This macro sends character to the specified module.
 * @param   module    UART0,UART1,UART2,UART3
 * @param   c         character
 * @note    Implemented as an inline macro.
 * @warning Doesn't wait until character is sent. Insert  
 *          <c>while(!UART_TxIdle(UART?));</c> statement prior this macro to 
 *          ensure transmitter is idle and able to send new character.
 * @see     @ref UART_GetChar, @ref UART_PutStr, @ref UART_Rd, @ref UART_Wr
 ******************************************************************************/
#define UART_PutChar(module,c)    module##_D = (c)

/***************************************************************************//*!
 * @brief   Sends string.
 * @details This macro sends string to the specified module.
 * @param   module    UART0,UART1,UART2,UART3
 * @param   str       String terminated by null character
 * @note    Implemented as an inline macro.
 * @warning Doesn't terminate until NULL character is read.
 * @see     @ref UART_GetChar, @ref UART_PutChar, @ref UART_Rd, @ref UART_Wr
 ******************************************************************************/
#define UART_PutStr(module,str)     {                                         \
                                      register uint16 i=0;                    \
                                      while(str[i] != 0)                      \
                                      {                                       \
                                        while (!UART_TxIdle(module));         \
                                        UART_PutChar(module,str[i]);          \
                                        i++;                                  \
                                      }                                       \
                                    } 

/***************************************************************************//*!
 * @brief   Reads <c>count</c> bytes and stores them in a <c>buffer</c>.
 * @details This macro reads <c>count</c> bytes from specified module and 
 *          stores them in <c>buffer</c>. Returns when <c>count</c> bytes have 
 *          been read.
 * @param   module    UART0,UART1,UART2,UART3
 * @param   buffer    @ref uint8 [] specifies the variable in which to store 
 *                    the data that was read from the selected module.
 * @param   count     specifies the number of bytes to read from the selected 
 *                    module.
 * @note    Implemented as an inline macro.
 * @warning Doesn't terminate until <c>count</c> bytes are read.
 * @see     @ref UART_GetChar, @ref UART_PutChar, @ref UART_PutStr, @ref UART_Wr
 ******************************************************************************/
#define UART_Rd(module,buffer,count)                                          \
{                                                                             \
    register uint16 i=0;                                                      \
    while (i < count)                                                         \
    {                                                                         \
      while (!UART_RxFull(module)); /* wait until new byte is available   */  \
      buffer[i++]=module##_D;       /* read and store new byte            */  \
    }                                                                         \
}

/***************************************************************************//*!
 * @brief   Writes <c>count</c> bytes from <c>buffer</c>.
 * @details This macro writes <c>count</c> bytes from <c>buffer</c> to the 
 *          specified module. Returns when <c>count</c> bytes are written.
 * @param   module    UART0,UART1,UART2,UART3
 * @param   buffer    @ref uint8 [] specifies the variable from which to read 
 *                    the data that will be written to the selected module.
 * @param   count     specifies the number of bytes to write to the selected 
 *                    module.
 * @note    Implemented as an inline macro.
 * @see     @ref UART_GetChar, @ref UART_PutChar, @ref UART_PutStr, @ref UART_Rd
 ******************************************************************************/
#define UART_Wr(module,buffer,count)                                          \
{                                                                             \
    register uint16 i=0;                                                      \
    while (i < count)                                                         \
    {                                                                         \
      while(!UART_TxIdle(module));  /* wait until transmitter is idle     */  \
      module##_D = buffer[i++];     /* write new byte                     */  \
    }                                                                         \
    while(!UART_TxIdle(module));    /* wait until last character is sent  */  \
}

/***************************************************************************//*!
 * @brief   Enables/disables internal loop mode.
 * @details This macro enables/disables internal loop mode for the specified module.
 *          When loop mode is set, the RxD pin is disconnected from the UART and the 
 *          transmitter output is internally connected to the receiver input. The 
 *          transmitter and the receiver must be enabled to use the loop function.
 * @param   module    UART0,UART1,UART2,UART3
 * @param   ctrl      TRUE (loopback mode enabled, receiver input is internally 
 *                          connected to transmitter output)\n
 *                    FALSE (normal operation)\n
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define UART_LoopModeCtrl(module,ctrl)                                        \
{                                                                             \
  if (ctrl==TRUE)                                                             \
  {                                                                           \
    iosetb(module##_C1,UART_C1_LOOPS_MASK);                                   \
    ioclrb(module##_C1,UART_C1_RSRC_MASK);                                    \
  }                                                                           \
  else                                                                        \
    ioclrb(module##_C1,UART_C1_LOOPS_MASK);                                   \
}
                                      
/***************************************************************************//*!
 * @brief   Installs callback function.
 * @details This function installs callback function for interrupt vectors 
 *          of the UART modules.
 * @param   vector    UART0_UART1 (vector 35 - OR'ed Interrupt UART0 and UART1)\n
 *                    UART2_UART3 (vector 36 - OR'ed Interrupt UART0 and UART1)\n
 * @param   ip        @ref cm0plus_prilvl
 * @param   callback  Pointer to @ref UART_CALLBACK
 * @note    Implemented as a function call.
 * @warning UART0 and UART1 interrupt sources are OR'ed into single interrupt vector.
 *          UART2 and UART3 interrupt sources are OR'ed into single interrupt vector.
 *          Register interrupt handler for UART modules:
 *|Module |Appconfig.h                                                                  |
 *|:-----:|:----------------------------------------------------------------------------|
 *|UART0-1|<c>extern void UART0_UART1_isr(void);\n#define VECTOR_035 UART0_UART1_isr</c>|
 *|UART2-3|<c>extern void UART2_UART3_isr(void);\n#define VECTOR_036 UART2_UART3_isr</c>|
 * @see     @ref UART_Init
 ******************************************************************************/
#define UART_InstallCallback(vector,ip,callback)                              \
                                    UART_InstallCallback##vector(ip,callback)

/***************************************************************************//*!
 * @brief   UART initialization.
 * @details This function initializes selected UART module.
 * @param   module    UART0,UART1,UART2,UART3  
 * @param   cfg       Select one of the following @ref uart_config
 * @note    Implemented as a function call.
 * @see     @ref UART_InstallCallback
 ******************************************************************************/
#define UART_Init(module,cfg)       module##_Init(cfg)
/*! @} End of uart_macro                                                      */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/
extern void UART_InstallCallbackUART0_UART1 (uint8 ip, UART_CALLBACK pCallback);
extern void UART_InstallCallbackUART2_UART3 (uint8 ip, UART_CALLBACK pCallback);

extern void UART0_Init (tUART sci);
extern void UART1_Init (tUART sci);
extern void UART2_Init (tUART sci);
extern void UART3_Init (tUART sci);
#endif /* __UART_H */ 