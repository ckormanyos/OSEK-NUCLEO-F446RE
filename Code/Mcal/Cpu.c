//===============================================================================================================================
// includes
//===============================================================================================================================
#include "STM32F446RE_Reg.h"

//===============================================================================================================================
// externs
//===============================================================================================================================
extern void STM32F446RE_InitClock(void);


//===============================================================================================================================
// functions
//===============================================================================================================================
void STM32F446RE_InitCpu(void)
{
  /* set the flash wait state (5WS) and Enable caches */
  FLASH_ACR = (5UL<<0) | (1UL<<8) | (1UL<<9) | (1UL<<10);

  /* Initialize the FPU: Enable CP10 and CP11. */
  CPACR |= 0x00F00000UL;

  /* enable faults exceptions */
  SHCSR->bits.u1BUSFAULTENA = 1U;
  SHCSR->bits.u1MEMFAULTENA = 1U;
  SHCSR->bits.u1USGFAULTENA = 1U;


  /* Initialize the clock */
  STM32F446RE_InitClock();
}