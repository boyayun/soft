/*****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      start.c
 * @version   1.0.4.0
 * @date      Jun-8-2013      
 * @brief     startup function (high level part).
 ******************************************************************************/
#include "start.h"
#include "common.h"
#include "drivers.h"   

#define WDOG_DISABLE()                                                        \
{                                                                             \
  /* Write 0xC520 to the unlock register                          */          \
  WDOG_UNLOCK = 0xC520;                                                       \
                                                                              \
  /* Followed by 0xD928 to complete the unlock                    */          \
  WDOG_UNLOCK = 0xD928;                                                       \
                                                                              \
  /* Clear the WDOGEN bit to disable the watchdog                 */          \
  WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;                                  \
}

/******************************************************************************
 * Startup function definition - function calls all of the needed startup     *
 * routines and then branches to the main process.                            *
 ******************************************************************************/
void start(void)
{ 
#if defined(BOOT_HWVLPR_MODE)
  /* Hardware forced VLPR boot mode. The device will boot in VLPR mode after  */
  /* next POR provided VBAT is powered.                                       */
  IRTC_BootVLPR   ();
#else
  /* Activate VLPR mode by software if BOOT_EXE_MODE constant has been        */
  /* defined in the appconfig.h header file.                                  */
  if (NV_FOPT & NV_FOPT_EXE_MODE_MASK) 
  {
    SMC_Init (SMC_MODULE_VLPS_ON_VLLS_ON_CONFIG); /* enable VLPx & VLLSx modes*/
    SMC_SetMode (VLPR);                           /* enter VLPR mode          */
  }
#endif  
  	
  /* Disable watchdog Timer                                                   */
  WDOG_DISABLE    ();
      
  /* Copy any vector or data sections that need to be in RAM                  */
  startup ();
	
  /* Jump to main process                                                     */
  main ();
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/

