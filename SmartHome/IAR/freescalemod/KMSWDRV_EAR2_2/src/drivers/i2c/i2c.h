/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      i2c.h
 * @version   1.0.2.0
 * @date      Dec-20-2011
 * @brief     I2C bus comtroller (I2C) driver header file.
 ******************************************************************************/
#ifndef __I2C_H
#define __I2C_H 
 
/******************************************************************************
 * definition configuration structure                                         *
 ******************************************************************************/
typedef struct { uint8 A1, F, C1, C2, FLT, RA, SMB, A2, SLTH, SLTL; } tI2C;

/******************************************************************************
* I2C Timer channel default configurations used by I2C_init() function
*
*//*! @addtogroup i2c_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   I2C setting:
 * @details Configures I2C module with following characteristics:
 *          - baud rate 100kBd / 48MHz, 
 *          - module enable after initialization, master mode, IRQ disable
 * @details The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 ******************************************************************************/
#define I2C_MODULE_100K_MASTER_IRQ_DI_CONFIG                                  \
(tI2C){                                                                       \
/* A1     */  CLR(I2C_A1_AD(0)),                                              \
/* F      */  SET(I2C_F_MULT(0))|SET(I2C_F_ICR(0x30)),                        \
/* C1     */  SET(I2C_C1_IICEN_MASK)|CLR(I2C_C1_IICIE_MASK)|                  \
/* ..     */  CLR(I2C_C1_MST_MASK)|CLR(I2C_C1_TX_MASK)|                       \
/* ..     */  CLR(I2C_C1_TXAK_MASK)|CLR(I2C_C1_RSTA_MASK)|                    \
/* ..     */  CLR(I2C_C1_WUEN_MASK)|CLR(I2C_C1_DMAEN_MASK),                   \
/* C2     */  CLR(I2C_C2_GCAEN_MASK)|CLR(I2C_C2_ADEXT_MASK)|                  \
/* ..     */  CLR(I2C_C2_HDRS_MASK)|CLR(I2C_C2_SBRC_MASK)|                    \
/* ..     */  CLR(I2C_C2_RMEN_MASK)|CLR(I2C_C2_AD(0)),                        \
/* FLT    */  CLR(I2C_FLT_FLT(0)),                                            \
/* RA     */  CLR(I2C_RA_RAD(0)),                                             \
/* SMB    */  CLR(I2C_SMB_FACK_MASK)|CLR(I2C_SMB_ALERTEN_MASK)|               \
/* ...    */  CLR(I2C_SMB_SIICAEN_MASK)|CLR(I2C_SMB_TCKSEL_MASK)|             \
/* ...    */  CLR(I2C_SMB_SLTF_MASK)|CLR(I2C_SMB_SHTF2_MASK)|                 \
/* ...    */  CLR(I2C_SMB_SHTF2IE_MASK),                                      \
/* A2     */  CLR(I2C_A2_SAD(0)),                                             \
/* SLTH   */  CLR(I2C_SLTH_SSLT(0)),                                          \
/* SLTL   */  CLR(I2C_SLTL_SSLT(0)),                                          \
}

/***************************************************************************//*!
 * @brief   I2C setting:
 * @details Configures I2C module with following characteristics:
 *          - baud rate 100kBd / 48MHz, 
 *          - module enable after initialization, master mode, IRQ enable
 * @details The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 ******************************************************************************/
#define I2C_MODULE_100K_MASTER_IRQ_EN_CONFIG                                  \
(tI2C){                                                                       \
/* A1     */  CLR(I2C_A1_AD(0)),                                              \
/* F      */  SET(I2C_F_MULT(0))|SET(I2C_F_ICR(0x30)),                        \
/* C1     */  SET(I2C_C1_IICEN_MASK)|SET(I2C_C1_IICIE_MASK)|                  \
/* ..     */  CLR(I2C_C1_MST_MASK)|CLR(I2C_C1_TX_MASK)|                       \
/* ..     */  CLR(I2C_C1_TXAK_MASK)|CLR(I2C_C1_RSTA_MASK)|                    \
/* ..     */  CLR(I2C_C1_WUEN_MASK)|CLR(I2C_C1_DMAEN_MASK),                   \
/* C2     */  CLR(I2C_C2_GCAEN_MASK)|CLR(I2C_C2_ADEXT_MASK)|                  \
/* ..     */  CLR(I2C_C2_HDRS_MASK)|CLR(I2C_C2_SBRC_MASK)|                    \
/* ..     */  CLR(I2C_C2_RMEN_MASK)|CLR(I2C_C2_AD(0)),                        \
/* FLT    */  CLR(I2C_FLT_FLT(0)),                                            \
/* RA     */  CLR(I2C_RA_RAD(0)),                                             \
/* SMB    */  CLR(I2C_SMB_FACK_MASK)|CLR(I2C_SMB_ALERTEN_MASK)|               \
/* ...    */  CLR(I2C_SMB_SIICAEN_MASK)|CLR(I2C_SMB_TCKSEL_MASK)|             \
/* ...    */  CLR(I2C_SMB_SLTF_MASK)|CLR(I2C_SMB_SHTF2_MASK)|                 \
/* ...    */  CLR(I2C_SMB_SHTF2IE_MASK),                                      \
/* A2     */  CLR(I2C_A2_SAD(0)),                                             \
/* SLTH   */  CLR(I2C_SLTH_SSLT(0)),                                          \
/* SLTL   */  CLR(I2C_SLTL_SSLT(0)),                                          \
}
/*! @} End of i2c_config                                                      */

/******************************************************************************
* I2C callback registered by I2C_InstallCallback() function
*
*//*! @addtogroup i2c_callback
* @{
*******************************************************************************/
/*! @brief I2C_CALLBACK type declaration                                      */
typedef enum 
{ 
  I2C0_CALLBACK=1,      ///< I2C0 interrupt
  I2C1_CALLBACK=2,      ///< I2C1 interrupt
} I2C_CALLBACK_TYPE;

/*! @brief I2C_CALLBACK function declaration                                  */
typedef void (*I2C_CALLBACK)( vuint32 status);
/*! @} End of i2c_callback                                                    */

/******************************************************************************
* I2C function and macro definitions
*
*//*! @addtogroup i2c_macro
* @{
*******************************************************************************/
#define I2C0_StartSignal         (I2C0_C1 |= I2C_C1_MST_MASK)
#define I2C1_StartSignal         (I2C1_C1 |= I2C_C1_MST_MASK)
/***************************************************************************//*!
 * @brief   Asserts Start condition on the bus.
 * @details This macro asserts Start condition on the bus.
 * @param   module    I2C0,I2C1
 * @note    Implemented as an inline macro.
 * @see     @ref I2C_StopSignal, @ref I2C_RepeatStartSignal 
 ******************************************************************************/
#define I2C_StartSignal(module)           module##_StartSignal

#define I2C0_StopSignal          (I2C0_C1 &= ~I2C_C1_MST_MASK)
#define I2C1_StopSignal          (I2C1_C1 &= ~I2C_C1_MST_MASK)
/***************************************************************************//*!
 * @brief   Asserts Stop condition on the bus.
 * @details This macro asserts Stop condition on the bus.
 * @param   module    I2C0,I2C1
 * @note    Implemented as an inline macro.
 * @see     @ref I2C_StartSignal
 ******************************************************************************/
#define I2C_StopSignal(module)             module##_StopSignal

#define I2C0_RepeatStartSignal    (I2C0_C1 |= I2C_C1_RSTA_MASK)
#define I2C1_RepeatStartSignal    (I2C1_C1 |= I2C_C1_RSTA_MASK)
/***************************************************************************//*!
 * @brief   Asserts Repeat Start condition on the bus.
 * @details This macro asserts Repeat Start condition on the bus.
 * @param   module    I2C0,I2C1
 * @note    Implemented as an inline macro.
 * @see     @ref I2C_StartSignal
 ******************************************************************************/
#define I2C_RepeatStartSignal(module)     module##_RepeatStartSignal

#define I2C0_WrData(data)         (I2C0_D = data)
#define I2C1_WrData(data)         (I2C1_D = data)
/***************************************************************************//*!
 * @brief   Writes data for transfer.
 * @details This macro writes data to Data register for transfer start.
 * @param   module    I2C0,I2C1
 * @param   data     @ref uint8 load register value 
 * @note    Implemented as an inline macro.
 * @see     @ref I2C_RdData
 ******************************************************************************/
#define I2C_WrData(module,data)           module##_WrData(data)

#define I2C0_RdData             (uint8)I2C0_D
#define I2C1_RdData             (uint8)I2C1_D
/***************************************************************************//*!
 * @brief   Return data from last transfer.
 * @details This macro returns last received data from Data register.
 * @param   module    I2C0,I2C1
 * @note    Implemented as an inline macro.
 * @see     @ref I2C_WrData
 ******************************************************************************/
#define I2C_RdData(module)                module##_RdData

#define I2C0_GetIsrFlag         ((I2C0_S & I2C_S_IICIF_MASK) ? TRUE:FALSE)
#define I2C1_GetIsrFlag         ((I2C1_S & I2C_S_IICIF_MASK) ? TRUE:FALSE)
/***************************************************************************//*!
 * @brief   Returns state of the interrupt flag.
 * @details This macro returns state of the interrupt flag.
 * @param   module    I2C0,I2C1
 * @return  TRUE    Interrupt pending\n
 * @return  FALSE   No interrupt pending\n   
 * @note    Implemented as an inline macro.
 * @see     @ref I2C_ClrIsrFlag
 ******************************************************************************/
#define I2C_GetIsrFlag(module)          module##_GetIsrFlag

#define I2C0_ClrIsrFlag (I2C0_S = (I2C0_S | I2C_S_IICIF_MASK) & ~I2C_S_ARBL_MASK)
#define I2C1_ClrIsrFlag (I2C1_S = (I2C1_S | I2C_S_IICIF_MASK) & ~I2C_S_ARBL_MASK)
/***************************************************************************//*!
 * @brief   Clears interrupt flag.
 * @details This macro clears interrupt flag.
 * @param   module    I2C0,I2C1
 * @note    Implemented as an inline macro.
 * @see     @ref I2C_GetIsrFlag
 ******************************************************************************/
#define I2C_ClrIsrFlag(module)          module##_ClrIsrFlag

#define I2C0_SetRxMode          (I2C0_C1 &= ~I2C_C1_TX_MASK)
#define I2C1_SetRxMode          (I2C1_C1 &= ~I2C_C1_TX_MASK)
/***************************************************************************//*!
 * @brief   Sets RX mode.
 * @details This macro sets RX mode.
 * @param   module    I2C0,I2C1
 * @note    Implemented as an inline macro.
 * @see     @ref I2C_SetTxMode
 ******************************************************************************/
#define I2C_SetRxMode(module)           module##_SetRxMode

#define I2C0_SetTxMode          (I2C0_C1 |= I2C_C1_TX_MASK)
#define I2C1_SetTxMode          (I2C1_C1 |= I2C_C1_TX_MASK)
/***************************************************************************//*!
 * @brief   Sets TX mode.
 * @details This macro sets TX mode.
 * @param   module    I2C0,I2C1
 * @note    Implemented as an inline macro.
 * @see     @ref I2C_SetRxMode
 ******************************************************************************/
#define I2C_SetTxMode(module)           module##_SetTxMode

#define I2C0_SetNackMode        (I2C0_C1 |= I2C_C1_TXAK_MASK)
#define I2C1_SetNackMode        (I2C1_C1 |= I2C_C1_TXAK_MASK)
/***************************************************************************//*!
 * @brief   Sets NACK mode.
 * @details This macro sets NACK mode.
 * @param   module    I2C0,I2C1
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define I2C_SetNackMode(module)         module##_SetNackMode

#define I2C0_SetADDR(addr)      (I2C0_A1 = addr)
#define I2C1_SetADDR(addr)      (I2C1_A1 = addr)
/***************************************************************************//*!
 * @brief   Sets 7-bit I2C address.
 * @details This macro sets 7-bit I2C address.
 * @param   module    I2C0,I2C1
 * @param   addr      7-bit address
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define I2C_SetADDR(module,addr)        module##_SetADDR(addr)

#define I2C0_GetRxakFlag          (I2C0_S & I2C_S_RXAK_MASK)
#define I2C1_GetRxakFlag          (I2C1_S & I2C_S_RXAK_MASK)
/***************************************************************************//*!
 * @brief   Returns RXAK flag.
 * @details This macro returns state of the RXAK flag of the Status register.
 * @param   module    I2C0,I2C1
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define I2C_GetRxakFlag(module)           module##_GetRxakFlag

#define I2C0_GetBusyFlag          (I2C0_S & I2C_S_BUSY_MASK)
#define I2C1_GetBusyFlag          (I2C1_S & I2C_S_BUSY_MASK)
/***************************************************************************//*!
 * @brief   Return BUSY flag.
 * @details This macro returns state of the BUSY flag of the Status register.
 * @param   module    I2C0,I2C1
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define I2C_GetBusyFlag(module)           module##_GetBusyFlag

#define I2C0_Reset                  (I2C0_C1 = I2C_C1_IICEN_MASK)
#define I2C1_Reset                  (I2C1_C1 = I2C_C1_IICEN_MASK)
/***************************************************************************//*!
 * @brief   Resets C1 register, but specified I2C module is still enabled.
 * @details This macro resets the specified I2C module while keeping it enabled.
 * @param   module    I2C0,I2C1
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define I2C_Reset(module)            module##_Reset

/* Function redefinition                                                      */
/***************************************************************************//*!
 * @brief   Installs callback function for vector 42.
 * @details This function is used to install callback function for interrupt 
 *          vector 42.
 * @param   ip        @ref cm0plus_prilvl
 * @param   callback  Pointer to @ref I2C_CALLBACK
 * @note    Implemented as a function call.
 * @warning I2C0 and I2C1 interrupt sources are OR'ed into single interrupt vector.
 *          Register interrupt handler for I2C modules:
 *|Module|Appconfig.h                                                              |
 *|:----:|:------------------------------------------------------------------------|
 *|I2Cx  |<c>extern void I2C0_I2C1_isr(void);\n#define VECTOR_042 I2C0_I2C1_isr</c>|
 * @see     @ref I2C_Init
 ******************************************************************************/
#define I2C_InstallCallback(ip,callback) I2C_InstallCallback (ip,callback)

/***************************************************************************//*!
 * @brief   I2C initialization.
 * @details This function initializes selected Timer channel of the 
 *          I2C bus (I2C) block.  
 * @param   module    I2C0,I2C1
 * @param   cfg       Select one of the following @ref i2c_config
 * @note    Implemented as a function call.
 * @see     @ref I2C_InstallCallback
 ******************************************************************************/
#define I2C_Init(module,cfg)             module##_Init(cfg)
/*! @} End of i2c_macro                                                       */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/    
extern void I2C_InstallCallback (uint8 ip, I2C_CALLBACK pCallback);
extern void I2C0_Init (tI2C i2c);
extern void I2C1_Init (tI2C i2c);

#endif /* __I2C_H */ 