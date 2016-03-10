/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      spi.c
 * @version   1.0.3.0
 * @date      Mar-12-2013
 * @brief     SPI driver source code.
******************************************************************************/
#include "common.h"
#include "spi.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static SPI_CALLBACK     volatile pCallbackSPI0=NULL;
static SPI_CALLBACK     volatile pCallbackSPI1=NULL;
static SPI_SSCALLBACK   volatile pCallbackSS0=NULL;
static SPI_SSCALLBACK   volatile pCallbackSS1=NULL;

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
void spi0_isr (void); 
void spi1_isr (void); 

/******************************************************************************
 * Public functions definitions                                               *
 ******************************************************************************/
#pragma diag_suppress=Pa082 

/******************************************************************************
 * SPI Init functions                                                         *
 ******************************************************************************/
void SPI0_Init (tSPI spi,SPI_SSCALLBACK pC2,uint8 ip,SPI_CALLBACK pC1)
{     
  /* main callback function initialization */
  if (pC1 != NULL)  
  { 
    pCallbackSPI0 = pC1; 
    NVIC_SetIsr(INT_SPI0,ip); 
  }

  /* initialization of SS callback function where SS will be driven manually */
  if (pC2 != NULL) { 
    pCallbackSS0 = pC2; 
    pCallbackSS0 (SS_INIT);     /* SS port initialization */
  }
  
  /* SPI configuration (without C3 register) */
  SPI0_C1      = spi.C1;        /* SPI control register 1 */
  SPI0_C2      = spi.C2;        /* SPI control register 2 */
  SPI0_BR      = spi.BR;        /* SPI baud rate register */
  SPI0_ML      = spi.ML;        /* SPI match register low */
  SPI0_MH      = spi.MH;        /* SPI match register high */
  SPI0_C1 |= SPI_C1_SPE_MASK;   /* Enable SPI */ 
  /* Note: FIFO functionality is not supported in SPI0 module */
}

void SPI1_Init (tSPI spi,SPI_SSCALLBACK pC2,uint8 ip, SPI_CALLBACK pC1)
{     
  /* main callback function initialization */
  if (pC1 != NULL)  
  { 
    pCallbackSPI1 = pC1; 
    NVIC_SetIsr(INT_SPI1,ip); 
  }

  /* initialization of SS callback function where SS will be driven manually */
  if (pC2 != NULL) { 
    pCallbackSS1 = pC2; 
    pCallbackSS1 (SS_INIT);     /* SS port initialization */
  }
  
  /* SPI configuration (with C3 register) */
  SPI1_C1      = spi.C1;        /* SPI control register 1 */
  SPI1_C2      = spi.C2;        /* SPI control register 2 */
  SPI1_C3      = spi.C3;        /* SPI control register 3 */
  SPI1_BR      = spi.BR;        /* SPI baud rate register */
  SPI1_ML      = spi.ML;        /* SPI match register low */
  SPI1_MH      = spi.MH;        /* SPI match register high */
  SPI1_C1 |= SPI_C1_SPE_MASK;   /* Enable SPI */ 
}

/******************************************************************************
 * Simple SPI Tx/Rx function (1 word Tx + 1 word Rx)                          *
 ******************************************************************************/
uint16 SPI0_TxRxWord(uint16 txdata)
{
  while (!SPI_TxCompl(SPI0));   /* wait until Tx buffer is empty */
  SPI_PutData(SPI0,txdata);     /* write an 16-bit data */
  while(!SPI_RxFull(SPI0));     /* wait until Rx buffer is full */
  return (SPI_GetData(SPI0));   /* read data and exit */
}

uint16 SPI1_TxRxWord(uint16 txdata)
{
  while (!SPI_TxCompl(SPI1));   /* wait until Tx buffer is empty */
  SPI_PutData(SPI1,txdata);     /* write an 16-bit data */
  while(!SPI_RxFull(SPI1));     /* wait until Rx buffer is full */
  return (SPI_GetData(SPI1));   /* read data and exit */
}

/******************************************************************************
 * Simple SPI Tx/Rx function (1 byte Tx + 1 byte Rx)                          *
 ******************************************************************************/
uint8 SPI0_TxRxByte(uint8 txdata)
{
  while (!SPI_TxCompl(SPI0));   /* wait until Tx buffer is empty */
  SPI_PutData(SPI0,txdata);     /* write an 8-bit data */
  while(!SPI_RxFull(SPI0));     /* wait until Rx buffer is full */
  return (SPI_GetData(SPI0));   /* read data and exit */
}

uint8 SPI1_TxRxByte(uint8 txdata)
{
  while (!SPI_TxCompl(SPI1));   /* wait until Tx buffer is empty */
  SPI_PutData(SPI1,txdata);     /* write an 8-bit data */
  while(!SPI_RxFull(SPI1));     /* wait until Rx buffer is full */
  return (SPI_GetData(SPI1));   /* read data and exit */
}

/******************************************************************************
 * Communication via an SPI (Tx+Rx data packet) with driving a slave select   *
 ******************************************************************************/
void SPI0_CommWord(uint16 *txdata, uint16 txcnt,uint16 *rxdata, uint16 rxcnt)
{
  register uint16 i;

  if (pCallbackSS0 != (SPI_SSCALLBACK)NULL) 
  {  
    pCallbackSS0 (SS_CLEAR);      /* SS=0 */
  }
  if (txdata != NULL) {           /* any data for transmitting? */
    for (i=0; i<txcnt; i++) {     /* writing cycle */
      SPI_TxRxWord(SPI0,*txdata);     /* Write an 16-bit data */
      txdata++;                   /* increment a source address */
    }
  }

  if (rxdata != NULL) {           /* any data for receiving? */      
    for (i=0; i<rxcnt; i++) {     /* reading cycle */
      /* Read an 16-bit data with writing a dummy data to generate SCLK signal */
      *rxdata=SPI_TxRxWord(SPI0,0xFF);/* 0xFF is dummy data for writing! */   
      rxdata++;                   /* increment a destination address */
    }
  }

  if (pCallbackSS0 != (SPI_SSCALLBACK)NULL) 
  {  
    pCallbackSS0 (SS_SET);        /* SS=1 */
  }
}

void SPI0_CommByte(uint8 *txdata, uint16 txcnt,uint8 *rxdata, uint16 rxcnt)
{
  register uint16 i;

  if (pCallbackSS0 != (SPI_SSCALLBACK)NULL) 
  {  
    pCallbackSS0 (SS_CLEAR);      /* SS=0 */
  }
  if (txdata != NULL) {           /* any data for transmitting? */
    for (i=0; i<txcnt; i++) {     /* writing cycle */
      SPI_TxRxByte(SPI0,*txdata);     /* Write an 8-bit data */
      txdata++;                   /* increment a source address */
    }
  }

  if (rxdata != NULL) {           /* any data for receiving? */      
    for (i=0; i<rxcnt; i++) {     /* reading cycle */
      /* Read an 8-bit data with writing a dummy data to generate SCLK signal */
      *rxdata=SPI_TxRxByte(SPI0,0xFF);/* 0xFF is dummy data for writing! */   
      rxdata++;                   /* increment a destination address */
    }
  }

  if (pCallbackSS0 != (SPI_SSCALLBACK)NULL) 
  {  
    pCallbackSS0 (SS_SET);        /* SS=1 */
  }
}

void SPI1_CommWord(uint16 *txdata, uint16 txcnt,uint16 *rxdata, uint16 rxcnt)
{
  register uint16 i;

  if (pCallbackSS1 != (SPI_SSCALLBACK)NULL) 
  {  
    pCallbackSS1 (SS_CLEAR);      /* SS=0 */
  }

  if (txdata != NULL) {           /* any data for transmitting? */
    for (i=0; i<txcnt; i++) {     /* writing cycle */
      SPI_TxRxWord(SPI1,*txdata);     /* Write an 16-bit data */
      txdata++;                   /* increment a source address */
    }
  }
  
  if (rxdata != NULL) {           /* any data for receiving? */      
    for (i=0; i<rxcnt; i++) {     /* reading cycle */
      /* Read an 16-bit data with writing a dummy data to generate SCLK signal */
      *rxdata=SPI_TxRxWord(SPI1,0xFF);/* 0xFF is dummy data for writing! */   
      rxdata++;                   /* increment a destination address */
    }
  }

  if (pCallbackSS1 != (SPI_SSCALLBACK)NULL) 
  {  
    pCallbackSS1 (SS_SET);        /* SS=1 */
  }
}

void SPI1_CommByte(uint8 *txdata, uint16 txcnt,uint8 *rxdata, uint16 rxcnt)
{
  register uint16 i;

  if (pCallbackSS1 != (SPI_SSCALLBACK)NULL) 
  {  
    pCallbackSS1 (SS_CLEAR);      /* SS=0 */
  }

  if (txdata != NULL) {           /* any data for transmitting? */
    for (i=0; i<txcnt; i++) {     /* writing cycle */
      SPI_TxRxByte(SPI1,*txdata);     /* Write an 8-bit data */
      txdata++;                   /* increment a source address */
    }
  }
  
  if (rxdata != NULL) {           /* any data for receiving? */      
    for (i=0; i<rxcnt; i++) {     /* reading cycle */
      /* Read an 8-bit data with writing a dummy data to generate SCLK signal */
      *rxdata=SPI_TxRxByte(SPI1,0xFF);/* 0xFF is dummy data for writing! */   
      rxdata++;                   /* increment a destination address */
    }
  }
  
  if (pCallbackSS1 != (SPI_SSCALLBACK)NULL) 
  {  
    pCallbackSS1 (SS_SET);        /* SS=1 */
  }
}

/******************************************************************************
 * interrupt function definitions                                             *
 ******************************************************************************/
void spi0_isr (void)
{
  register uint8 tmp;

  if (pCallbackSPI0 != (SPI_CALLBACK)NULL) 
  {  

    tmp = SPI0_S & (SPI_S_SPRF_MASK|SPI_S_SPTEF_MASK|SPI_S_SPMF_MASK|
                    SPI_S_MODF_MASK);
    
    /* Rx buffer or FIFO buffer is full */
    if(tmp & (SPI_S_SPRF_MASK)) 
      {pCallbackSPI0 (SPI0_CALLBACK,RXSPI_CALLBACK);}
    /* SPRF is cleared by reading data registers DH:DL */
    
    /* Tx buffer or FIFO buffer is empty */
    if(tmp & (SPI_S_SPTEF_MASK))
      {pCallbackSPI0 (SPI0_CALLBACK,TXSPI_CALLBACK);}
    /* SPTEF is cleared by writing a value to DH:DL */
    
    /* Receive data buffer matches the value in MH:ML registers */
    if(tmp & (SPI_S_SPMF_MASK))
      {pCallbackSPI0 (SPI0_CALLBACK,MATCH_CALLBACK);
      iosetb (SPI0_S,SPI_S_SPMF_MASK); /* clear SPMF flag */ }
    
    /* Mode fault error detected (Only for MSTR=1, MODFEN=1, SSOE=0) */
    if(tmp & (SPI_S_MODF_MASK)) 
      {pCallbackSPI0 (SPI0_CALLBACK,FAULT_CALLBACK);
      iosetb (SPI0_S,SPI_S_MODF_MASK); /* clear MODF flag */ }
  }
}

void spi1_isr (void)
{
  register uint8 tmp;

  if (pCallbackSPI1 != (SPI_CALLBACK)NULL) 
  {  

    tmp = SPI1_S & (SPI_S_SPRF_MASK|SPI_S_SPTEF_MASK|SPI_S_SPMF_MASK|
                    SPI_S_MODF_MASK);
    
    /* Rx buffer or FIFO buffer is full */
    if(tmp & (SPI_S_SPRF_MASK)) 
      {pCallbackSPI1 (SPI1_CALLBACK,RXSPI_CALLBACK);}
    /* SPRF is cleared by reading data registers DH:DL */
    
    /* Tx buffer or FIFO buffer is empty */
    if(tmp & (SPI_S_SPTEF_MASK))
      {pCallbackSPI1 (SPI1_CALLBACK,TXSPI_CALLBACK);}
    /* SPTEF is cleared by writing a value to DH:DL */
    
    /* Receive data buffer matches the value in MH:ML registers */
    if(tmp & (SPI_S_SPMF_MASK))
      {pCallbackSPI1 (SPI1_CALLBACK,MATCH_CALLBACK);
      iosetb (SPI1_S,SPI_S_SPMF_MASK); /* clear SPMF flag */ }
    
    /* Mode fault error detected (Only for MSTR=1, MODFEN=1, SSOE=0) */
    if(tmp & (SPI_S_MODF_MASK)) 
      {pCallbackSPI1 (SPI1_CALLBACK,FAULT_CALLBACK);
      iosetb (SPI1_S,SPI_S_MODF_MASK); /* clear MODF flag */ }
  }
}

/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/