/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      spi.h
 * @version   1.0.3.0
 * @date      Mar-12-2013
 * @brief     SPI driver header file.
******************************************************************************/
#ifndef __SPI_H
#define __SPI_H 

/******************************************************************************
 * definition configuration structures                                        *
 ******************************************************************************/
typedef struct { uint8 C1, C2, C3, BR, ML, MH; } tSPI;

/******************************************************************************
* SPI callback registered by SPI_Init() function
*
*//*! @addtogroup spi_callback
* @{
*******************************************************************************/
/*! @brief SPI_CALLBACK_SRC declaration                                       */
typedef enum 
{ 
    SPI0_CALLBACK=1,    ///< SPI0 interrupt
    SPI1_CALLBACK=2,    ///< SPI1 interrupt
} SPI_CALLBACK_SRC;

/*! @brief SPI_CALLBACK_TYPE declaration                                      */
typedef enum 
{
    TXSPI_CALLBACK=1,   ///< Transmit Complete (SPTEF)
    RXSPI_CALLBACK=2,   ///< Receive Data Register Full (SPRF)
    MATCH_CALLBACK=3,   ///< Received value matches MH:ML registers (SPMF)
    FAULT_CALLBACK=4    ///< Mode fault error (MODF), only if SS is disabled 
} SPI_CALLBACK_TYPE;

/*! @brief SPI_CALLBACK function declaration                                  */
typedef void (*SPI_CALLBACK)(SPI_CALLBACK_SRC module, SPI_CALLBACK_TYPE type);
/*! @} End of spi_callback                                                    */

/******************************************************************************
* SPI SS callback used in SPI_Init function
*
*//*! @addtogroup spi_ss_callback
* @{
*******************************************************************************/
/*! @brief SS_CONTROL declaration                                             */
typedef enum 
{ 
    SS_CLEAR=0,         ///< SS=0 
    SS_SET=1,           ///< SS=1
    SS_INIT=2,          ///< SS pin and port initialization
} SS_CONTROL;

/*! @brief SPI_SSCALLBACK function declaration                                */
typedef void (*SPI_SSCALLBACK)(SS_CONTROL ss_pin);
/*! @} End of spi_ss_callback                                                 */

/******************************************************************************
* SPI default configurations used by SPI_init() function
*
*//*! @addtogroup spi_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   SPI setting in 8bit IRQ mode
 * @details Baud rate is 1/24 of BUSCLK, 8-bit interrupt mode, module enabled 
 *          after initialization, master mode, SS is configured as GPIO 
 *          (must be driven manually!), no FIFO no DMA no MATCH functionality, 
 *          full-duplex mode.
 *          The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 * @note    SPI Tx interrupt is set by SPI_TxIsrEnable command.
 ******************************************************************************/
#define SPI_MODULE_DIV24_8B_IRQ_CONFIG                                         \
(tSPI){                                                                        \
/* C1   */ SET(SPI_C1_SPIE_MASK)|SET(SPI_C1_SPE_MASK)|CLR(SPI_C1_SPTIE_MASK)|  \
/* ..   */ SET(SPI_C1_MSTR_MASK)|CLR(SPI_C1_CPOL_MASK)|CLR(SPI_C1_CPHA_MASK)|  \
/* ..   */ CLR(SPI_C1_SSOE_MASK)|CLR(SPI_C1_LSBFE_MASK),                       \
/* C2   */ CLR(SPI_C2_SPMIE_MASK)|CLR(SPI_C2_SPIMODE_MASK)|                    \
/* ..   */ CLR(SPI_C2_TXDMAE_MASK)|CLR(SPI_C2_MODFEN_MASK)|                    \
/* ..   */ CLR(SPI_C2_RXDMAE_MASK)|CLR(SPI_C2_SPISWAI_MASK)|                   \
/* ..   */ CLR(SPI_C2_SPC0_MASK),                                              \
/* C3   */ 0,   /* C3 is not used for SPI0 module due to no FIFO support */    \
/* BR   */ SET(SPI_BR_SPPR(2))|SET(SPI_BR_SPR(2)),                             \
/* ML   */ 0,                                                                  \
/* MH   */ 0,                                                                  \
}

/***************************************************************************//*!
 * @brief   SPI setting in 8bit POLLING mode
 * @details Baud rate is 1/24 of BUSCLK, 8-bit polling mode, module enabled 
 *          after initialization, master mode, SS is configured as GPIO 
 *          (must be driven manually!), no FIFO no DMA no MATCH functionality, 
 *          full-duplex mode.
 *          The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 ******************************************************************************/
#define SPI_MODULE_DIV24_8B_POLL_CONFIG                                        \
(tSPI){                                                                        \
/* C1   */ CLR(SPI_C1_SPIE_MASK)|SET(SPI_C1_SPE_MASK)|CLR(SPI_C1_SPTIE_MASK)|  \
/* ..   */ SET(SPI_C1_MSTR_MASK)|CLR(SPI_C1_CPOL_MASK)|CLR(SPI_C1_CPHA_MASK)|  \
/* ..   */ CLR(SPI_C1_SSOE_MASK)|CLR(SPI_C1_LSBFE_MASK),                       \
/* C2   */ CLR(SPI_C2_SPMIE_MASK)|CLR(SPI_C2_SPIMODE_MASK)|                    \
/* ..   */ CLR(SPI_C2_TXDMAE_MASK)|CLR(SPI_C2_MODFEN_MASK)|                    \
/* ..   */ CLR(SPI_C2_RXDMAE_MASK)|CLR(SPI_C2_SPISWAI_MASK)|                   \
/* ..   */ CLR(SPI_C2_SPC0_MASK),                                              \
/* C3   */ 0,   /* C3 is not used for SPI0 module due to no FIFO support */    \
/* BR   */ SET(SPI_BR_SPPR(2))|SET(SPI_BR_SPR(2)),                             \
/* ML   */ 0,                                                                  \
/* MH   */ 0,                                                                  \
}

/***************************************************************************//*!
 * @brief   SPI setting in 8bit IRQ mode
 * @details Baud rate is 1/4 of BUSCLK, 8-bit interrupt mode, module enabled 
 *          after initialization, master mode, SS is configured as GPIO 
 *          (must be driven manually!), no FIFO no DMA no MATCH functionality, 
 *          full-duplex mode.
 *          The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 * @note    SPI Tx interrupt is set by SPI_TxIsrEnable command.
 *****************************************************************************/
#define SPI_MODULE_DIV4_8B_IRQ_CONFIG                                          \
(tSPI){                                                                        \
/* C1   */ SET(SPI_C1_SPIE_MASK)|SET(SPI_C1_SPE_MASK)|CLR(SPI_C1_SPTIE_MASK)|  \
/* ..   */ SET(SPI_C1_MSTR_MASK)|CLR(SPI_C1_CPOL_MASK)|CLR(SPI_C1_CPHA_MASK)|  \
/* ..   */ CLR(SPI_C1_SSOE_MASK)|CLR(SPI_C1_LSBFE_MASK),                       \
/* C2   */ CLR(SPI_C2_SPMIE_MASK)|CLR(SPI_C2_SPIMODE_MASK)|                    \
/* ..   */ CLR(SPI_C2_TXDMAE_MASK)|CLR(SPI_C2_MODFEN_MASK)|                    \
/* ..   */ CLR(SPI_C2_RXDMAE_MASK)|CLR(SPI_C2_SPISWAI_MASK)|                   \
/* ..   */ CLR(SPI_C2_SPC0_MASK),                                              \
/* C3   */ 0,   /* C3 is not used for SPI0 module due to no FIFO support */    \
/* BR   */ SET(SPI_BR_SPPR(0))|SET(SPI_BR_SPR(1)),                             \
/* ML   */ 0,                                                                  \
/* MH   */ 0,                                                                  \
}

/***************************************************************************//*!
 * @brief   SPI setting in 8bit POLLING mode
 * @details Baud rate is 1/4 of BUSCLK, 8-bit polling mode, module enabled 
 *          after initialization, master mode, SS is configured as GPIO 
 *          (must be driven manually!), no FIFO no DMA no MATCH functionality, 
 *          full-duplex mode.
 *          The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 ******************************************************************************/
#define SPI_MODULE_DIV4_8B_POLL_CONFIG                                         \
(tSPI){                                                                        \
/* C1   */ CLR(SPI_C1_SPIE_MASK)|SET(SPI_C1_SPE_MASK)|CLR(SPI_C1_SPTIE_MASK)|  \
/* ..   */ SET(SPI_C1_MSTR_MASK)|CLR(SPI_C1_CPOL_MASK)|CLR(SPI_C1_CPHA_MASK)|  \
/* ..   */ CLR(SPI_C1_SSOE_MASK)|CLR(SPI_C1_LSBFE_MASK),                       \
/* C2   */ CLR(SPI_C2_SPMIE_MASK)|CLR(SPI_C2_SPIMODE_MASK)|                    \
/* ..   */ CLR(SPI_C2_TXDMAE_MASK)|CLR(SPI_C2_MODFEN_MASK)|                    \
/* ..   */ CLR(SPI_C2_RXDMAE_MASK)|CLR(SPI_C2_SPISWAI_MASK)|                   \
/* ..   */ CLR(SPI_C2_SPC0_MASK),                                              \
/* C3   */ 0,   /* C3 is not used for SPI0 module due to no FIFO support */    \
/* BR   */ SET(SPI_BR_SPPR(0))|SET(SPI_BR_SPR(1)),                             \
/* ML   */ 0,                                                                  \
/* MH   */ 0,                                                                  \
}

/***************************************************************************//*!
 * @brief   SPI setting in 8bit IRQ mode
 * @details Baud rate is 1/12 of BUSCLK, 8-bit interrupt mode, module enabled 
 *          after initialization, master mode, SS is configured as GPIO 
 *          (must be driven manually!), no FIFO no DMA no MATCH functionality, 
 *          full-duplex mode.
 *          The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 * @note    SPI Tx interrupt is set by SPI_TxIsrEnable command.
 ******************************************************************************/
#define SPI_MODULE_DIV12_8B_IRQ_CONFIG                                         \
(tSPI){                                                                        \
/* C1   */ SET(SPI_C1_SPIE_MASK)|SET(SPI_C1_SPE_MASK)|CLR(SPI_C1_SPTIE_MASK)|  \
/* ..   */ SET(SPI_C1_MSTR_MASK)|CLR(SPI_C1_CPOL_MASK)|CLR(SPI_C1_CPHA_MASK)|  \
/* ..   */ CLR(SPI_C1_SSOE_MASK)|CLR(SPI_C1_LSBFE_MASK),                       \
/* C2   */ CLR(SPI_C2_SPMIE_MASK)|CLR(SPI_C2_SPIMODE_MASK)|                    \
/* ..   */ CLR(SPI_C2_TXDMAE_MASK)|CLR(SPI_C2_MODFEN_MASK)|                    \
/* ..   */ CLR(SPI_C2_RXDMAE_MASK)|CLR(SPI_C2_SPISWAI_MASK)|                   \
/* ..   */ CLR(SPI_C2_SPC0_MASK),                                              \
/* C3   */ 0,   /* C3 is not used for SPI0 module due to no FIFO support */    \
/* BR   */ SET(SPI_BR_SPPR(2))|SET(SPI_BR_SPR(1)),                             \
/* ML   */ 0,                                                                  \
/* MH   */ 0,                                                                  \
}

/***************************************************************************//*!
 * @brief   SPI setting in 8bit POLLING mode
 * @details Baud rate is 1/12 of BUSCLK, 8-bit polling mode, module enabled 
 *          after initialization, master mode, SS is configured as GPIO 
 *          (must be driven manually!), no FIFO no DMA no MATCH functionality, 
 *          full-duplex mode.
 *          The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 ******************************************************************************/
#define SPI_MODULE_DIV12_8B_POLL_CONFIG                                        \
(tSPI){                                                                        \
/* C1   */ CLR(SPI_C1_SPIE_MASK)|SET(SPI_C1_SPE_MASK)|CLR(SPI_C1_SPTIE_MASK)|  \
/* ..   */ SET(SPI_C1_MSTR_MASK)|CLR(SPI_C1_CPOL_MASK)|CLR(SPI_C1_CPHA_MASK)|  \
/* ..   */ CLR(SPI_C1_SSOE_MASK)|CLR(SPI_C1_LSBFE_MASK),                       \
/* C2   */ CLR(SPI_C2_SPMIE_MASK)|CLR(SPI_C2_SPIMODE_MASK)|                    \
/* ..   */ CLR(SPI_C2_TXDMAE_MASK)|CLR(SPI_C2_MODFEN_MASK)|                    \
/* ..   */ CLR(SPI_C2_RXDMAE_MASK)|CLR(SPI_C2_SPISWAI_MASK)|                   \
/* ..   */ CLR(SPI_C2_SPC0_MASK),                                              \
/* C3   */ 0,   /* C3 is not used for SPI0 module due to no FIFO support */    \
/* BR   */ SET(SPI_BR_SPPR(2))|SET(SPI_BR_SPR(1)),                             \
/* ML   */ 0,                                                                  \
/* MH   */ 0,                                                                  \
}

/***************************************************************************//*!
 * @brief   SPI setting in 8bit IRQ mode
 * @details Baud rate is 1/2 of BUSCLK, 8-bit interrupt mode, module enabled 
 *          after initialization, master mode, SS is configured as GPIO 
 *          (must be driven manually!), no FIFO no DMA no MATCH functionality, 
 *          full-duplex mode.
 *          The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 * @note    SPI Tx interrupt is set by SPI_TxIsrEnable command.
 ******************************************************************************/
#define SPI_MODULE_DIV2_8B_IRQ_CONFIG                                          \
(tSPI){                                                                        \
/* C1   */ SET(SPI_C1_SPIE_MASK)|SET(SPI_C1_SPE_MASK)|CLR(SPI_C1_SPTIE_MASK)|  \
/* ..   */ SET(SPI_C1_MSTR_MASK)|CLR(SPI_C1_CPOL_MASK)|CLR(SPI_C1_CPHA_MASK)|  \
/* ..   */ CLR(SPI_C1_SSOE_MASK)|CLR(SPI_C1_LSBFE_MASK),                       \
/* C2   */ CLR(SPI_C2_SPMIE_MASK)|CLR(SPI_C2_SPIMODE_MASK)|                    \
/* ..   */ CLR(SPI_C2_TXDMAE_MASK)|CLR(SPI_C2_MODFEN_MASK)|                    \
/* ..   */ CLR(SPI_C2_RXDMAE_MASK)|CLR(SPI_C2_SPISWAI_MASK)|                   \
/* ..   */ CLR(SPI_C2_SPC0_MASK),                                              \
/* C3   */ 0,   /* C3 is not used for SPI0 module due to no FIFO support */    \
/* BR   */ SET(SPI_BR_SPPR(0))|SET(SPI_BR_SPR(0)),                             \
/* ML   */ 0,                                                                  \
/* MH   */ 0,                                                                  \
}

/***************************************************************************//*!
 * @brief   SPI setting in 8bit POLLING mode
 * @details Baud rate is 1/2 of BUSCLK, 8-bit polling mode, module enabled after 
 *          initialization, master mode, SS is configured as GPIO 
 *          (must be driven manually!), no FIFO no DMA no MATCH functionality, 
 *          full-duplex mode.
 *          The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 ******************************************************************************/
#define SPI_MODULE_DIV2_8B_POLL_CONFIG                                         \
(tSPI){                                                                        \
/* C1   */ CLR(SPI_C1_SPIE_MASK)|SET(SPI_C1_SPE_MASK)|CLR(SPI_C1_SPTIE_MASK)|  \
/* ..   */ SET(SPI_C1_MSTR_MASK)|CLR(SPI_C1_CPOL_MASK)|CLR(SPI_C1_CPHA_MASK)|  \
/* ..   */ CLR(SPI_C1_SSOE_MASK)|CLR(SPI_C1_LSBFE_MASK),                       \
/* C2   */ CLR(SPI_C2_SPMIE_MASK)|CLR(SPI_C2_SPIMODE_MASK)|                    \
/* ..   */ CLR(SPI_C2_TXDMAE_MASK)|CLR(SPI_C2_MODFEN_MASK)|                    \
/* ..   */ CLR(SPI_C2_RXDMAE_MASK)|CLR(SPI_C2_SPISWAI_MASK)|                   \
/* ..   */ CLR(SPI_C2_SPC0_MASK),                                              \
/* C3   */ 0,   /* C3 is not used for SPI0 module due to no FIFO support */    \
/* BR   */ SET(SPI_BR_SPPR(0))|SET(SPI_BR_SPR(0)),                             \
/* ML   */ 0,                                                                  \
/* MH   */ 0,                                                                  \
}    

/***************************************************************************//*!
 * @brief   SPI setting in 16bit IRQ mode
 * @details Baud rate is 1/2 of BUSCLK, 8-bit interrupt mode, module enabled 
 *          after initialization, master mode, SS is configured as GPIO 
 *          (must be driven manually!), no FIFO no DMA no MATCH functionality, 
 *          full-duplex mode.
 *          The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 * @note    SPI Tx interrupt is set by SPI_TxIsrEnable command.
 ******************************************************************************/
#define SPI_MODULE_DIV2_16B_IRQ_CONFIG                                         \
(tSPI){                                                                        \
/* C1   */ SET(SPI_C1_SPIE_MASK)|SET(SPI_C1_SPE_MASK)|CLR(SPI_C1_SPTIE_MASK)|  \
/* ..   */ SET(SPI_C1_MSTR_MASK)|CLR(SPI_C1_CPOL_MASK)|CLR(SPI_C1_CPHA_MASK)|  \
/* ..   */ CLR(SPI_C1_SSOE_MASK)|CLR(SPI_C1_LSBFE_MASK),                       \
/* C2   */ CLR(SPI_C2_SPMIE_MASK)|SET(SPI_C2_SPIMODE_MASK)|                    \
/* ..   */ CLR(SPI_C2_TXDMAE_MASK)|CLR(SPI_C2_MODFEN_MASK)|                    \
/* ..   */ CLR(SPI_C2_RXDMAE_MASK)|CLR(SPI_C2_SPISWAI_MASK)|                   \
/* ..   */ CLR(SPI_C2_SPC0_MASK),                                              \
/* C3   */ 0,   /* C3 is not used for SPI0 module due to no FIFO support */    \
/* BR   */ SET(SPI_BR_SPPR(0))|SET(SPI_BR_SPR(0)),                             \
/* ML   */ 0,                                                                  \
/* MH   */ 0,                                                                  \
}    

/***************************************************************************//*!
 * @brief   SPI setting in 16bit POLLING mode
 * @details Baud rate is 1/2 of BUSCLK, 16-bit polling mode, module enabled 
 *          after initialization, master mode, SS is configured as GPIO 
 *          (must be driven manually!), no FIFO no DMA no MATCH functionality, 
 *          full-duplex mode.
 *          The initialization value of the configuration structure split by 
 *          peripheral registers is shown above.
 * @showinitializer
 ******************************************************************************/
#define SPI_MODULE_DIV2_16B_POLL_CONFIG                                        \
(tSPI){                                                                        \
/* C1   */ CLR(SPI_C1_SPIE_MASK)|SET(SPI_C1_SPE_MASK)|CLR(SPI_C1_SPTIE_MASK)|  \
/* ..   */ SET(SPI_C1_MSTR_MASK)|CLR(SPI_C1_CPOL_MASK)|CLR(SPI_C1_CPHA_MASK)|  \
/* ..   */ CLR(SPI_C1_SSOE_MASK)|CLR(SPI_C1_LSBFE_MASK),                       \
/* C2   */ CLR(SPI_C2_SPMIE_MASK)|SET(SPI_C2_SPIMODE_MASK)|                    \
/* ..   */ CLR(SPI_C2_TXDMAE_MASK)|CLR(SPI_C2_MODFEN_MASK)|                    \
/* ..   */ CLR(SPI_C2_RXDMAE_MASK)|CLR(SPI_C2_SPISWAI_MASK)|                   \
/* ..   */ CLR(SPI_C2_SPC0_MASK),                                              \
/* C3   */ 0,   /* C3 is not used for SPI0 module due to no FIFO support */    \
/* BR   */ SET(SPI_BR_SPPR(0))|SET(SPI_BR_SPR(0)),                             \
/* ML   */ 0,                                                                  \
/* MH   */ 0,                                                                  \
}    
/*! @} End of spi_config                                                      */

/******************************************************************************
* SPI macro definitions
*
*//*! @addtogroup spi_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Disables transmit interrupt request.
 * @details This macro disables transmit interrupt request for specified module.     
 * @param   module  SPI0,SPI1
 * @note    Implemented as an inline macro.
 * @see     @ref SPI_TxIsrEnable
 ******************************************************************************/
#define SPI_TxIsrDisable(module)  ioclrb (module##_C1,SPI_C1_SPTIE_MASK)

/***************************************************************************//*!
 * @brief   Enables transmit interrupt request.
 * @details This macro enables transmit interrupt request for specified module.     
 * @param   module  SPI0,SPI1
 * @note    Implemented as an inline macro.
 * @see     @ref SPI_TxIsrDisable
 ******************************************************************************/
#define SPI_TxIsrEnable(module)   iosetb (module##_C1,SPI_C1_SPTIE_MASK)

/***************************************************************************//*!
 * @brief   Returns 8/16bit data from the SPI receive buffer.
 * @details This macro returns 8/16bit data from the specified module.     
 * @param   module  SPI0|SPI1
 * @return  received data (8/16-bit).
 * @note    Implemented as an inline macro.
 * @see     @ref SPI_RxFull
 ******************************************************************************/
#define SPI_GetData(module)       ((uint16)module##_DL|(uint16)(module##_DH<<8))

/***************************************************************************//*!
 * @brief   Writes 8/16bit data to the SPI transmit buffer
 * @details This macro writes 8/16bit data to the specified module.     
 * @param   module  SPI0,SPI1
 * @param   c       Data to be sent (8/16-bit)
 * @note    Implemented as an inline macro.
 * @see     @ref SPI_TxCompl
 ******************************************************************************/
#define SPI_PutData(module,c)     {                                           \
                                    register uint16 tmp = (uint16)c;          \
                                    module##_DL = (uint8)(tmp & 0x00FF);      \
                                    module##_DH = (uint8)((tmp & 0xFF00)>>8); \
                                  }

/***************************************************************************//*!
 * @brief   Returns SPI receiver state.
 * @details This macro returns receiver state for the specified module.     
 * @param   module  SPI0|SPI1
 * @return  TRUE    SPI Rx buffer is empty\n
 * @return  FALSE   SPI Rx buffer is full (contains some data for reading)\n
 * @note    Implemented as an inline macro.
 * @see     @ref SPI_GetData
 ******************************************************************************/
#define SPI_RxFull(module)      ((module##_S & SPI_S_SPRF_MASK) ? TRUE : FALSE)

/***************************************************************************//*!
 * @brief   Returns SPI transmitter state.
 * @details This macro returns transmitter state for the specified module.     
 * @param   module  SPI0,SPI1
 * @return  TRUE    SPI Transfer is not complete\n
 * @return  FALSE   SPI Transfer is complete\n
 * @note    Implemented as an inline macro.
 * @see     @ref SPI_PutData
 ******************************************************************************/
#define SPI_TxCompl(module)     ((module##_S & SPI_S_SPTEF_MASK) ? TRUE : FALSE)
                               
/***************************************************************************//*!
 * @brief   SPI initialization.
 * @details This macro initializes SPI module. It also installs callback 
 *          function for interrupt vectors associated with initialized 
 *          SPI modules: vector 20(SPI0) and vector 21(SPI1).      
 * @param   module  SPI0,SPI1
 * @param   cfg     Select one of @ref spi_config 
 * @param   call2   Pointer to @ref spi_ss_callback where SS pin is manually 
 *                  driven  
 * @param   ip      @ref cm0plus_prilvl
 * @param   call1   Pointer to SPI @ref spi_callback
 * @note    Implemented as a function call.
 * @warning Register interrupt handler for SPI modules:
 *|Module|Appconfig.h                                                    |
 *|:----:|:--------------------------------------------------------------|
 *|SPI0  |<c>extern void spi0_isr(void);\n#define VECTOR_020 spi0_isr</c>|
 *|SPI1  |<c>extern void spi1_isr(void);\n#define VECTOR_021 spi1_isr</c>|
 ******************************************************************************/ 
#define SPI_Init(module,cfg,call2,ip,call1)   module##_Init(cfg,call2,ip,call1)

/***************************************************************************//*!
 * @brief   Transmit and receives one word.
 * @details This macro transmits and receives one word.     
 * @param   module  SPI0|SPI1
 * @param   data    Data for transferring
 * @return  received data (16-bit)
 * @note    Implemented as a function call.
 * @see     @ref SPI_TxRxByte, @ref SPI_CommWord, @ref SPI_CommByte 
 ******************************************************************************/
#define SPI_TxRxWord(module,data)   module##_TxRxWord(data)

/***************************************************************************//*!
 * @brief   Transmit and receives one byte.
 * @details This macro transmits and receives one byte.     
 * @param   module  SPI0|SPI1
 * @param   data    Data for transferring
 * @return  received data (8-bit)
 * @note    Implemented as a function call.
 * @see     @ref SPI_TxRxWord, @ref SPI_CommWord, @ref SPI_CommByte  
 ******************************************************************************/
#define SPI_TxRxByte(module,data)   module##_TxRxByte(data)

 /***************************************************************************//*!
 * @brief   Transmits and receives whole data packet (16-bit length).
 * @details This macro transmits and receives whole data packet.     
 * @param   module  SPI0|SPI1
 * @param   txd     Pointer to buffer where writing data is stored
 * @param   txcnt   Number of words for writing
 * @param   rxd     Pointer to buffer where reading data will be saved
 * @param   rxcnt   Number of words for reading
 * @note    Implemented as a function call.
 * @note    SS pin is driven manually (as a basic GPIO pin).
 * @see     @ref SPI_TxRxWord, @ref SPI_TxRxByte, @ref SPI_CommByte  
 ******************************************************************************/
#define SPI_CommWord(module,txd,txcnt,rxd,rxcnt)                              \
                                    module##_CommWord(txd,txcnt,rxd,rxcnt) 

 /***************************************************************************//*!
 * @brief   Transmits and receives whole data packet (8-bit length).
 * @details This macro transmits and receives whole data packet.     
 * @param   module  SPI0|SPI1
 * @param   txd     Pointer to buffer where writing data is stored
 * @param   txcnt   Number of bytes for writing
 * @param   rxd     Pointer to buffer where reading data will be saved
 * @param   rxcnt   Number of bytes for reading
 * @note    Implemented as a function call.
 * @note    SS pin is driven manually (as a basic GPIO pin).
 * @see     @ref SPI_TxRxWord, @ref SPI_TxRxByte, @ref SPI_CommWord  
 ******************************************************************************/
#define SPI_CommByte(module,txd,txcnt,rxd,rxcnt)                              \
                                    module##_CommByte(txd,txcnt,rxd,rxcnt) 
/*! @} End of spi_macro                                                       */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/    
extern void SPI0_Init (tSPI spi,SPI_SSCALLBACK pC2,uint8 ip,SPI_CALLBACK pC1);
extern void SPI1_Init (tSPI spi,SPI_SSCALLBACK pC2,uint8 ip,SPI_CALLBACK pC1);
extern void SPI0_CommWord(uint16 *txdata,uint16 txcnt,uint16 *rxdata, uint16 rxcnt);
extern void SPI1_CommWord(uint16 *txdata,uint16 txcnt,uint16 *rxdata, uint16 rxcnt);
extern void SPI0_CommByte(uint8 *txdata,uint16 txcnt,uint8 *rxdata, uint16 rxcnt);
extern void SPI1_CommByte(uint8 *txdata,uint16 txcnt,uint8 *rxdata, uint16 rxcnt);
extern uint16 SPI0_TxRxWord(uint16 txdata);
extern uint16 SPI1_TxRxWord(uint16 txdata);
extern uint8 SPI0_TxRxByte(uint8 txdata);
extern uint8 SPI1_TxRxByte(uint8 txdata);

#endif /* __SPI_H */ 