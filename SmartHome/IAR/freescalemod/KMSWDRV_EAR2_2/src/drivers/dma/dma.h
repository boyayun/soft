/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      dma.h
 * @version   1.0.3.0
 * @date      Feb-24-2013
 * @brief     Direct Memory Access (DMA) driver header file.
 ******************************************************************************/
#ifndef __DMA_H
#define __DMA_H

/******************************************************************************
 * definition configuration structures                                        *
 ******************************************************************************/
typedef struct { uint32 CHCFG, SAR, DAR, DSR, DCR; } tDMA_CH;

/**************************************************************************//*!
*//*! @addtogroup dma0_request_sources
* @{
* @details DMA channel 0 request sources used by some @ref dma_chan_config 
* configuration structures.
 *****************************************************************************/
#define DMA0_REQ_DISABLED (uint8)0x00   ///< DISABLED (default)
#define DMA0_REQ_SPI0_TX  (uint8)0x01   ///< SPI0 TX
#define DMA0_REQ_SPI0_RX  (uint8)0x02   ///< SPI0 RX
#define DMA0_REQ_UART0_TX (uint8)0x03   ///< UART0 TX
#define DMA0_REQ_UART0_RX (uint8)0x04   ///< UART0 RX
#define DMA0_REQ_I2C0     (uint8)0x05   ///< I2C0
#define DMA0_REQ_XBAR     (uint8)0x06   ///< XBAR
#define DMA0_REQ_UART3_TX (uint8)0x07   ///< UART3 TX
#define DMA0_REQ_UART3_RX (uint8)0x08   ///< UART3 RX
#define DMA0_REQ_AFE_CH0  (uint8)0x09   ///< AFE CH0
#define DMA0_REQ_TMR_0    (uint8)0x0a   ///< TMR 0
#define DMA0_REQ_TMR_3    (uint8)0x0b   ///< TMR 3
#define DMA0_REQ_AFE_CH2  (uint8)0x0c   ///< AFE CH2
#define DMA0_REQ_ADC      (uint8)0x0d   ///< ADC
#define DMA0_REQ_CMP0     (uint8)0x0e   ///< CMP0
#define DMA0_REQ_PTE      (uint8)0x0f   ///< PTE
#define DMA0_REQ_PTA      (uint8)0x10   ///< PTA
#define DMA0_REQ_ENABLED  (uint8)0x11   ///< ALWAYS ENABLED
/*! @} End of dma0_request_sources                                            */

/**************************************************************************//*!
*//*! @addtogroup dma1_request_sources
* @{
* @details DMA channel 1 request sources used by some @ref dma_chan_config 
* configuration structures.
 *****************************************************************************/
#define DMA1_REQ_DISABLED (uint8)0x00   ///< DISABLED (default)
#define DMA1_REQ_SPI0_TX  (uint8)0x01   ///< SPI0 TX
#define DMA1_REQ_SPI0_RX  (uint8)0x02   ///< SPI0 RX
#define DMA1_REQ_UART1_TX (uint8)0x03   ///< UART1 TX
#define DMA1_REQ_UART1_RX (uint8)0x04   ///< UART1 RX
#define DMA1_REQ_I2C0     (uint8)0x05   ///< I2C0
#define DMA1_REQ_XBAR     (uint8)0x06   ///< XBAR
#define DMA1_REQ_UART3_TX (uint8)0x07   ///< UART3 TX
#define DMA1_REQ_UART3_RX (uint8)0x08   ///< UART3 RX
#define DMA1_REQ_AFE_CH0  (uint8)0x09   ///< AFE CH0
#define DMA1_REQ_TMR_0    (uint8)0x0a   ///< TMR 0
#define DMA1_REQ_TMR_3    (uint8)0x0b   ///< TMR 3
#define DMA1_REQ_AFE_CH2  (uint8)0x0c   ///< AFE CH2
#define DMA1_REQ_PTE      (uint8)0x0d   ///< PTE
#define DMA1_REQ_CMP0     (uint8)0x0e   ///< CMP0
#define DMA1_REQ_PTF      (uint8)0x0f   ///< PTF
#define DMA1_REQ_PTB      (uint8)0x10   ///< PTB
#define DMA1_REQ_ENABLED  (uint8)0x11   ///< ALWAYS ENABLED
/*! @} End of dma1_request_sources                                            */

/**************************************************************************//*!
*//*! @addtogroup dma2_request_sources
* @{
* @details DMA channel 2 request sources used by some @ref dma_chan_config 
* configuration structures.
 *****************************************************************************/
#define DMA2_REQ_DISABLED (uint8)0x00   ///< DISABLED (default)
#define DMA2_REQ_SPI1_TX  (uint8)0x01   ///< SPI1 TX
#define DMA2_REQ_SPI1_RX  (uint8)0x02   ///< SPI1 RX
#define DMA2_REQ_UART1_TX (uint8)0x03   ///< UART1 TX
#define DMA2_REQ_UART1_RX (uint8)0x04   ///< UART1 RX
#define DMA2_REQ_I2C1     (uint8)0x05   ///< I2C1
#define DMA2_REQ_XBAR     (uint8)0x06   ///< XBAR
#define DMA2_REQ_UART2_TX (uint8)0x07   ///< UART2 TX
#define DMA2_REQ_UART2_RX (uint8)0x08   ///< UART2 RX
#define DMA2_REQ_AFE_CH1  (uint8)0x09   ///< AFE CH1
#define DMA2_REQ_TMR_2    (uint8)0x0a   ///< TMR 2
#define DMA2_REQ_TMR_1    (uint8)0x0b   ///< TMR 1
#define DMA2_REQ_AFE_CH3  (uint8)0x0c   ///< AFE CH3
#define DMA2_REQ_PTI      (uint8)0x0d   ///< PTI
#define DMA2_REQ_CMP1     (uint8)0x0e   ///< CMP1
#define DMA2_REQ_PTG      (uint8)0x0f   ///< PTG
#define DMA2_REQ_PTC      (uint8)0x10   ///< PTC
#define DMA2_REQ_ENABLED  (uint8)0x11   ///< ALWAYS ENABLED
/*! @} End of dma2_request_sources                                            */

/**************************************************************************//*!
*//*! @addtogroup dma3_request_sources
* @{
* @details DMA channel 3 request sources used by some @ref dma_chan_config 
* configuration structures.
 *****************************************************************************/
#define DMA3_REQ_DISABLED (uint8)0x00   ///< DISABLED (default)
#define DMA3_REQ_SPI1_TX  (uint8)0x01   ///< SPI1 TX
#define DMA3_REQ_SPI1_RX  (uint8)0x02   ///< SPI1 RX
#define DMA3_REQ_UART0_TX (uint8)0x03   ///< UART0 TX
#define DMA3_REQ_UART0_RX (uint8)0x04   ///< UART0 RX
#define DMA3_REQ_I2C1     (uint8)0x05   ///< I2C1
#define DMA3_REQ_XBAR     (uint8)0x06   ///< XBAR
#define DMA3_REQ_UART2_TX (uint8)0x07   ///< UART2 TX
#define DMA3_REQ_UART2_RX (uint8)0x08   ///< UART2 RX
#define DMA3_REQ_AFE_CH1  (uint8)0x09   ///< AFE CH1
#define DMA3_REQ_TMR_2    (uint8)0x0a   ///< TMR 2
#define DMA3_REQ_TMR_1    (uint8)0x0b   ///< TMR 1
#define DMA3_REQ_AFE_CH3  (uint8)0x0c   ///< AFE CH3
#define DMA3_REQ_ADC      (uint8)0x0d   ///< ADC
#define DMA3_REQ_CMP1     (uint8)0x0e   ///< CMP1
#define DMA3_REQ_PTH      (uint8)0x0f   ///< PTH
#define DMA3_REQ_PTD      (uint8)0x10   ///< PTD
#define DMA3_REQ_ENABLED  (uint8)0x11   ///< ALWAYS ENABLED
/*! @} End of dma3_request_sources                                            */

/**************************************************************************//*!
*//*! @addtogroup dma_request_sizes
* @{
* @details Determines the data size for the DMA controller transfers.
 *****************************************************************************/
#define DMA_SIZE8BIT      (uint8)0x01   ///< 1-byte transfer
#define DMA_SIZE16BIT     (uint8)0x02   ///< 2-byte transfer
#define DMA_SIZE32BIT     (uint8)0x00   ///< 4-byte transfer (default)
/*! @} End of dma_request_sizes                                               */

/***************************************************************************//*!
 *//*! @addtogroup dma_chacr_attr
 * @{
 * @details Access control attributes needed to reference the channel's transfer 
 * channel descriptor (TCDn). When a read or write access occurs to any fields in
 * the TCDn, if the system mode has sufficient privileges as defined by DMA_CHACR, 
 * the access completes. Otherwise, the access aborts and terminates in an error.
 * @warning The CHACR field can be changed only during operation in privileged 
 * secure mode. In any other mode, attempted changes to this field are not 
 * performed, and the access terminates with an error.  
 ******************************************************************************/
#define DMA_CHACR_RW_RW_RW  (uint8)0    ///< User nonsecure=Read/Write,  
                                        ///  User secure=Read/Write, 
                                        ///  Privileged secure=Read/Write (default)
#define DMA_CHACR_NO_RW_RW  (uint8)1    ///< User nonsecure=None, 
                                        ///  User secure=Read/Write, 
                                        ///  Privileged secure=Read/Write
#define DMA_CHACR_NO_NO_RW  (uint8)2    ///< User nonsecure=None, 
                                        ///  User secure=None, 
                                        ///  Privileged secure=Read/Write
/*! @} End of dma_chacr_attr                                                  */

/***************************************************************************//*!
 *//*! @addtogroup dma_umnsm_attr
 * @{
 * @details These assignments define the privileged/user and secure/nonsecure 
 * attributes for the DMA channel as it executes. When a given channel is activated 
 * and executes, the DMA generates the appropriate privileged/user and 
 * secure/nonsecure attributes for all source reads and destination writes defined 
 * by DMA_UMNSM. 
 ******************************************************************************/
#define DMA_UMNSM_EN_PRIV_OR_USER_SECURE_OR_NONSEC  (uint8)0  ///< Enable privileged or user, secure or nonsecure (default).
#define DMA_UMNSM_FR_PRIV_SECURE                    (uint8)1  ///< Forced to privileged secure.
#define DMA_UMNSM_FR_USER_SECURE                    (uint8)2  ///< Forced to user secure.
#define DMA_UMNSM_FR_USER_UNSEC                     (uint8)3  ///< Forced to user unsecure.
/*! @} End of dma_umnsm_attr                                                  */

/******************************************************************************
* DMA default configurations used by DMA_Init() function
*
*//*! @addtogroup dma_chan_config
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief Selects and starts software triggered continuous memory to memory data 
 * transfer.  
 * @details Configures and starts DMA channel to operate in software trigger 
 * continuous memory to memory data transfer. 
 * @param   srcaddr   Source memory buffer address (uint32).
 * @param   dstaddr   Destination memory buffer address (uint32).
 * @param   nbytes    Bytes count to be transferred in range 0x000001 to 0x0FFFFF.
 * @note              Implemented as an inline macro.
 *****************************************************************************/
#define DMA_CH_SWTRG_M2M_CN_CONFIG(srcaddr,dstaddr,nbytes)                    \
(tDMA_CH){                                                                    \
/* CHCFG */  CLR(DMAMUX_CHCFG_ENBL_MASK)|CLR(DMAMUX_CHCFG_TRIG_MASK)|         \
/* ..... */  SET(DMAMUX_CHCFG_SOURCE(0)), /* DMA_REQ_DISABLED */              \
/* SAR   */  (uint32)srcaddr,                                                 \
/* DAR   */  (uint32)dstaddr,                                                 \
/* DSR   */  CLR(DMA_DSR_BCR_DONE_MASK)|((uint32)nbytes&0x0FFFFF),            \
/* DCR   */  CLR(DMA_DCR_EINT_MASK)|CLR(DMA_DCR_ERQ_MASK)|                    \
/* ...   */  CLR(DMA_DCR_CS_MASK)|CLR(DMA_DCR_AA_MASK)|CLR(DMA_DCR_CHACR(0))| \
/* ...   */  CLR(DMA_DCR_UMNSM(0))|CLR(DMA_DCR_EADREQ_MASK)|                  \
/* ...   */  SET(DMA_DCR_SINC_MASK)|SET(DMA_DCR_SSIZE(DMA_SIZE8BIT))|         \
/* ...   */  SET(DMA_DCR_DINC_MASK)|SET(DMA_DCR_DSIZE(DMA_SIZE8BIT))|         \
/* ...   */  SET(DMA_DCR_START_MASK)|SET(DMA_DCR_SMOD(0))|                    \
/* ...   */  SET(DMA_DCR_DMOD(0))|CLR(DMA_DCR_D_REQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_LINKCC(0))|SET(DMA_DCR_LCH1(0))|SET(DMA_DCR_LCH2(0)) \
}

/***************************************************************************//*!
 * @brief Selects software initiated continuous memory to peripheral data transfer.  
 * @details Configures software initiated continuous memory to peripheral data 
 * transfer. 
 * @param   srcaddr   Source memory buffer address (uint32-regsize aligned)
 * @param   regsize   Select @ref dma_request_sizes.
 * @param   regaddr   Destination peripheral register address (uint32).
 * @param   nbytes    Bytes count to be transferred in range 0x000001 to 0x0FFFFF.
 * @note              Implemented as an inline macro.
 *****************************************************************************/
#define DMA_CH_SWTRG_M2P_CN_CONFIG(srcaddr,regsize,regaddr,nbytes)            \
(tDMA_CH){                                                                    \
/* CHCFG */  CLR(DMAMUX_CHCFG_ENBL_MASK)|CLR(DMAMUX_CHCFG_TRIG_MASK)|         \
/* ..... */  SET(DMAMUX_CHCFG_SOURCE(0)), /* DMA_REQ_DISABLED */              \
/* SAR   */  (uint32)srcaddr,                                                 \
/* DAR   */  (uint32)regaddr,                                                 \
/* DSR   */  CLR(DMA_DSR_BCR_DONE_MASK)|((uint32)nbytes&0x0FFFFF),            \
/* DCR   */  CLR(DMA_DCR_EINT_MASK)|CLR(DMA_DCR_ERQ_MASK)|                    \
/* ...   */  CLR(DMA_DCR_CS_MASK)|CLR(DMA_DCR_AA_MASK)|CLR(DMA_DCR_CHACR(0))| \
/* ...   */  CLR(DMA_DCR_UMNSM(0))|CLR(DMA_DCR_EADREQ_MASK)|                  \
/* ...   */  SET(DMA_DCR_SINC_MASK)|SET(DMA_DCR_SSIZE(regsize))|              \
/* ...   */  CLR(DMA_DCR_DINC_MASK)|SET(DMA_DCR_DSIZE(regsize))|              \
/* ...   */  SET(DMA_DCR_START_MASK)|SET(DMA_DCR_SMOD(0))|                    \
/* ...   */  SET(DMA_DCR_DMOD(0))|CLR(DMA_DCR_D_REQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_LINKCC(0))|SET(DMA_DCR_LCH1(0))|SET(DMA_DCR_LCH2(0)) \
}

/***************************************************************************//*!
 * @brief Selects hardware initiated cycle steal peripheral to memory data 
 * transfer.  
 * @details Configures hardware initiated cycle steal peripheral to memory data 
 * transfer. 
 * @param   reqsrc    Request source chosen for DMA channel being configured:\n 
 *                    DMA0=@ref dma0_request_sources\n
 *                    DMA1=@ref dma1_request_sources\n
 *                    DMA2=@ref dma2_request_sources\n
 *                    DMA3=@ref dma3_request_sources.
 * @param   regsize   Select @ref dma_request_sizes.
 * @param   regaddr   Peripheral register address (uint32).
 * @param   dstaddr   Destination memory buffer address (uint32-regsize aligned).
 * @param   nbytes    Bytes count to be transferred in range 0x000001 to 0x0FFFFF.
 * @note              Implemented as an inline macro.
 *****************************************************************************/
#define DMA_CH_HWTRG_P2M_CS_CONFIG(reqsrc,regsize,regaddr,dstaddr,nbytes)     \
(tDMA_CH){                                                                    \
/* CHCFG */  SET(DMAMUX_CHCFG_ENBL_MASK)|CLR(DMAMUX_CHCFG_TRIG_MASK)|         \
/* ..... */  SET(DMAMUX_CHCFG_SOURCE(reqsrc)),                                \
/* SAR   */  (uint32)regaddr,                                                 \
/* DAR   */  (uint32)dstaddr,                                                 \
/* DSR   */  CLR(DMA_DSR_BCR_DONE_MASK)|((uint32)nbytes&0x0FFFFF),            \
/* DCR   */  CLR(DMA_DCR_EINT_MASK)|SET(DMA_DCR_ERQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_CS_MASK)|CLR(DMA_DCR_AA_MASK)|CLR(DMA_DCR_CHACR(0))| \
/* ...   */  CLR(DMA_DCR_UMNSM(0))|SET(DMA_DCR_EADREQ_MASK)|                  \
/* ...   */  CLR(DMA_DCR_SINC_MASK)|SET(DMA_DCR_SSIZE(regsize))|              \
/* ...   */  SET(DMA_DCR_DINC_MASK)|SET(DMA_DCR_DSIZE(regsize))|              \
/* ...   */  CLR(DMA_DCR_START_MASK)|SET(DMA_DCR_SMOD(0))|                    \
/* ...   */  SET(DMA_DCR_DMOD(0))|SET(DMA_DCR_D_REQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_LINKCC(0))|SET(DMA_DCR_LCH1(0))|SET(DMA_DCR_LCH2(0)) \
}

/***************************************************************************//*!
 * @brief Selects hardware initiated cycle steal memory to peripheral data 
 * transfer.  
 * @details Configures hardware initiated cycle steal memory to peripheral data 
 * transfer. 
 * @param   reqsrc    Request source chosen for DMA channel being configured:\n 
 *                    DMA0=@ref dma0_request_sources\n
 *                    DMA1=@ref dma1_request_sources\n
 *                    DMA2=@ref dma2_request_sources\n
 *                    DMA3=@ref dma3_request_sources.
 * @param   srcaddr   Source memory buffer address (uint32-regsize aligned)
 * @param   regsize   Select @ref dma_request_sizes.
 * @param   regaddr   Destination peripheral register address (uint32).
 * @param   nbytes    Bytes count to be transferred in range 0x000001 to 0x0FFFFF.
 * @note              Implemented as an inline macro.
 *****************************************************************************/
#define DMA_CH_HWTRG_M2P_CS_CONFIG(reqsrc,srcaddr,regsize,regaddr,nbytes)     \
(tDMA_CH){                                                                    \
/* CHCFG */  SET(DMAMUX_CHCFG_ENBL_MASK)|CLR(DMAMUX_CHCFG_TRIG_MASK)|         \
/* ..... */  SET(DMAMUX_CHCFG_SOURCE(reqsrc)),                                \
/* SAR   */  (uint32)srcaddr,                                                 \
/* DAR   */  (uint32)regaddr,                                                 \
/* DSR   */  CLR(DMA_DSR_BCR_DONE_MASK)|((uint32)nbytes&0x0FFFFF),            \
/* DCR   */  CLR(DMA_DCR_EINT_MASK)|SET(DMA_DCR_ERQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_CS_MASK)|CLR(DMA_DCR_AA_MASK)|CLR(DMA_DCR_CHACR(0))| \
/* ...   */  CLR(DMA_DCR_UMNSM(0))|SET(DMA_DCR_EADREQ_MASK)|                  \
/* ...   */  SET(DMA_DCR_SINC_MASK)|SET(DMA_DCR_SSIZE(regsize))|              \
/* ...   */  CLR(DMA_DCR_DINC_MASK)|SET(DMA_DCR_DSIZE(regsize))|              \
/* ...   */  CLR(DMA_DCR_START_MASK)|SET(DMA_DCR_SMOD(0))|                    \
/* ...   */  SET(DMA_DCR_DMOD(0))|SET(DMA_DCR_D_REQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_LINKCC(0))|SET(DMA_DCR_LCH1(0))|SET(DMA_DCR_LCH2(0)) \
}

/***************************************************************************//*!
 * @brief Selects hardware initiated cycle steal peripheral to peripheral data
 * transfer executed in a loop.  
 * @details Configures hardware initiated cycle steal peripheral to peripheral 
 * data transfer executed in a loop. 
 * @param   reqsrc    Request source chosen for DMA channel being configured:\n 
 *                    DMA0=@ref dma0_request_sources\n
 *                    DMA1=@ref dma1_request_sources\n
 *                    DMA2=@ref dma2_request_sources\n
 *                    DMA3=@ref dma3_request_sources.
 * @param   regsize   Select @ref dma_request_sizes.
 * @param   reg1addr  Source peripheral register address (uint32).
 * @param   reg2addr  Destination peripheral register address (uint32).
 * @note              Implemented as an inline macro.
 *****************************************************************************/
#define DMA_CH_HWTRG_P2P_CS_LOOP_CONFIG(reqsrc,regsize,reg1addr,reg2addr)     \
(tDMA_CH){                                                                    \
/* CHCFG */  SET(DMAMUX_CHCFG_ENBL_MASK)|CLR(DMAMUX_CHCFG_TRIG_MASK)|         \
/* ..... */  SET(DMAMUX_CHCFG_SOURCE(reqsrc)),                                \
/* SAR   */  (uint32)reg1addr,                                                \
/* DAR   */  (uint32)reg2addr,                                                \
/* DSR   */  CLR(DMA_DSR_BCR_DONE_MASK)|((uint32)regsize&0x0FFFFF),           \
/* DCR   */  CLR(DMA_DCR_EINT_MASK)|SET(DMA_DCR_ERQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_CS_MASK)|CLR(DMA_DCR_AA_MASK)|CLR(DMA_DCR_CHACR(0))| \
/* ...   */  CLR(DMA_DCR_UMNSM(0))|SET(DMA_DCR_EADREQ_MASK)|                  \
/* ...   */  CLR(DMA_DCR_SINC_MASK)|SET(DMA_DCR_SSIZE(regsize))|              \
/* ...   */  CLR(DMA_DCR_DINC_MASK)|SET(DMA_DCR_DSIZE(regsize))|              \
/* ...   */  CLR(DMA_DCR_START_MASK)|SET(DMA_DCR_SMOD(0))|                    \
/* ...   */  SET(DMA_DCR_DMOD(0))|SET(DMA_DCR_D_REQ_MASK)|                    \
/* ...   */  SET(DMA_DCR_LINKCC(0))|SET(DMA_DCR_LCH1(0))|SET(DMA_DCR_LCH2(0)) \
}
/*! @} End of dma_chan_config                                                 */

/******************************************************************************
* DMA callback registered by DMA_Init() function
*
*//*! @addtogroup dma_ch_callback
* @{
*******************************************************************************/
/*! DMA_CH_CALLBACK type declaration                                          */
typedef enum
{
  CE_CALLBACK   =1,   ///< Configuration error (CE) interrupt
  DONE_CALLBACK =2,   ///< Transactions done (DONE) interrupt
} DMA_CH_CALLBACK_TYPE;

/*! @brief DMA_CH_CALLBACK function declaration                               */
typedef void (*DMA_CH_CALLBACK)(DMA_CH_CALLBACK_TYPE type);
/*! @} End of dma_ch_callback                                                 */

/******************************************************************************
* DMA function and macro definitions
*
*//*! @addtogroup dma_macro
* @{
*******************************************************************************/
#define DMA0_Ack              iosetw (DMA_DSR_BCR0,DMA_DSR_BCR_DONE_MASK)
#define DMA1_Ack              iosetw (DMA_DSR_BCR1,DMA_DSR_BCR_DONE_MASK)
#define DMA2_Ack              iosetw (DMA_DSR_BCR2,DMA_DSR_BCR_DONE_MASK)
#define DMA3_Ack              iosetw (DMA_DSR_BCR3,DMA_DSR_BCR_DONE_MASK)
/***************************************************************************//*!
 * @brief   DMA channel transfer complete acknowledge.
 * @details This macro acknowledges completed DMA channel transfer.
 * @param   chan      DMA0,DMA1,DMA2,DMA3
 * @note    Implemented as an inline macro.
 ******************************************************************************/ 
#define DMA_Ack(chan)         chan##_Ack

#define DMA0_IsError          ((DMA_DSR_BCR0&DMA_DSR_BCR_CE_MASK)?TRUE:FALSE)
#define DMA1_IsError          ((DMA_DSR_BCR1&DMA_DSR_BCR_CE_MASK)?TRUE:FALSE)
#define DMA2_IsError          ((DMA_DSR_BCR2&DMA_DSR_BCR_CE_MASK)?TRUE:FALSE)
#define DMA3_IsError          ((DMA_DSR_BCR3&DMA_DSR_BCR_CE_MASK)?TRUE:FALSE)
/***************************************************************************//*!
 * @brief   DMA channel configuration error check.
 * @details This macro checks DMA channel for configuration error.
 * @param   chan   DMA0,DMA1,DMA2,DMA3
 * @return  TRUE   a channel configuration error occurred (call <c>DMA_Ack()</c> 
 *                 to clear error condition)\n
 * @return  FALSE  no configuration error exists\n 
 * @note    Implemented as an inline macro.
 ******************************************************************************/ 
#define DMA_IsError(chan)     chan##_IsError 

#define DMA0_IsDone           ((DMA_DSR_BCR0&DMA_DSR_BCR_DONE_MASK)?TRUE:FALSE)
#define DMA1_IsDone           ((DMA_DSR_BCR1&DMA_DSR_BCR_DONE_MASK)?TRUE:FALSE)
#define DMA2_IsDone           ((DMA_DSR_BCR2&DMA_DSR_BCR_DONE_MASK)?TRUE:FALSE)
#define DMA3_IsDone           ((DMA_DSR_BCR3&DMA_DSR_BCR_DONE_MASK)?TRUE:FALSE)
/***************************************************************************//*!
 * @brief   DMA channel transfer complete check.
 * @details This macro checks DMA channel transfer complete.
 * @param   chan   DMA0,DMA1,DMA2,DMA3
 * @return  TRUE   a channel DMA transfer completed (call <c>DMA_Ack()</c>
 *                 to acknowledge the DMA transfer)\n
 * @return  FALSE  DMA channel transfer is not yet complete\n 
 * @note    Implemented as an inline macro.
 ******************************************************************************/ 
#define DMA_IsDone(chan)      chan##_IsDone

/***************************************************************************//*!
 * @brief   Function installs callback function for specified DMA channel.
 * @details This function installs callback function for specified DMA channel.
 * @param   chan      DMA0,DMA1,DMA2,DMA3
 * @param   ip        @ref cm0plus_prilvl
 * @param   callback  Pointer to @ref dma_ch_callback()
 * @note    Implemented as a function call.
 * @warning Register interrupt handler for DMA channels:
 *|Channel|Appconfig.h                                                    |
 *|:-----:|:--------------------------------------------------------------|
 *|DMA0   |<c>extern void DMA0_isr(void);\n#define VECTOR_016 DMA0_isr</c>|
 *|DMA1   |<c>extern void DMA1_isr(void);\n#define VECTOR_017 DMA1_isr</c>|
 *|DMA2   |<c>extern void DMA2_isr(void);\n#define VECTOR_018 DMA2_isr</c>|
 *|DMA3   |<c>extern void DMA3_isr(void);\n#define VECTOR_019 DMA3_isr</c>|
 * @see     @ref DMA_Init, @ref DMA_AdvInit
 ******************************************************************************/
#define DMA_InstallCallback(chan,ip,callback)                                 \
                              DMA_InstallCallback##chan(ip,callback) 

/***************************************************************************//*!
 * @brief   DMA channel initialization function.
 * @details This function initializes specified DMA channel for data transfer.
 * @param   chan      DMA0,DMA1,DMA2,DMA3
 * @param   dma       Structure containing the initialization parameters
 *                    @ref dma_chan_config
 * @note    Implemented as a function call.
 * @see     @ref DMA_AdvInit, @ref DMA_InstallCallback
 ******************************************************************************/
#define DMA_Init(chan,dma)    chan##_Init(dma)

/***************************************************************************//*!
 * @brief   Advanced DMA channel initialization with access rights setting.
 * @details This function initializes specified DMA channel for data transfer. 
 * It also sets the access control attributes needed to reference the channel's 
 * transfer channel descriptor (TCDn) and defines the privileged/user and 
 * secure/nonsecure attributes for the DMA channel as it executes. 
 * @param   chan    DMA0,DMA1,DMA2,DMA3
 * @param   dma     Structure containing the initialization parameters
 *                  @ref dma_chan_config
 * @param   chacr   Select one of the following @ref dma_chacr_attr 
 * @param   umnsm   Select one of the following @ref dma_umnsm_attr                                                                 
 * @note    Implemented as a function call.
 * @see     @ref DMA_Init, @ref DMA_InstallCallback
 ******************************************************************************/
#define DMA_AdvInit(chan,dma,chacr,umnsm)   chan##_AdvInit(dma,chacr,umnsm)                               
/*! @} End of dma_macro                                                       */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/    
extern void DMA_InstallCallbackDMA0 (uint8 ip, DMA_CH_CALLBACK pCallback);
extern void DMA_InstallCallbackDMA1 (uint8 ip, DMA_CH_CALLBACK pCallback);
extern void DMA_InstallCallbackDMA2 (uint8 ip, DMA_CH_CALLBACK pCallback);
extern void DMA_InstallCallbackDMA3 (uint8 ip, DMA_CH_CALLBACK pCallback);

extern void DMA0_Init(tDMA_CH dma);
extern void DMA1_Init(tDMA_CH dma);
extern void DMA2_Init(tDMA_CH dma);
extern void DMA3_Init(tDMA_CH dma);

extern void DMA0_AdvInit(tDMA_CH dma, uint8 chacr, uint8 umnsm);
extern void DMA1_AdvInit(tDMA_CH dma, uint8 chacr, uint8 umnsm);
extern void DMA2_AdvInit(tDMA_CH dma, uint8 chacr, uint8 umnsm);
extern void DMA3_AdvInit(tDMA_CH dma, uint8 chacr, uint8 umnsm);
                                    
#endif /* __DMA_H */