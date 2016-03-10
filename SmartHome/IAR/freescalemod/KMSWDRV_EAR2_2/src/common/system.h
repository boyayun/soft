/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      system.h
 * @version   1.0.1.0
 * @date      Aug-10-2012      
 * @brief     Kinetis-M system definitions header file.
 ******************************************************************************/
#ifndef __SYSTEM_H
#define __SYSTEM_H

/* function prototypes for default interrupt service routine in vectors.c     */
void default_isr(void);

/* interrupt vector table function pointer definition                         */
typedef void pointer(void);

/* import prototype of the startup function                                   */
#if defined (__ICCARM__) /* IAR                                               */
  extern void __iar_program_start(void);
#elif defined(__GNUC__)  /* CW GCC                                            */
  extern void __gcc_program_start(void);  
#endif

/******************************************************************************/
/**** ARM core cortex M0+ System handler Vectors ******************************/
/******************************************************************************/
//                                            Address      Vector IRQ Source module   Source description
#define VECTOR_000 (pointer*)0x20002ff8   // 0x0000_0000 0 -      ARM core    Initial Supervisor SP
#if defined (__ICCARM__) /* IAR                                               */
  #define VECTOR_001  __iar_program_start // 0x0000_0004 1 -      ARM core    Initial Program Counter
#elif defined(__GNUC__)  /* CW GCC                                            */
  #define VECTOR_001  __gcc_program_start // 0x0000_0004 1 -      ARM core    Initial Program Counter
#endif
#ifndef VECTOR_002
  #define VECTOR_002    default_isr       // 0x0000_0008 2 -      ARM core    Non-maskable Interrupt (NMI)
#endif
#ifndef VECTOR_003
  #define VECTOR_003    default_isr       // 0x0000_000C 3 -      ARM core    Hard Fault
#endif
#ifndef VECTOR_004  
  #define VECTOR_004    default_isr       // 0x0000_0010 4 - 
#endif
#ifndef VECTOR_005
  #define VECTOR_005    default_isr       // 0x0000_0014 5 -          
#endif
#ifndef VECTOR_006
  #define VECTOR_006    default_isr       // 0x0000_0018 6 -          
#endif
#ifndef VECTOR_007
  #define VECTOR_007    default_isr       // 0x0000_001C 7 -                           
#endif
#ifndef VECTOR_008
  #define VECTOR_008    default_isr       // 0x0000_0020 8 -                           
#endif
#ifndef VECTOR_009
  #define VECTOR_009    default_isr       // 0x0000_0024 9 -
#endif
#ifndef VECTOR_010
  #define VECTOR_010    default_isr       // 0x0000_0028 10 -
#endif
#ifndef VECTOR_011
  #define VECTOR_011    default_isr       // 0x0000_002C 11 -     ARM core    Supervisor call (SVCall)
#endif
#ifndef VECTOR_0012
  #define VECTOR_012    default_isr       // 0x0000_0030 12 -         
#endif
#ifndef VECTOR_013
  #define VECTOR_013    default_isr       // 0x0000_0034 13 -                          
#endif
#ifndef VECTOR_014
  #define VECTOR_014    default_isr       // 0x0000_0038 14 -     ARM core    Pendable request for system service (PendableSrvReq)
#endif
#ifndef VECTOR_015
  #define VECTOR_015    default_isr       // 0x0000_003C 15 -     ARM core    System tick Timer (SysTick)
#endif

/******************************************************************************/
/**** Non-core On Platform Vectors ********************************************/
/******************************************************************************/
//                                            Address      Vector IRQ Source module   Source description
#ifndef VECTOR_016  
  #define VECTOR_016    default_isr       // 0x0000_0040 16   0   DMA0        DMA Channel 0 transfer complete
#endif
#ifndef VECTOR_017
  #define VECTOR_017    default_isr       // 0x0000_0044 17   1   DMA1        DMA Channel 1 transfer complete
#endif
#ifndef VECTOR_018
  #define VECTOR_018    default_isr       // 0x0000_0048 18   2   DMA2        DMA Channel 2 transfer complete
#endif
#ifndef VECTOR_019
  #define VECTOR_019    default_isr       // 0x0000_004C 19   3   DMA3        DMA Channel 3 transfer complete
#endif
#ifndef VECTOR_020
  #define VECTOR_020    default_isr       // 0x0000_0050 20   4   SPI0        Single interrupt vector for all sources
#endif
#ifndef VECTOR_021
  #define VECTOR_021    default_isr       // 0x0000_0054 21   5   SPI1        Single interrupt vector for all sources
#endif
#ifndef VECTOR_022
  #define VECTOR_022    default_isr       // 0x0000_0058 22   6   PMC         Low-voltage detect, Low-voltage warning
#endif
#ifndef VECTOR_023
  #define VECTOR_023    default_isr       // 0x0000_005C 23   7   TMR0        Quad Timer Counter 0
#endif
#ifndef VECTOR_024
  #define VECTOR_024    default_isr       // 0x0000_0060 24   8   TMR1        Quad Timer Counter 1
#endif
#ifndef VECTOR_025
  #define VECTOR_025    default_isr       // 0x0000_0064 25   9   TMR2        Quad Timer Counter 2
#endif
#ifndef VECTOR_026
  #define VECTOR_026    default_isr       // 0x0000_0068 26   10  TMR3        Quad Timer Counter 3
#endif
#ifndef VECTOR_027
  #define VECTOR_027    default_isr       // 0x0000_006C 27   11  PIT0/PIT1   Single interrupt vector for both PITs
#endif
#ifndef VECTOR_028
  #define VECTOR_028    default_isr       // 0x0000_0070 28   12  LLWU        Low Leakage Wakeup
#endif
#ifndef VECTOR_029
  #define VECTOR_029    default_isr       // 0x0000_0074 29   13  FLASH       Single interrupt vector for FLASH
#endif
#ifndef VECTOR_030
  #define VECTOR_030    default_isr       // 0x0000_0078 30   14  CMP0/CMP1   Single interrupt vector for both COMPs
#endif
#ifndef VECTOR_031
  #define VECTOR_031    default_isr       // 0x0000_007C 31   15  SLCD        Single interrupt vector for both SLCD
#endif
#ifndef VECTOR_032
  #define VECTOR_032    default_isr       // 0x0000_0080 32   16  ADC         OR'ed Interrupt for SAR
#endif
#ifndef VECTOR_033
  #define VECTOR_033    default_isr       // 0x0000_0084 33   17  PORTx       Single interrupt vector for all GPIO (PTx)
#endif
#ifndef VECTOR_034
  #define VECTOR_034    default_isr       // 0x0000_0088 34   18  RNGA        OR'ed interrupt for RNGA
#endif
#ifndef VECTOR_035
  #define VECTOR_035    default_isr       // 0x0000_008C 35   19  UART0/UART1 OR'ed interrupt for UART0 and UART1
#endif
#ifndef VECTOR_036
  #define VECTOR_036    default_isr       // 0x0000_0090 36   20  UART2/UART3 OR'ed interrupt for UART2 and UART3
#endif
#ifndef VECTOR_037
  #define VECTOR_037    default_isr       // 0x0000_0094 37   21  AFE0        AFE OR'ed interrupt
#endif
#ifndef VECTOR_038
  #define VECTOR_038    default_isr       // 0x0000_0098 38   22  AFE1        AFE OR'ed interrupt
#endif
#ifndef VECTOR_039
  #define VECTOR_039    default_isr       // 0x0000_009C 39   23  AFE2        AFE OR'ed interrupt
#endif
#ifndef VECTOR_040
  #define VECTOR_040    default_isr       // 0x0000_00A0 40   24  AFE0        AFE OR'ed interrupt
#endif
#ifndef VECTOR_041
  #define VECTOR_041    default_isr       // 0x0000_00A4 41   25  iRTC        iRTC interrupt   
#endif
#ifndef VECTOR_042
  #define VECTOR_042    default_isr       // 0x0000_00A8 42   26  I2C0/I2C1   OR'ed interrupt for I2C0 and I2C1
#endif
#ifndef VECTOR_043
  #define VECTOR_043    default_isr       // 0x0000_00AC 43   27  EWM         External Watchdog Monitor
#endif
#ifndef VECTOR_044
  #define VECTOR_044    default_isr       // 0x0000_00B0 44   28  MCG         MCG loss of Clock, loss os lock
#endif
#ifndef VECTOR_045
  #define VECTOR_045    default_isr       // 0x0000_00B4 45   29  WDOG        OR'ed WDOG interrupt
#endif
#ifndef VECTOR_046
  #define VECTOR_046    default_isr       // 0x0000_00B8 46   30  LPTMR       OR'ed LPTIMER interrupt
#endif
#ifndef VECTOR_047
  #define VECTOR_047    default_isr       // 0x0000_00BC 47   31  XBAR        Peripheral XBAR OR'ed Interrupt
#endif

/******************************************************************************/
/**** Boot options default parameters - can be overloaded in appconfig.h ******/
/******************************************************************************/
/** Flash Security FTFA_FSEC[1-0]: ********************************************/
/*  2 - MCU security status is unsecure (default - shipping condition)        */
/*  3 - MCU security status is secure                                         */
#ifdef BOOT_FLASH_SECURE 
  #define MCU_SECURITY   3
#else
  #define MCU_SECURITY   2
#endif

/** Low-power boot FTFL_FOPT[0]: **********************************************/
/*  0 - core and system clock divider set to divide by 8                      */
/*  1 - core and system clock divider set to divide by 1 (default)            */
#ifdef BOOT_DIV_CLK_BY_8 
  #define BOOT_LPBOOT    0 
#else
  #define BOOT_LPBOOT    1 
#endif

/** NMI_B pin enable FTFL_FOPT[2]: ********************************************/
/*  0 - NMI_B pin is disabled. NMI functionality is disabled                  */
/*  1 - NMI_B pin is enabled. NMI functionality is enabled (default)          */
#ifdef BOOT_NMI_DISABLE 
  #define BOOT_NMI_EN    0 
#else
  #define BOOT_NMI_EN    1 
#endif

/** Execution Mode FTFL_FOPT[3]: **********************************************/
/*  0 - Execution Mode is RUN Mode (default)                                  */
/*  1 - VLPR execution mode enetered by startup                               */
#ifdef BOOT_SWVLPR_MODE
  #define BOOT_EXE_MODE  1 
#else
  #define BOOT_EXE_MODE  0 
#endif

/** Clock source used by Flash in RUN boot mode FTFL_FOPT[5]: *****************/
/*  0 - Externally supplied clock used by Flash                               */
/*  1 - Internal clock source used by Flash (default)                         */
#ifdef BOOT_EXT_CLK 
  #define BOOT_CLK_SRC   0
#else
  #define BOOT_CLK_SRC   1
#endif

#define CONFIG_1	(pointer*)0xffffffff 
#define CONFIG_2	(pointer*)0xffffffff 
#define CONFIG_3	(pointer*)0xffffffff
#define CONFIG_4	(pointer*)((long)0xffffd2fc             |                   \
                            ((long)MCU_SECURITY   <<  0)  |                   \
                            ((long)BOOT_LPBOOT    <<  8)  |                   \
                            ((long)BOOT_NMI_EN    << 10)  |                   \
                            ((long)BOOT_EXE_MODE  << 11)  |                   \
                            ((long)BOOT_CLK_SRC   << 13))
#endif /* __SYSTEM_H */