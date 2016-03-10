#include "drivers.h"

void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);
  
  /* route core clock to PTF7 for monitoring                                  */
  SIM_SelClkoutSrc (CLKOUT_SRC1);
  PORT_Init (PORTF,PORT_MODULE_ALT3_MODE,PIN7);
  
  while(1);
}
