/**************************************************************************************
 * Filter–Based Metering Library Configuration Header File, Created: Tue Oct 08 14:45:01 2013
 **************************************************************************************
 * @TAGNAME       = METERLIB1PH_CFG
 * @LOCKED        =         0
 * @FSAMPLE       =      1200
 * @DFACTOR       =         1
 * @IMAX          =  215.0000
 * @UMAX          =  405.0000
 * @FREQ          =        50
 * @COUNTERS_RES  =    100000
 * @PWR_THRESHOLD =    0.5000
 * @APWR_OFS      =    0.0300
 * @RPWR_OFS      =    0.0150
 * @IMP_PER_KWH   =     50000
 * @IMP_PER_KVARH =     50000
 * @HPF_FCUT      =    0.5000
 * @LPF1_FCUT     =    0.4000
 * @LPF2_FCUT     =    3.0000
 * @KWIN_BETA     =    6.0672
 * @KWIN_GAIN     =    1.0000
 * @FIR_TAPS_CHG  =         0
 * @CUR_SENSOR    =         1
 * @LIB_TYPE      =         1
 **************************************************************************************/
#ifndef __METERLIB1PH_CFG_H
#define __METERLIB1PH_CFG_H

/**************************************************************************************
 * General parameters and scaling coefficients
 **************************************************************************************/
#define I_MAX          85.000          /*!< Maximal current I-peak                    */
#define U_MAX         420.000          /*!< Maximal voltage U-peak                    */
#define CALCFREQ     1200.000          /*!< Sample frequency in Hz                    */
#define IMP_PER_KWH     50000          /*!< Impulses per kWh                          */
#define IMP_PER_KVARH   50000          /*!< Impulses per kVARh                        */
#define DECIM_FACTOR        1          /*!< Auxiliary calculations decimation factor  */
/**************************************************************************************
 * Filter-based metering algorithm configuration structure
 **************************************************************************************/
#define METERLIB1PH_CFG                                                               \
{                                                                                     \
  U_MAX,                                                                              \
  I_MAX,                                                                              \
  PWR_SENS_THR(   0.500),                                                             \
  1,                                                                                  \
  {FRAC32(+0.99869271354830),FRAC32(-0.99869271354830),FRAC32(-0.99738542709660)},    \
  {FRAC32(+0.13165249758740),FRAC32(+0.13165249758740),FRAC32(-1.0)},                 \
  {{0l,0ll},{0l,0ll}},                                                                \
  {0l,0ll},                                                                           \
  {{0l,0ll},{0l,0ll}},                                                                \
  {  49,                                                                              \
    {                                                                                 \
      FRAC32(0.0),FRAC32(-0.00073728465714),FRAC32(0.0),FRAC32(-0.00196750272687),    \
      FRAC32(0.0),FRAC32(-0.00411945802255),FRAC32(0.0),FRAC32(-0.00756839142185),    \
      FRAC32(0.0),FRAC32(-0.01278720365088),FRAC32(0.0),FRAC32(-0.02040684105768),    \
      FRAC32(0.0),FRAC32(-0.03136483560542),FRAC32(0.0),FRAC32(-0.04728105184137),    \
      FRAC32(0.0),FRAC32(-0.07151114503989),FRAC32(0.0),FRAC32(-0.11276139617420),    \
      FRAC32(0.0),FRAC32(-0.20318408017719),FRAC32(0.0),FRAC32(-0.63356345988777),    \
      FRAC32(0.0),FRAC32(+0.63356345988777),FRAC32(0.0),FRAC32(+0.20318408017719),    \
      FRAC32(0.0),FRAC32(+0.11276139617420),FRAC32(0.0),FRAC32(+0.07151114503989),    \
      FRAC32(0.0),FRAC32(+0.04728105184137),FRAC32(0.0),FRAC32(+0.03136483560542),    \
      FRAC32(0.0),FRAC32(+0.02040684105768),FRAC32(0.0),FRAC32(+0.01278720365088),    \
      FRAC32(0.0),FRAC32(+0.00756839142185),FRAC32(0.0),FRAC32(+0.00411945802255),    \
      FRAC32(0.0),FRAC32(+0.00196750272687),FRAC32(0.0),FRAC32(+0.00073728465714),    \
      FRAC32(0.0),                                                                    \
    },                                                                                \
     25,                                                                              \
    {                                                                                 \
      FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),        \
      FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),        \
      FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),        \
      FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),        \
      FRAC16(-1.0),                                                                   \
    }                                                                                 \
  },                                                                                  \
  {                                                                                   \
    {                                                                                 \
      0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,  \
      0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,           \
    },                                                                                \
    0ll,                                                                              \
    {                                                                                 \
      0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,0l,     \
    },                                                                                \
    0l,                                                                               \
  },                                                                                  \
  {                                                                                   \
    {FRAC32(+0.00104610245766),FRAC32(+0.00104610245766),FRAC32(-0.99790779508468)},  \
    {FRAC32(+0.00104610245766),FRAC32(+0.00104610245766),FRAC32(-0.99790779508468)},  \
  },                                                                                  \
  {0ll,0ll,0l,0ll},                                                                   \
  {0ll,0ll,0l,0ll},                                                                   \
  {0ll,0ll,0l,0ll},                                                                   \
  {0ll,0ll,0l,0ll},                                                                   \
  {                                                                                   \
    FRAC48((+0.0300/(U_MAX*I_MAX))),KWH_DISP_RES  (  100000),0ll,0ll,0l,FRAC16(-1.0), \
    {FRAC32(+0.00779293629195),FRAC32(+0.00779293629195),FRAC32(-0.98441412741610)},  \
    0ll,0ll,                                                                          \
  },                                                                                  \
  {                                                                                   \
    FRAC48((+0.0150/(U_MAX*I_MAX))),KVARH_DISP_RES(  100000),0ll,0ll,0l,FRAC16(-1.0), \
    {FRAC32(+0.00779293629195),FRAC32(+0.00779293629195),FRAC32(-0.98441412741610)},  \
    0ll,0ll,                                                                          \
  }                                                                                   \
}
#endif  /* __METERLIB1PH_CFG_H */