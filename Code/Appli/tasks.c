

/*******************************************************************************************************************
** Includes
*******************************************************************************************************************/
#include"OsTcb.h"
#include"OsAPIs.h"
#include "Gpio.h"


//===============================================================================================================================
// OS TASK : T1
//===============================================================================================================================
TASK(T1)
{
  static uint32 T1AliveCounter = 0;
  OsEventMaskType OsWaitEventMask = EVT_BLINK_GREEN_LED_FAST;
  OsEventMaskType Events = 0;
  (void)OS_SetRelAlarm(ALARM_BLINK_GREEN_LED_FAST,0,1000);

  for(;;)
  {
    if(E_OK == OS_WaitEvent(OsWaitEventMask))
    {
      (void)OS_GetEvent((OsTaskType)T1, &Events);

      if((Events & EVT_BLINK_GREEN_LED_FAST) == EVT_BLINK_GREEN_LED_FAST)
      {
        OS_ClearEvent(EVT_BLINK_GREEN_LED_FAST);

        __asm("NOP");

        T1AliveCounter++;
      }
    }
    else
    {
      OS_TerminateTask(); /* In case of error we switch off the task */
    }
  }
}

//===============================================================================================================================
// OS TASK : T2
//===============================================================================================================================
TASK(T2)
{
  static uint32 T2AliveCounter = 0;
  static uint32 LedPeriod      = 0;
  OsEventMaskType OsWaitEventMask = EVT_BLINK_GREEN_LED_SLOW;
  OsEventMaskType Events = 0;
  (void)OS_SetRelAlarm(ALARM_BLINK_GREEN_LED_SLOW,0,5000);
  LED_GREEN_CFG();

  for(;;)
  {
    if(E_OK == OS_WaitEvent(OsWaitEventMask))
    {
      (void)OS_GetEvent((OsTaskType)T2, &Events);

      if((Events & EVT_BLINK_GREEN_LED_SLOW) == EVT_BLINK_GREEN_LED_SLOW)
      {
        OS_ClearEvent(EVT_BLINK_GREEN_LED_SLOW);

        T2AliveCounter++;

        LedPeriod = (((T2AliveCounter % 2) == 0) ? 500U : 50U);


        (void)OS_CancelAlarm(ALARM_BLINK_GREEN_LED_FREQ);
        (void)OS_SetRelAlarm(ALARM_BLINK_GREEN_LED_FREQ, 0, LedPeriod);
      }
    }
    else
    {
      OS_TerminateTask(); /* In case of error we switch off the task */
    }
  }
}

//===============================================================================================================================
// ALARM CALLBACK
//===============================================================================================================================
ALARMCALLBACK(ToggleTheGreenLed)
{
  LED_GREEN_TOGGLE();
}

ISR(DummyInt)
{
  static uint32 NestingCnt = 0;
  
  NestingCnt++;

  //while(1)
  {
   __asm("NOP");
  }
}
