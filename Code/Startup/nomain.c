#include "Gpio.h"
#include "scb.h"

int main(void);
void SysTickTimer_Init(void);
void SysTickTimer_Start(void);

#if 0

unsigned int OsHwSearchForHighPrio(void);

unsigned long long TestPrio = 0x0000000080000000;
const unsigned long TestPrioSize = sizeof(TestPrio);

/* define a RAM buffer for MPU */
unsigned char __RAM__BUFFER__[256] __attribute__((aligned(256))); /* READ ONLY REGION */

#endif

int main(void)
{
  //(void)OsHwSearchForHighPrio();

  LED_GREEN_CFG();
  SysTickTimer_Init();
  SysTickTimer_Start();

#if 0
  /* configure the MPU */
  #define MPU_BASE_ADDRESS    0xE000ED90UL
  #define MPU_TYPER           (*(volatile unsigned long*)(MPU_BASE_ADDRESS + 0x00UL))
  #define MPU_CTRL            (*(volatile unsigned long*)(MPU_BASE_ADDRESS + 0x04UL))
  #define MPU_RNR             (*(volatile unsigned long*)(MPU_BASE_ADDRESS + 0x08UL))
  #define MPU_RBAR            (*(volatile unsigned long*)(MPU_BASE_ADDRESS + 0x0CUL))
  #define MPU_RASR            (*(volatile unsigned long*)(MPU_BASE_ADDRESS + 0x10UL))

  if(((MPU_TYPER & (0xFFUL << 8U)) >> 8U) == 8U)
  {
    /* MPU is implemented on this microcontroller */
    /* disable the MPU */
    __asm("DMB");
    MPU_CTRL = 0U;

    /* select memory region 0 */
    MPU_RNR = 0U;

    /* set the base address of the memory region 0 */
    MPU_RBAR = (unsigned long)(&__RAM__BUFFER__[0]);

    /* set the attributes and size for memory region 0 */
    MPU_RASR = (7UL << 24U) | (7UL << 1U ) | 1UL;

    /* disable the other regions */
    for(unsigned long idx = 1; idx < 8; idx++)
    {
      MPU_RNR  = idx;
      MPU_RBAR = 0UL;
      MPU_RASR = 0UL;
    }

    /* enable the MPU */
    MPU_CTRL = (1UL << 2U) | (1UL << 0U);
    __asm("DSB");
    __asm("ISB");

    /* expect an exception */
    __RAM__BUFFER__[0] = 5U;

  }
  else
  {
    /* MPU is not implemented on this microcontroller */
  }

#endif

  while(1)
  {
    __asm("NOP");
  }
  return 0;
}

 void IsrTimer(void)
{
  LED_GREEN_TOGGLE();
}


