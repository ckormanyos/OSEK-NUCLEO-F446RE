

/*******************************************************************************************************************
** Includes
*******************************************************************************************************************/
#include"OsTcb.h"
#include"OsAPIs.h"
#include "Gpio.h"

#include <stdint.h>

TASK(T1)
{
  LED_GREEN_CFG();
  LED_GREEN_TOGGLE();

  const OsEventMaskType OsWaitEventMask = (OsEventMaskType) EVT_BLINK_LED;

  (void) OS_SetRelAlarm(ALARM_BLINK_LED, (OsTickType) UINT8_C(0), (OsTickType) UINT16_C(997));

  for(;;)
  {
    if(E_OK == OS_WaitEvent(OsWaitEventMask))
    {
      OsEventMaskType Events = (OsEventMaskType) UINT8_C(0);

      (void) OS_GetEvent((OsTaskType) T1, &Events);

      if((Events & EVT_BLINK_LED) == EVT_BLINK_LED)
      {
        OS_ClearEvent(EVT_BLINK_LED);

        LED_GREEN_TOGGLE();
      }
    }
    else
    {
      OS_TerminateTask();
    }
  }
}

TASK(Idle)
{
  for(;;)
  {
    __asm("NOP");
  }
}

ISR(DummyInt)
{
  static uint32 NestingCnt = 0;

  ++NestingCnt;

  //while(1)
  {
    __asm("NOP");
  }
}
