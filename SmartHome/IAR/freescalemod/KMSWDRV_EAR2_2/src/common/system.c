/****************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 *************************************************************************//*!
 * @file      system.c
 * @version   1.0.1.0
 * @date      Aug-10-2012      
 * @brief     Kinetis-M system definitions.
 ****************************************************************************/
#include "appconfig.h"
#include "system.h"
#include "common.h"

/****************************************************************************
* Vector Table
*****************************************************************************/
typedef void (*vector_entry)(void);

#if defined (__ICCARM__)    /* IAR                                          */
  extern void __iar_program_start(void);
  #pragma location = ".intvec"
  __root const vector_entry __vector_table[] = 
#elif defined(__GNUC__)     /* CW GCC                                       */
  extern void __gcc_program_start(void);  
  void (* const __vector_table[])() __attribute__ ((section(".intvec"))) =
#endif
{
   VECTOR_000,              /* Initial SP                                   */
   VECTOR_001,              /* Initial PC                                   */
   VECTOR_002,
   VECTOR_003,
   VECTOR_004,
   VECTOR_005,
   VECTOR_006,
   VECTOR_007,
   VECTOR_008,
   VECTOR_009,
   VECTOR_010,
   VECTOR_011,
   VECTOR_012,
   VECTOR_013,
   VECTOR_014,
   VECTOR_015,
   VECTOR_016,
   VECTOR_017,
   VECTOR_018,
   VECTOR_019,
   VECTOR_020,
   VECTOR_021,
   VECTOR_022,
   VECTOR_023,
   VECTOR_024,
   VECTOR_025,
   VECTOR_026,
   VECTOR_027,
   VECTOR_028,
   VECTOR_029,
   VECTOR_030,
   VECTOR_031,
   VECTOR_032,
   VECTOR_033,
   VECTOR_034,
   VECTOR_035,
   VECTOR_036,
   VECTOR_037,
   VECTOR_038,
   VECTOR_039,
   VECTOR_040,
   VECTOR_041,
   VECTOR_042,
   VECTOR_043,
   VECTOR_044,
   VECTOR_045,
   VECTOR_046,
   VECTOR_047
};
// Vector table end  

/****************************************************************************
 * Flash configuration field start
 ****************************************************************************/
#if defined (__ICCARM__)    /* IAR                                          */
  #pragma location = ".memcfg"
  __root const vector_entry __memcfg_table[] = 
#elif defined(__GNUC__)     /* CW GCC                                       */
  void (* const __memcfg_table[])() __attribute__ ((section(".memcfg"))) =
#endif
{    
   CONFIG_1,
   CONFIG_2,
   CONFIG_3,
   CONFIG_4,
};
//Flash configuration field end
  
/****************************************************************************
 * Memory trace buffer start
 ****************************************************************************/
#if defined (MTB_RAM_RESERVE)
  #if !defined(MTB_RAM_SIZE)
    #define MTB_RAM_SIZE 0x40
  #endif
    #if defined (__ICCARM__) /* IAR                                         */
      #pragma location = ".mtbram"
      __root unsigned char __mtb_ram[MTB_RAM_SIZE];
    #elif defined(__GNUC__)  /* CW GCC                                      */
      unsigned char __attribute__((section (".mtbram"))) __mtb_ram[MTB_RAM_SIZE];
  #endif
#endif   
// Memory trace buffer end  

/****************************************************************************
* default_isr(void)
*
* Default ISR definition.
*
* In:  n/a
* Out: n/a
*****************************************************************************/
void default_isr(void) 
{ 
  for (;;)
  {
  }
}
/****************************************************************************
 * End of module                                                            *
 ****************************************************************************/