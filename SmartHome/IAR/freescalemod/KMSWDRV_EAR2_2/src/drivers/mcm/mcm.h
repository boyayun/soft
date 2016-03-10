/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      mcm.h
 * @version   1.0.2.0
 * @date      Feb-24-2013
 * @brief     Miscellaneous Control Module (MCM) driver header file.
 ******************************************************************************/
#ifndef __MCM_H
#define __MCM_H

/***************************************************************************//*!
 *//*! @addtogroup mcm_master
 * @{
 * @details AXBS Master Assignments
 ******************************************************************************/
#define MCM_CM0_MASTER (uint8)(1<<0)    ///< CM0 Core Master (port number 0)
#define MCM_DMA_MASTER (uint8)(1<<1)    ///< DMA Master (port number 2)
/*! @} End of mcm_master                                                      */

/***************************************************************************//*!
 *//*! @addtogroup mcm_master_attr
 * @{
 * @details Attribute Configuration Master (ATC) definition values. It defines 
 * the privileged/user and secure/nonsecure master attribute configurations. 
 * @note    Loading MCM_MASTER_EN... attributes forces the master to simply 
 * use the internally-generated two attributes. Internal attributes are 
 * controlled by @ref UserMode() and @ref SupervisorMode() macros. 
 ******************************************************************************/
#define MCM_MASTER_FR_PRIV_SECURE                   0  ///< Master forced to {privileged, secure} (default)
#define MCM_MASTER_FR_USER_SECURE                   2  ///< Master forced to {user, secure}
#define MCM_MASTER_FR_USER_NONSEC                   3  ///< Master forced to {user, nonsecure}
#define MCM_MASTER_EN_PRIV_OR_USER_FR_SECURE        4  ///< Enable {privileged or user} and force {secure}
#define MCM_MASTER_EN_PRIV_OR_USER_FR_NONSEC        5  ///< Enable {privileged or user} and force {nonsecure}
#define MCM_MASTER_EN_PRIV_OR_USER_SECURE_OR_NONSEC 6  ///< Enable {privileged or user, secure or nonsecure}
/*! @} End of mcm_master_attr                                                 */

/******************************************************************************
* MCM function and macro definitions
*
*//*! @addtogroup mcm_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Crossbar master access configuration and locking.
 * @details This macro configures access attributes for selected master(s) and 
 * controls access into configuration attributes.   
 * @param   mask   Select one or more OR'ed @ref mcm_master. 
 * @param   attr   @ref mcm_master_attr.
 * @param   lock   TRUE  (configuration locked until next power on reset)\n
 *                 FALSE (configuration left opened for further changes).
 * @note    Implemented as an inline macro.
 * @warning Writing @ref MCM_CM0_MASTER access attributes must take special care. 
 * Typically, the processor would use an @ref mcm_master_attr value of either 
 * @ref MCM_MASTER_FR_PRIV_SECURE or @ref MCM_MASTER_EN_PRIV_OR_USER_SECURE_OR_NONSEC 
 * because the other values can create a configuration where access to system 
 * level resources is not allowed.
 * @see     @ref MCM_LoadPID
 ******************************************************************************/ 
#define MCM_SetMasterAttr(mask,attr,lock)                                     \
{                                                                             \
  register uint32 tmp=0l;                                                     \
  if ((mask) & MCM_CM0_MASTER)                                                \
    tmp|= (((uint32)attr<<MCM_MATCR_ATC0_SHIFT)|                              \
           ((uint32)lock<<MCM_MATCR_RO0_SHIFT));                              \
  if ((mask) & MCM_DMA_MASTER)                                                \
    tmp|= (((uint32)attr<<MCM_MATCR_ATC2_SHIFT)|                              \
          ((uint32)lock<<MCM_MATCR_RO2_SHIFT));                               \
  MCM_MATCR0=tmp;                                                             \
}

/***************************************************************************//*!
 * @brief   Loads process ID register.
 * @details This macro loads process ID register.
 * @param   pid     00h - Reserved for privileged secure tasks\n
 *                  01h - 7Fh Allocated for user secure tasks\n
 *                  80h - FFh Allocated for user nonsecure tasks.
 * @note    Implemented as an inline macro.
 * @see     @ref MCM_SetMasterAttr
 ******************************************************************************/ 
#define MCM_LoadPID(pid)    MCM_PID = pid

/***************************************************************************//*!
 * @brief   Crossbar master arbitration control.
 * @details This macro controls arbitration for crossbar masters.
 * @param   ctrl    FALSE Sets fixed-priority arbitration (default)\n
 *                  TRUE  Sets round-robin arbitration.
 * @note    Implemented as an inline macro.
 ******************************************************************************/ 
#define MCM_MasterArbCtrl(ctrl)                                               \
{                                                                             \
  if  (ctrl==TRUE)  iosetw (MCM_PLACR,MCM_PLACR_ARB_MASK);                    \
  else              ioclrw (MCM_PLACR,MCM_PLACR_ARB_MASK);                    \
}

/***************************************************************************//*!
 * @brief   Initializes compute operation mode (CPO).
 * @details This macro initializes compute operation mode (CPO). Macro waits 
 *          until compute operation acknowledge (CPOACK) bit is asserted notifying 
 *          successful transition to CPO mode. Compute operation wakeup on 
 *          interrupt or exception vector fetch is set optionally. 
 * @param   wakeup  TRUE    Sets compute operation as well as wakeup by any 
 *                          interrupt or exception vector fetch.
 *                  FALSE   Sets compute operation (default).
 * @note    Implemented as an inline macro. 
 * @see     @ref MCM_QuitCPO                                  
 ******************************************************************************/                          
#define MCM_InitCPO(wakeup)                                                   \
{                                                                             \
  if (wakeup==TRUE) iosetw (MCM_CPO,MCM_CPO_CPOREQ_MASK|MCM_CPO_CPOWOI_MASK); \
  else              iosetw (MCM_CPO,MCM_CPO_CPOREQ_MASK                    ); \
  while (!(MCM_CPO & MCM_CPO_CPOACK_MASK));                                   \
}

/***************************************************************************//*!
 * @brief   Quits pending compute operation mode (CPO).
 * @details This macro quits pending compute operation mode (CPO). Macro waits 
 *          until compute operation acknowledge (CPOACK) bit is de-asserted 
 *          notifying successful transition to RUN or VLPR mode.                                  
 * @note    Implemented as an inline macro.
 * @see     @ref MCM_InitCPO                                   
 ******************************************************************************/                          
#define MCM_QuitCPO()                                                         \
{                                                                             \
  ioclrw (MCM_CPO,MCM_CPO_CPOREQ_MASK);                                       \
  while (MCM_CPO & MCM_CPO_CPOACK_MASK);                                      \
} 
/*! @} End of mcm_macro                                                       */
 
#endif /* __MCM_H */ 