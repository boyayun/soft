/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      arm_cm0.h
 * @version   1.0.1.0
 * @date      May-18-2012      
 * @brief     Header file.
 ******************************************************************************/
#ifndef __ARM_CM0_H
#define __ARM_CM0_H

/*ARM Cortex M0 implementation for interrupt priority shift                   */
#define ARM_INTERRUPT_LEVEL_BITS          6

/******************************************************************************
* Priority level definition
*
*//*! @addtogroup cm0plus_prilvl
* @{
*******************************************************************************/
#define PRI_LVL0 (uint8)0x00        ///< Priority level 0 (highest priority)
#define PRI_LVL1 (uint8)0x01        ///< Priority level 1 
#define PRI_LVL2 (uint8)0x02        ///< Priority level 2 
#define PRI_LVL3 (uint8)0x03        ///< Priority level 3 (lowest priority)
/*! @} End of cm0plus_prilvl                                                  */

/***************************************************************************//*!
 * @brief   Configures the ARM system control register for STOP (deep sleep) mode.
 * @details This macro configures the ARM system control register for STOP 
 *          (deep sleep) mode and then executes the WFI instruction to enter the 
 *          mode.  
 * @note    Implemented as an inline macro.
 *****************************************************************************/
#define stop()      iosetw(SCB_SCR,SCB_SCR_SLEEPDEEP_MASK); __asm volatile("WFI")

/***************************************************************************//*!
 * @brief   Configures the ARM system control register for WAIT (sleep) mode.
 * @details This macro configures the ARM system control register for WAIT (sleep)
 *          mode and then executes the WFI instruction to enter the mode.  
 * @note    Implemented as an inline macro.
 *****************************************************************************/
#define wait()      ioclrw(SCB_SCR,SCB_SCR_SLEEPDEEP_MASK); __asm volatile("WFI")

/***************************************************************************//*!
 * @brief   Reads CONTROL register.
 * @details This function reads CONTROL register.
 * @return  uint32   32-bit value of the CONTROL register.
 * @note    Implemented as a fucntion.
 *****************************************************************************/
static inline uint32 GetCONTROL(void) 
{ 
  register uint32 regval;
  __asm volatile("MRS %[Rd],CONTROL":[Rd]"=r"(regval));  
  return regval; 
}

/***************************************************************************//*!
 * @brief   Writes CONTROL register.
 * @details This function writes <c>regval</c> to the CONTROL register.
 * @param   regval  32-bit value of the CONTROL register.   
 * @note    Implemented as an inline function.
 *****************************************************************************/
static inline void SetCONTROL(register uint32 regval) 
{ 
  __asm volatile("MSR CONTROL,%[Rs]":[Rs]"+r"(regval)); 
}

/******************************************************************************
* Cortex-M0+ function and macro definitions
*
*//*! @addtogroup cm0plus_macros
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Returns state of global interrupt disable bit from Priority Mask 
 *          Register (PRIMASK).
 * @details This macro returns state of global interrupt disable bit from 
 *          Priority Mask Register (PRIMASK).
 * @return  TRUE    all interrupts disabled\n
 * @return  FALSE   all interrupts enabled\n
 * @note    Implemented as an inline function.
 * @see     @ref SetInterruptMask, @ref EnableInterrupts, @ref DisableInterrupts, 
 *          @ref NVIC_SetIsr, @ref NVIC_ClrIsr, @ref NVIC_CallIsr, 
 *          @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr, @ref FAULT_SetIsr 
 ******************************************************************************/
static inline uint32 GetInterruptMask (void)        
{ 
  register uint32 mask;
  __asm volatile("MRS %[Rd],PRIMASK":[Rd]"=r"(mask));  
  return mask;
}

/***************************************************************************//*!
 * @brief   Controls state of global interrupt disable bit in Priority Mask 
 *          Register (PRIMASK).
 * @details This macro controls state of global interrupt disable bit in 
 *          Priority Mask Register (PRIMASK).
 * @param   mask    TRUE (all interrupts disabled)\n
 *                  FALSE (all interrupts enabled).
 * @note    Implemented as an inline function.
 * @see     @ref GetInterruptMask, @ref EnableInterrupts, @ref DisableInterrupts, 
 *          @ref NVIC_SetIsr, @ref NVIC_ClrIsr, @ref NVIC_CallIsr, 
 *          @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr, @ref FAULT_SetIsr 
 ******************************************************************************/
static inline void SetInterruptMask (register uint32 mask)
{ 
  __asm volatile("MSR PRIMASK,%[Rs]":[Rs]"+r"(mask)); 
}

/***************************************************************************//*!
 * @brief   Enables all interrupts.
 * @details This macro enables all interrupts by clearing disable bit in Priority 
 *          Mask Register (PRIMASK).
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref DisableInterrupts, 
 *          @ref NVIC_SetIsr, @ref NVIC_ClrIsr, @ref NVIC_CallIsr, 
 *          @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr, @ref FAULT_SetIsr 
 ******************************************************************************/
#define EnableInterrupts()    __asm volatile("CPSIE i");

/***************************************************************************//*!
 * @brief   Disables all interrupts.
 * @details This macro disables all interrupts by setting disable bit in Priority 
 *          Mask Register (PRIMASK).
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts, 
 *          @ref NVIC_SetIsr, @ref NVIC_ClrIsr, @ref NVIC_CallIsr, 
 *          @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr, @ref FAULT_SetIsr 
 ******************************************************************************/
#define DisableInterrupts()   __asm volatile("CPSID i");

/***************************************************************************//*!
 * @brief   Enters unprivileged (user) mode of execution.
 * @details This macro enters unprivileged (user) mode of execution. 
 *          It can be called from software running in privileged (supervisor) mode.
 * @note    Implemented as an inline macro.
 * @see     @ref SupervisorMode, @ref SVC_Call, @ref ProcessStackPointer, 
 *          @ref MainStackPointer
 ******************************************************************************/
#define UserMode()            SetCONTROL(GetCONTROL() |  0x00000001l)

/***************************************************************************//*!
 * @brief   Enters privileged (supervisor) mode of execution.
 * @details This macro enters privileged (supervisor) mode of execution. 
 *          It can be called from software running in handler or privileged 
 *          (supervisor) modes. 
 * @note    Implemented as an inline macro.
 * @warning Calling it from unprivileged (user) mode of execution causes not 
 *          entering into privileged (supervisor) mode of execution. 
 *          Call <c>SVC_Call()</c> from privileged to user mode.
 * @see     @ref UserMode, @ref SVC_Call, @ref ProcessStackPointer, 
 *          @ref MainStackPointer
 ******************************************************************************/
#define SupervisorMode()      SetCONTROL(GetCONTROL() & ~0x00000001l)

/***************************************************************************//*!
 * @brief   Supervisor call.
 * @details This macro performs supervisor call. Service routine executes in 
 *          handler mode.
 * @note    Implemented as an inline macro.
 * @see     @ref UserMode, @ref SupervisorMode, @ref ProcessStackPointer, 
 *          @ref MainStackPointer
 ******************************************************************************/
#define SVC_Call()            __asm volatile("SVC #0x00")

/***************************************************************************//*!
 * @brief   Switches stack pointer to process stack pointer. 
 * @details This macro switches stack pointer to process stack pointer.
 * @note    Implemented as an inlined macro.
 * @see     @ref UserMode, @ref SupervisorMode, @ref SVC_Call, 
 *          @ref MainStackPointer
 ******************************************************************************/
#define ProcessStackPointer() SetCONTROL(GetCONTROL() |  0x00000002l)
   
/***************************************************************************//*!
 * @brief   Switches stack pointer back to main stack pointer.
 * @details This macro switches stack pointer back to main stack pointer.
 * @note    Implemented as an inlined macro.
 * @see     @ref UserMode, @ref SupervisorMode, @ref SVC_Call, 
 *          @ref ProcessStackPointer
 ******************************************************************************/
#define MainStackPointer()    SetCONTROL(GetCONTROL() & ~0x00000002l)

/***************************************************************************//*!
 * @brief   Executes system reset.
 * @details This macro executes system reset.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#pragma diag_suppress=Pe111
#define SystemReset()         {                                               \
                                SCB_AIRCR = SCB_AIRCR_VECTKEY(0x5FA)|         \
                                            SCB_AIRCR_SYSRESETREQ_MASK;       \
                                while(1);                                     \
                              }

#pragma diag_suppress=Pe175
#define IRQ(x)              ((x)-16)        
#define IPR_INDEX(x)        (uint16)(IRQ(x)>>2)
#define IPR_SHIFT(x)        (uint16)(((IRQ(x)%4)<<3)+ARM_INTERRUPT_LEVEL_BITS)
/***************************************************************************//*!
 * @brief   Enables interrupt request and sets its priority.
 * @details This macro enables interrupt request and sets its priority.
 * @param   src   Select interrupt request:
 *                INT_DMA0,INT_DMA1,INT_DMA2,INT_DMA3,INT_SPI0,INT_SPI1,INT_PMC,
 *                INT_QTIM0,INT_QTIM1,INT_QTIM2,INT_QTIM3,INT_PIT0_PIT1,INT_LLWU,
 *                INT_FTFA,INT_CMP0_CMP1,INT_LCD,INT_ADC,INT_PTx,INT_RNGA,
 *                INT_SCI0_SCI1,INT_SCI2_SCI3,INT_AFE0,INT_AFE1,INT_AFE2,
 *                INT_AFE3,INT_RTC,INT_I2C0_I2C1,INT_EWM,INT_MCG,INT_Watchdog,
 *                INT_LPTMR,INT_PXBAR
 * @param   ip    @ref cm0plus_prilvl
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts, 
 *          @ref DisableInterrupts, @ref NVIC_ClrIsr, @ref NVIC_CallIsr, 
 *          @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr, @ref FAULT_SetIsr 
 ******************************************************************************/
#define NVIC_SetIsr(src,ip) {                                                 \
                              NVIC_ICPR_REG(NVIC_BASE_PTR) |=                 \
                              (uint32)(1 << IRQ(src));                        \
                              NVIC_ISER_REG(NVIC_BASE_PTR) |=                 \
                              (uint32)(1 << IRQ(src));                        \
                              NVIC_IP_REG(NVIC_BASE_PTR,IPR_INDEX(src)) |=    \
                              (uint32)((uint32)(ip)<<IPR_SHIFT(src));         \
                            }

/***************************************************************************//*!
 * @brief   Disables interrupt request.
 * @details This macro disables interrupt request.
 * @param   src   Select interrupt request:
 *                INT_DMA0,INT_DMA1,INT_DMA2,INT_DMA3,INT_SPI0,INT_SPI1,INT_PMC,
 *                INT_QTIM0,INT_QTIM1,INT_QTIM2,INT_QTIM3,INT_PIT0_PIT1,INT_LLWU,
 *                INT_FTFA,INT_CMP0_CMP1,INT_LCD,INT_ADC,INT_PTx,INT_RNGA,
 *                INT_SCI0_SCI1,INT_SCI2_SCI3,INT_AFE0,INT_AFE1,INT_AFE2,
 *                INT_AFE3,INT_RTC,INT_I2C0_I2C1,INT_EWM,INT_MCG,INT_Watchdog,
 *                INT_LPTMR,INT_PXBAR
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts, 
 *          @ref DisableInterrupts, @ref NVIC_SetIsr, @ref NVIC_CallIsr, 
 *          @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr, @ref FAULT_SetIsr 
 ******************************************************************************/
#define NVIC_ClrIsr(src)  NVIC_ICER_REG(NVIC_BASE_PTR)=(uint32)(1<<IRQ(src))

/***************************************************************************//*!
 * @brief   Asserts interrupt request.
 * @details This macro asserts interrupt request.
 * @param   src   Select interrupt request:  
 *                INT_DMA0,INT_DMA1,INT_DMA2,INT_DMA3,INT_SPI0,INT_SPI1,INT_PMC,
 *                INT_QTIM0,INT_QTIM1,INT_QTIM2,INT_QTIM3,INT_PIT0_PIT1,INT_LLWU,
 *                INT_FTFA,INT_CMP0_CMP1,INT_LCD,INT_ADC,INT_PTx,INT_RNGA,
 *                INT_SCI0_SCI1,INT_SCI2_SCI3,INT_AFE0,INT_AFE1,INT_AFE2,
 *                INT_AFE3,INT_RTC,INT_I2C0_I2C1,INT_EWM,INT_MCG,INT_Watchdog,
 *                INT_LPTMR,INT_PXBAR
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts, 
 *          @ref DisableInterrupts, @ref NVIC_SetIsr, @ref NVIC_ClrIsr, 
 *          @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr, @ref FAULT_SetIsr 
 ******************************************************************************/
#define NVIC_CallIsr(src) NVIC_ISPR_REG(NVIC_BASE_PTR)=(uint32)(1<<IRQ(src))

/***************************************************************************//*!
 * @brief   Deasserts pending bits for interrupt request.
 * @details This macro deasserts pending bits for interrupt request.
 * @param   src   Select interrupt request:  
 *                INT_DMA0,INT_DMA1,INT_DMA2,INT_DMA3,INT_SPI0,INT_SPI1,INT_PMC,
 *                INT_QTIM0,INT_QTIM1,INT_QTIM2,INT_QTIM3,INT_PIT0_PIT1,INT_LLWU,
 *                INT_FTFA,INT_CMP0_CMP1,INT_LCD,INT_ADC,INT_PTx,INT_RNGA,
 *                INT_SCI0_SCI1,INT_SCI2_SCI3,INT_AFE0,INT_AFE1,INT_AFE2,
 *                INT_AFE3,INT_RTC,INT_I2C0_I2C1,INT_EWM,INT_MCG,INT_Watchdog,
 *                INT_LPTMR,INT_PXBAR
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts, 
 *          @ref DisableInterrupts, @ref NVIC_SetIsr, @ref NVIC_ClrIsr, 
 *          @ref NVIC_CallIsr, @ref NVIC_GetPendingIsr, @ref FAULT_SetIsr 
 ******************************************************************************/
#define NVIC_ClrPendingIsr(src) {                                             \
                                  NVIC_ICPR_REG(NVIC_BASE_PTR) =              \
                                  (uint32)(1 << IRQ(src));                    \
                                }

/***************************************************************************//*!
 * @brief   Returns pending interrupt state.
 * @details This macro reads and returns pending interrupt state.
 * @param   src   Select interrupt request:  
 *                INT_DMA0,INT_DMA1,INT_DMA2,INT_DMA3,INT_SPI0,INT_SPI1,INT_PMC,
 *                INT_QTIM0,INT_QTIM1,INT_QTIM2,INT_QTIM3,INT_PIT0_PIT1,INT_LLWU,
 *                INT_FTFA,INT_CMP0_CMP1,INT_LCD,INT_ADC,INT_PTx,INT_RNGA,
 *                INT_SCI0_SCI1,INT_SCI2_SCI3,INT_AFE0,INT_AFE1,INT_AFE2,
 *                INT_AFE3,INT_RTC,INT_I2C0_I2C1,INT_EWM,INT_MCG,INT_Watchdog,
 *                INT_LPTMR,INT_PXBAR
 * @return  TRUE  interrupt is pending\n         
 * @return  FALSE interrupt is not pending\n
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts, 
 *          @ref DisableInterrupts, @ref NVIC_SetIsr, @ref NVIC_ClrIsr, 
 *          @ref NVIC_CallIsr, @ref NVIC_ClrPendingIsr,  @ref FAULT_SetIsr 
 ******************************************************************************/
#define NVIC_GetPendingIsr(src) (NVIC_ISPR&((uint32)(1<<(IRQ(src))))>>(IRQ(src)))

/***************************************************************************//*!
 * @brief   Sets the Vector Table Offset Register (VTOR).
 * @details This macro sets the Vector Table Offset Register (VTOR). 
 * @param   offset  32-bit vector table base offset value. This value must be a 
 *                  multiple of 0x100.
 * @note    Implemented as an inline macro.
 ******************************************************************************/
#define NVIC_SetVectorTable(offset)   SCB_VTOR=((uint32)offset)

/***************************************************************************//*!
 * @brief   Sets lower priority of the fault exceptions.
 * @details This macro sets lower priority of the selected fault exceptions. 
 *          Fault exceptions have always the highest priority after reset. 
 * @param   src   Select exception: INT_SVCall, INT_PendableSrvReq, INT_SysTick
 * @param   ip    @ref cm0plus_prilvl
 * @note    Implemented as an inline macro.
 * @see     @ref GetInterruptMask, @ref SetInterruptMask, @ref EnableInterrupts, 
 *          @ref DisableInterrupts, @ref NVIC_SetIsr, @ref NVIC_ClrIsr, 
 *          @ref NVIC_CallIsr, @ref NVIC_ClrPendingIsr,  @ref NVIC_GetPendingIsr
 ******************************************************************************/
#define FAULT_SetIsr(src,ip)                                                  \
{                                                                             \
 if (src==INT_SVCall        )                                                 \
   SCB_SHPR2|=(uint32)(ip<<(24+ARM_INTERRUPT_LEVEL_BITS));                    \
 if (src==INT_PendableSrvReq)                                                 \
   SCB_SHPR3|=(uint32)(ip<<(16+ARM_INTERRUPT_LEVEL_BITS));                    \
 if (src==INT_SysTick       )                                                 \
   SCB_SHPR3|=(uint32)(ip<<(24+ARM_INTERRUPT_LEVEL_BITS));                    \
}
/*! @} End of cm0plus_macros                                                  */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/ 
extern void main (void);

/***********************************************************************/
#endif	/* __ARM_CM0_H */

