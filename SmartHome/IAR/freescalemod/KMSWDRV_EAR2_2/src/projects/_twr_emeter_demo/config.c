/*****************************************************************************
 * (c) Copyright 2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * @file      config.c
 * @author    R55013
 * @version   1.0.1.0
 * @date      Sep-12-2011
 * @brief     Configuration data management.
 ******************************************************************************/
#include <string.h>
#include <math.h>
#include "drivers.h"
#include "fraclib.h"
#include "meterlib.h"
#include "config.h" 

/******************************************************************************
 * public data definitions
 ******************************************************************************/
/* this variables is stored in parameter section of the flash  addr=0x001fc00 */
#if defined (__ICCARM__) /* IAR   */
  #pragma location = ".config"
  const tCONFIG_FLASH_DATA nvmcfg =
#elif defined(__GNUC__) /* CW GCC */
  const tCONFIG_FLASH_DATA nvmcfg __attribute__ ((section(".config"))) = 
#endif
{
    /* basic power meter configuration data                                   */
    METER_SN,             /* Electricity meter serial number                  */
    1,                    /* tarif T1=1,T2=2,T3=3 and T4=4                    */ 
    8,                    /* number of pulses index for Wh generation   50k   */
    8,                    /* number of pulses index for VARh generation 50k   */ 
    46,                   /* VREFH trimming value - trimmed to 1.200 V on 2.0 */
    3,                    /* VREFL trimming value - trimmed to 0.400 V on 2.0 */
    /* pre-calibration data - user/measured inputs                            */  
    CAL_VOLT,             /* preset calibration voltage [Vrms]                */
    0.0,                  /* measured voltage [Vrms]                          */
    CAL_CURR,             /* preset calibration current [Arms]                */
    0.0,                  /* measured current [Arms]                          */
    0.785398163,          /* preset calibration angle [rad] = 45 degrees      */
    0.0,                  /* measured angle between voltage and current [rad] */
    0.0,                  /* measured active power [W]                        */
    0.0,                  /* measured reactive power [VAR]                    */
    FRAC32(-1.0),         /* measured maximum voltage- for offset calculation */
    FRAC32( 1.0),         /* measured minimum voltage- for offset calculation */
    FRAC32(-1.0),         /* measured maximum current- for offset calculation */
    FRAC32( 1.0),         /* measured minimum current- for offset calculation */   
    /* post-calibration data - calculated phase delay, offsets and gains      */
    0,                    /* AFE ch3 vs ch1 delay in modulator clocks         */
    0l,                   /* current measurement offset (AFE ch1)             */
    FRAC32(-1.0),         /* current measurement gain (AFE ch1)               */
    0l,                   /* voltage measurement offset (AFE ch3)             */
    FRAC32(-1.0),         /* voltage measurement gain (AFE ch3)               */
    /* configuration flag                                                     */
    0xffff                /* 0xffff=read default configuration data           */
};

/* this variables is stored in flash                                          */
const tCONFIG_NOINIT_DATA nvmcnt = 
{
    /* basic power meter configuration data                                   */
    0l,                   /* active energy counter                            */ 
    0l,                   /* reactive energy counter                          */ 
    3,                    /* menu index                                       */
    0,                    /* 0= active energy (T1) => XBAR_OUT6               */
    FALSE,                /* hardfault didn't occur                           */
    /* configuration flag                                                     */
    0x5555                /* 0x5555=configuration valid                       */
};

/* these variables are stored in non-initialized ram                          */
#if defined (__ICCARM__) /* IAR   */
  __no_init volatile tCONFIG_NOINIT_DATA  ramcnt;
  __no_init volatile tCONFIG_FLASH_DATA   ramcfg;
#elif defined(__GNUC__) /* CW GCC */
  volatile tCONFIG_NOINIT_DATA  ramcnt __attribute__ ((section(".noinit")));
  volatile tCONFIG_FLASH_DATA   ramcfg __attribute__ ((section(".noinit")));
#endif

/******************************************************************************
 * public function definitions
 ******************************************************************************/
/***************************************************************************//*!
 * @brief   Reads configuration data from NVM memory conditionally.
 * @param   ptr   - pointer to tCONFIG_DATA to be read
 * @note    Implemented as a function call.
 ******************************************************************************/
void CONFIG_ReadFlash (tCONFIG_FLASH_DATA *ptr)
{
  memcpy ((void*)ptr, (void*)&nvmcfg, sizeof(tCONFIG_FLASH_DATA)); 
}

/***************************************************************************//*!
 * @brief   Writes configuration data to NVM memory.
 * @param   ptr   - pointer to tCONFIG_DATA to be saved
 * @param   flag  - configuration flag 
 * @note    Implemented as a function call.
 ******************************************************************************/
void CONFIG_SaveFlash (tCONFIG_FLASH_DATA *ptr, uint16 flag)
{ 
  DisableInterrupts();
  FTFA_EraseSector ((void*)&nvmcfg);
  ptr->flag = flag;
  FTFA_WriteArray  ((void*)&nvmcfg, (void*)ptr, sizeof(tCONFIG_FLASH_DATA)); 
  EnableInterrupts();
}

/***************************************************************************//*!
 * @brief   Updates offset of the phase voltage and current measurements 
 *          conditionally.
 * @param   ptr   - pointer to tCONFIG_FLASH_DATA
 * @param   u     - phase voltage sample
 * @param   i     - phase current sample
 * @note    Implemented as a function call.
 ******************************************************************************/
void CONFIG_UpdateOffsets (tCONFIG_FLASH_DATA *ptr, Frac32 u, Frac32 i)
{
  if (ptr->flag == 0xfff5) /* update offsets if pre-processing active         */
  { 
    if (ptr->u_msrmax < u) { ptr->u_msrmax = u; } /* find voltage max. value  */
    if (ptr->u_msrmin > u) { ptr->u_msrmin = u; } /* find voltage min. value  */
    if (ptr->i_msrmax < i) { ptr->i_msrmax = i; } /* find current max. value  */
    if (ptr->i_msrmin > i) { ptr->i_msrmin = i; } /* find current min. value  */
  }
}

/***************************************************************************//*!
 * @brief   Pre-processes measurements conditionally.
 * @param   ptr   - pointer to tCONFIG_FLASH_DATA
 * @param   urms  - RMS phase voltage
 * @param   irms  - RMS phase current
 * @param   urms  - RMS phase voltage
 * @param   w     - active power
 * @param   var   - reactive power
 * @note    Implemented as a function call.
 ******************************************************************************/
#define TIMEOUT_IN_SEC(x) x*4
void CONFIG_PreProcessing (tCONFIG_FLASH_DATA *ptr, double urms, double irms, 
                           double w, double var)
{
  static int timeout=0;
  
  if (ptr->flag == 0xfff5)     /* store measurements if pre-processing active */
  {
    ptr->urms_msr    = urms;
    ptr->irms_msr    = irms;
    ptr->P_msr       = w;
    ptr->Q_msr       = var;
    
    /* timeout check - when timeout expires then finish pre-processing state  */
    /* by setting state at which calibration data are calculated after reset  */
    if ((timeout++) > TIMEOUT_IN_SEC(35)) { ptr->flag = 0xffa5; }
  }
}

/***************************************************************************//*!
 * @brief   Calculates calibration data conditionally.
 * @param   ptr   - pointer to tCONFIG_DATA
 * @return  FALSE - error
 *          TRUE  - success
 * @note    Implemented as a function call.
 ******************************************************************************/
int16 CONFIG_CalcCalibData (tCONFIG_FLASH_DATA *ptr)
{  
  /* calculates calibration data if pre-processing completed sucessfully      */
  if (ptr->flag == 0xffa5) 
  {
    /* check calibration conditions to eliminate pre-heating states           */
    if ((ptr->irms_msr >= ptr->irms_cal*1.0) && 
        (ptr->irms_msr <= ptr->irms_cal*1.1) &&
        (ptr->urms_msr >= ptr->urms_cal    ))
    {      
      /* store offsets                                                        */
      ptr->u_offset = (ptr->u_msrmax+ptr->u_msrmin)>>1;
      ptr->i_offset = (ptr->i_msrmax+ptr->i_msrmin)>>1;
      
      /* calculate and store voltage measurement gain (gain >= FRAC32(-1.0))  */
      ptr->u_gain   = FRAC32((-ptr->urms_cal/ptr->urms_msr));
      
      /* calculate and store current measurement gain (gain >= FRAC32(-1.0))  */
      /* constant 0.9998 is the gain adjustment to calibrate to 0.00% error   */
      ptr->i_gain   = FRAC32((-ptr->irms_cal/ptr->irms_msr)*0.9998);
      
      /* calculate and store phase shift delay value                          */
      ptr->angle_msr= atan2 (ptr->Q_msr, ptr->P_msr);
      ptr->delay = (int16)((((double)(ptr->angle_cal-ptr->angle_msr)/          \
                          (2.0*3.141592654*50.0))*(6.144e6/1.0))+0.5);
      
      ptr->flag = 0xa5a5;     /* calibration completed successfully           */
      return TRUE;
    }
    else
      ptr->flag = 0xfff5;     /* reinitiate calibration                       */  
  }
  return FALSE;
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
