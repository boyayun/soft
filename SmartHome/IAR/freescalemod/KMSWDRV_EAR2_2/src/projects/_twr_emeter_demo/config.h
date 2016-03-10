/*****************************************************************************
 * (c) Copyright 2010, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      config.h
 * @author    R55013
 * @version   1.0.1.0
 * @date      Sep-12-2011
 * @brief     Header file.
 ******************************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H

/******************************************************************************
 * Macro definitions                                                          *
 ******************************************************************************/
#define CONFIG_CAL_PENDING()    (((ramcfg.flag&0x00f0)==0x00f0)?TRUE:FALSE)  
#define CONFIG_CAL_DONE()       (((ramcfg.flag&0xffff)==0xffa5)?TRUE:FALSE)  
/******************************************************************************
 * application password - must be entered via FreeMASTER prior recalibration
 * and/or saving parameters into flash
 ******************************************************************************/  
#define PASSWORD  11111                     /*!< default password             */   

 /*****************************************************************************
  * Default calibration structure definition
  *****************************************************************************/
#define PGA_GAIN  PGA_GAIN16                /*!< PGA gain for phase current   */ 
#define I_SCALE   0                         /*!< Up-scaling phase current     */
#define U_SCALE   1                         /*!< Up-scaling phase voltage     */           
#define METER_CL  " C 5-120A"               /*!< Meter class shown on LCD     */ 
#define METER_SN  95                        /*!< Meter serial number          */
#define CAL_CURR  5.0                       /*!< Calibration point - voltage  */                 
#define CAL_VOLT  230.0                     /*!< Calibration point - current  */

/******************************************************************************
 * configuration data structure definition														        *
 ******************************************************************************/
typedef struct
{
  /* basic power meter configuration data                                     */
  uint16      id;         /* Electricity meter serial number                  */
  uint16      tarif;      /* tarif T1=1,T2=2,T3=3 and T4=4                    */ 
  uint16      wh_idx;     /* number of pulses index for Wh generation         */
  uint16      varh_idx;   /* number of pulses index for VARh generation       */ 
  uint16      vrefh;      /* VREFH trimming value                             */
  uint16      vrefl;      /* VREFL trimming value                             */
  
  /* pre-calibration data - user/measured input                               */  
  double      urms_cal;   /* preset calibration voltage [Vrms]                */
  double      urms_msr;   /* measured voltage [Vrms]                          */
  double      irms_cal;   /* preset calibration current [Arms]                */
  double      irms_msr;   /* measured current [Arms]                          */
  double      angle_cal;  /* preset calibration angle [rad] = 45 degrees      */
  double      angle_msr;  /* measured angle between voltage and current [rad] */
  double      P_msr;      /* measured active power [W]                        */
  double      Q_msr;      /* measured reactive power [VAR]                    */
  Frac32      u_msrmax;   /* measured maximum voltage                         */
  Frac32      u_msrmin;   /* measured minimum voltage                         */
  Frac32      i_msrmax;   /* measured maximum current                         */
  Frac32      i_msrmin;   /* measured minimum current                         */
  
  /* post-calibration data - calculated phase delay, offsets and gains        */
  int16       delay;      /* AFE ch3 vs ch1 delay in modulator clocks         */
  Frac32      i_offset;   /* current measurement offset (AFE ch1)             */
  Frac32      i_gain;     /* current measurement gain (AFE ch1)               */
  Frac32      u_offset;   /* voltage measurement offset (AFE ch3)             */
  Frac32      u_gain;     /* voltage measurement gain (AFE ch3)               */
  
  /* configuration flag                                                       */
  uint16      flag;       /* 0xffff=read default configuration data           */
                          /* 0xfff5=perform calibration pre-processing        */
                          /* 0xffa5=calculate calibration data                */
                          /* 0xa5a5=calibration completed and stored          */
} tCONFIG_FLASH_DATA;

typedef struct
{
  long        wh_cnt;     /* active energy counter                            */
  long        varh_cnt;   /* reactive energy counter                          */ 
  uint16      menu_idx;   /* menu index                                       */
  uint16      pulse_out;  /* 0=T1 => XBAR_OUT6, 1=T2 => XBAR_OUT6             */
  uint16      hardfault;  /* hardfault flag:TRUE-occurred, FALSE-didn't occur */                                     
  uint16      flag;       /* 0x5555= valid data                               */
                          /* 0x----= not valid data - initialization needed   */
} tCONFIG_NOINIT_DATA;

/******************************************************************************
 * exported data declarations                                                 *
 ******************************************************************************/
#if defined (__ICCARM__) /* IAR   */
  extern const              tCONFIG_NOINIT_DATA nvmcnt;
  extern __no_init volatile tCONFIG_NOINIT_DATA ramcnt; 
  extern __no_init volatile tCONFIG_FLASH_DATA  ramcfg;
#elif defined(__GNUC__) /* CW GCC */
  extern const              tCONFIG_NOINIT_DATA nvmcnt;
  extern           volatile tCONFIG_NOINIT_DATA ramcnt; 
  extern           volatile tCONFIG_FLASH_DATA  ramcfg;
#endif
  
/******************************************************************************
 * exported function prototypes														                    *
 ******************************************************************************/
extern void CONFIG_ReadFlash      (tCONFIG_FLASH_DATA *ptr);
extern void CONFIG_SaveFlash      (tCONFIG_FLASH_DATA *ptr, uint16 flag);
extern void CONFIG_UpdateOffsets  (tCONFIG_FLASH_DATA *ptr, Frac32 u, Frac32 i);
extern void CONFIG_PreProcessing  (tCONFIG_FLASH_DATA *ptr, double urms, 
                                   double irms, double w, double var);
extern int16 CONFIG_CalcCalibData (tCONFIG_FLASH_DATA *ptr);
#endif /* __CONFIG_H */