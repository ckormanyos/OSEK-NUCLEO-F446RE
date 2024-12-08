// *****************************************************************************************************************
// Filename    : OsCore.c
// 
// OS          : OSEK 2.2.3  
// 
// CC          : ECC1/BCC1
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : Operating system services implementation
// 
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------------------------------------------
#include"OsAPIs.h"
#include"OsInternal.h"
#include"OsTcb.h"
#include"OsAsm.h"
#include"OsHwPltfm.h"

//------------------------------------------------------------------------------------------------------------------
// Static function prototypes
//------------------------------------------------------------------------------------------------------------------
static void osInitTimer(void);
static void osStartTimer(void);
static void osIdleLoop(void);

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_StartOS
///
/// \descr  The user can call this system service to start the operating system in a specific mode
///
/// \param  OsAppModeType Mode
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_StartOS(OsAppModeType Mode)
{
  (void) Mode;

  if(TRUE == OsIsInterruptContext())
  {
    return;
  }   
  
  if(OS_NUMBER_OF_TASKS > 0)
  {
    /* INIT TCBs */
    for(int tcbIdx = 0; tcbIdx < OS_NUMBER_OF_TASKS; tcbIdx++)
    {
      /* Init all stacks with the magic marker */
      const uint32 stack_size = OCB_Cfg.pTcb[tcbIdx]->pstack_top - OCB_Cfg.pTcb[tcbIdx]->pstack_bot + sizeof(uint32);

      for(int offset=0;(offset < stack_size);(offset += sizeof(uint32)))
      {
        *(volatile uint32*)((OCB_Cfg.pTcb[tcbIdx]->pstack_bot) + offset) = (uint32)OS_STACK_MAGIC_MARKER;
      }
      
      /* Set default tasks priorities */
      OCB_Cfg.pTcb[tcbIdx]->Prio = OCB_Cfg.pTcb[tcbIdx]->FixedPrio;
    }
    
    /* Init system tick timer */
    osInitTimer();
    
    /* Start all autostart task */
    for(int tcbIdx = 0; tcbIdx < OS_NUMBER_OF_TASKS; tcbIdx++)
    {
      if(OCB_Cfg.pTcb[tcbIdx]->Autostart == OS_AUTOSTART && OCB_Cfg.pTcb[tcbIdx]->TaskStatus == SUSPENDED)
      {
        /* Switch to PRE_READY state */
        OCB_Cfg.pTcb[tcbIdx]->TaskStatus = PRE_READY;

        /* set the current task's ready bit */
        OsSetTaskPrioReady(OCB_Cfg.pTcb[tcbIdx]->Prio, tcbIdx);
        
        /* Update the number of multiple activation */
        if(OCB_Cfg.pTcb[tcbIdx]->TaskType == BASIC)
        {
          OCB_Cfg.pTcb[tcbIdx]->MultipleActivation++;
        }
      }
    }

    /* Init Interrupts */
    osInitInterrupts();
    
    /* Call startup hook function */
    #if(STARTUPHOOK)
    osStartupHook();
    #endif

    /* Enable the interrupts */
    ENABLE_INTERRUPTS();
    
    /* Start system tick timer */
    osStartTimer();

    /* Start all relative autostart alarms */
    for(int alarmIdx = 0; alarmIdx < OS_NUMBER_OF_ALARMS; alarmIdx++)
    {
      if(OCB_Cfg.pAlarm[alarmIdx]->AutoStart == ALARM_AUTOSTART)
      {
        OS_SetRelAlarm((OsAlarmType)alarmIdx,OCB_Cfg.pAlarm[alarmIdx]->InitTicks,OCB_Cfg.pAlarm[alarmIdx]->InitCycles);
      }
    }
        
    /* Call Scheduler */
    (void)OS_Schedule();
    
    /* Set the OS running flag */
    OCB_Cfg.OsIsRunning = OS_TRUE;

    /* Call the dispatcher */
    osDispatch();
  }
  
  /* Infinite loop */
  osIdleLoop();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osInitTimer
///
/// \descr  Init the HW timer used as system tick timer
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void osInitTimer(void)
{
  Ostm_Init();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osStartTimer
///
/// \descr  Start the HW timer used as system tick timer
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void osStartTimer(void)
{
  Ostm_Start();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_Dispatcher
///
/// \descr  Context switch engine
///
/// \param  uint32 StackPtr: Current stack pointer of the active task
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
uint32 OS_Dispatcher(uint32 StackPtr)
{

  /* Save the current stack pointer of the running task before switching the context */
  if(OCB_Cfg.CurrentTaskIdx < OS_INTERNAL_TASK_ID)
  {
    OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer = StackPtr;

    /* Check the stack pointer against stack overflow */
    if( (!(StackPtr <(uint32)(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pstack_top) &&
           StackPtr >= (uint32)(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pstack_bot)))  ||
       (*(uint32*)(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pstack_bot) != OS_STACK_MAGIC_MARKER)
      )
    {
      /* Stack overflow */
      osInternalError(E_OS_STACKFAULT);
    }
  }
  else
  {
    /* save the system stack */
    OCB_Cfg.OsCurrentSystemStackPtr = StackPtr;
  }

  /* Set the new current task */
  OCB_Cfg.CurrentTaskIdx = OCB_Cfg.HighPrioReadyTaskIdx;
  
  if(OCB_Cfg.CurrentTaskIdx < OS_INTERNAL_TASK_ID)  
  {  
    /* check if we need to create a new stack frame for the new task */
    if(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus == PRE_READY)
    {

      /* Update the current task state */
      OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus = RUNNING;

      /* Clear the current task's ready bit */
      OsClearTaskPrioReady(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->Prio);
      
      /* Call osPreTaskHook */
      #if(PRETASKHOOK)
      osPreTaskHook();
      #endif
      
      /* Create Stack Frame for the 1st execution */
      const uint32 NewStackFramePtr = OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pstack_top;
      const pFunc  NewTaskPtr       = OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->function;

      /* Save the new stack ptr */
      OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer = NewStackFramePtr;

      /* Start the new task */
      OsStartNewTask(NewStackFramePtr, NewTaskPtr);

    }
    else if(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus == READY)
    {
      OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus = RUNNING;

      /* Clear the current task's ready bit */
      OsClearTaskPrioReady(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->Prio);
      
      /* Call osPreTaskHook */
      #if(PRETASKHOOK)
      osPreTaskHook();
      #endif
    }
  }
  else
  {
    /* there is no ready task in the system 
       The OS will stay in endless loop until
       an event will be occurred */
    return(OCB_Cfg.OsCurrentSystemStackPtr);
  }
  return(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  Interrupt service routine
///
/// \descr  ISR of system tick interrupt
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
ISR(SysTickTimer)
{
  OCB_Cfg.OsSysTickCounter++;
  osAlarmsManagement();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsStoreStackPointer
///
/// \descr  Store the current stack pointer in case of category 2 interrupt
///
/// \param  uint32 StackPtrValue: Current stack pointer
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OsStoreStackPointer(uint32 StackPtrValue)
{
  OCB_Cfg.OsIsrInterruptLevel = 1;

  OCB_Cfg.OsInterruptNestingDeepth++;

  if(OCB_Cfg.OsInterruptNestingDeepth == 1) /* store only the 1st entrance, the other nesting interrupts will use the current stack */
  {
    if(OCB_Cfg.CurrentTaskIdx < OS_INTERNAL_TASK_ID)
    {  
      OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer = StackPtrValue; /* current task stack will be used */
    }
    else
    {
      OCB_Cfg.OsCurrentSystemStackPtr = StackPtrValue; /* system stack will be used */
    }
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsGetSavedStackPointer
///
/// \descr  return the saved stack pointer in case of category 2 interrupt
///
/// \param  void
///
/// \return uint32 : saved stack pointer
//------------------------------------------------------------------------------------------------------------------
uint32 OsGetSavedStackPointer(void)
{
  OCB_Cfg.OsIsrInterruptLevel = 0;
  
  if(OCB_Cfg.CurrentTaskIdx < OS_INTERNAL_TASK_ID)
  {
    return(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer);
  }
  else
  {
    return(OCB_Cfg.OsCurrentSystemStackPtr);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsIntCallDispatch
///
/// \descr  Call the dispatcher to switch the context if needed after an category 2 interrupt 
///
/// \param  uint32 StackPtr: Current stack pointer
///
/// \return uint32 : The new stack pointer after switching the context otherwise the last saved stack pointer
//------------------------------------------------------------------------------------------------------------------
uint32 OsIntCallDispatch(uint32 StackPtr)
{
  if(OCB_Cfg.OsInterruptNestingDeepth == 0)
  {
    if(OCB_Cfg.OsIntCallDispatcher == 1)
    {
      /* The internal system state is changed by the ISR cat2 (an Event is triggred)
         at this point the OS will switch to the new context (target task context) */

      /* Reset the flag */
      OCB_Cfg.OsIntCallDispatcher = 0;

      /* Call the Dispatcher to execute the context switch process */
      return(OS_Dispatcher(StackPtr));
    }
    else
    {
      /* The system behavior is not changed by the ISR cat2 
         The cpu will restore the context saved by the interrupt*/
      if(OCB_Cfg.CurrentTaskIdx < OS_INTERNAL_TASK_ID)
      {
        /* return the stack pointer of the current task */
        return(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer);
      }
      else
      {
        /* return the system stack pointer */
        return(StackPtr);
      }
    }
  }
  else
  {
    /* in nesting interrupt behavior */
    /* restore the stack pointer from the saved context */
    return(0);
  }
}  

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_GetActiveApplicationMode
///
/// \descr  This service returns the current application mode.
///
/// \param  void
///
/// \return OsAppModeType : The application mode
//------------------------------------------------------------------------------------------------------------------
OsAppModeType OS_GetActiveApplicationMode(void)
{
  return(APP_MODE);
}  

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ShutdownOS
///
/// \descr  The user can call this system service to abort the overall system (e.g. emergency off). 
///         The operating system also calls this function internally, 
///         if it has reached an undefined internal state and is no longer ready to run.
///
/// \param  OsStatusType Error: Error causing the shutdown of the OS
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_ShutdownOS(OsStatusType Error)
{
#if(SHUTDOWNHOOK)
  osShutdownHook(Error);
#else
  (void)Error;
#endif
  
  DISABLE_INTERRUPTS();
  
  /* Kill all tasks */
  for(int tcbIdx = 0; tcbIdx < OS_NUMBER_OF_TASKS; tcbIdx++)
  {
    OCB_Cfg.pTcb[tcbIdx]->TaskStatus = SUSPENDED;
  }
  for(;;);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osIdleLoop
///
/// \descr  This function is called by the dispatcher when no ready task is found.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void osIdleLoop(void)
{
  /* When no ready task is found the CPU will be waiting here for any event */
  for(;;);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsRunCat2Isr
///
/// \descr  This function is the entry point of all category 2 interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OsRunCat2Isr(void)
{
#if(INTERRUPT == 1)

  extern const unsigned int osNbrOfInterrupts;
  const uint32 ExceptionSourceCode = (uint32)OsGetEIIC();

  if(OCB_Cfg.OsInterruptNestingDeepth == 1)
  {
    /* Get the global mask prio */
    OCB_Cfg.OsInterruptSavedLevel = osGetPMR();
  }

  if(IsrLookupTable[ExceptionSourceCode].type == NESTED)
  {
    if(OCB_Cfg.OsInterruptNestingDeepth == 1)
    {
      /* save the nesting level */
      OCB_Cfg.OsInterruptNestingSavedLevel = IsrLookupTable[ExceptionSourceCode].Prio;

      /* Enable nesting for this level */
      osMaskNestedIntPrio(OCB_Cfg.OsInterruptNestingSavedLevel);
    }

    /* Enable interrupt nesting */
    ENABLE_INTERRUPTS();
  }
  else if(IsrLookupTable[ExceptionSourceCode].type == NOT_NESTED)
  {
    /* disable nesting for this level */
    osMaskNonNestedIntPrio(IsrLookupTable[ExceptionSourceCode].Prio);

    /* make this level nested by all cat1 interrupts */
    ENABLE_INTERRUPTS();
  }
  else
  {
    /* nothing to do */
  }
  
  /* Call the cat2 interrupt service routine (ISR) */
  if(osNbrOfInterrupts > ExceptionSourceCode)
  {
    IsrLookupTable[ExceptionSourceCode].IsrFunc();
  }

  OCB_Cfg.OsInterruptNestingDeepth--;

  if(OCB_Cfg.OsInterruptNestingDeepth == 0)
  {
    /* Disable interrupt nesting */
    DISABLE_INTERRUPTS();

    /* Restore the global mask prio */
    osSetPMR(OCB_Cfg.OsInterruptSavedLevel);
  }
  else
  {
    /* restor nesting for this level */
    osMaskNestedIntPrio(OCB_Cfg.OsInterruptNestingSavedLevel);
  }
#endif
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsIsCat2IntContext
///
/// \descr  This function check if the cpu is executing a category 2 interrupt.
///
/// \param  void
///
/// \return boolean: TRUE -> Category 2 interrupt context, FALSE -> none category 2 interrupt context
//------------------------------------------------------------------------------------------------------------------
boolean OsIsCat2IntContext(void)
{
    return((boolean)OCB_Cfg.OsIsrInterruptLevel);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_DisableAllInterrupts
///
/// \descr  Disable all interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_DisableAllInterrupts(void)
{
  DISABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_EnableAllInterrupts
///
/// \descr  enable all interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_EnableAllInterrupts(void)
{
  ENABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_SuspendAllInterrupts
///
/// \descr  Suspend all interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_SuspendAllInterrupts(void)
{
  DISABLE_INTERRUPTS();
}  

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ResumeAllInterrupts
///
/// \descr  Resume all suspended interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_ResumeAllInterrupts(void)
{
  ENABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_SuspendOSInterrupts
///
/// \descr  Suspend all OS interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_SuspendOSInterrupts(void)
{
  /* Get the global mask prio */
  OCB_Cfg.OsInterruptSavedLevel = osGetPMR();

  /* Disable OS interrupts */
  osSetPMR(OS_CAT1_PRIO_MASK);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ResumeOSInterrupts
///
/// \descr  Resume all suspended OS interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_ResumeOSInterrupts(void)
{
  /* Restore the global mask prio */
  osSetPMR(OCB_Cfg.OsInterruptSavedLevel);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsIsr_UndefinedFunc
///
/// \descr  default handler for EI level interrupt
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OsIsr_UndefinedFunc(void)
{
  _osInternalError(E_OS_DISABLEDINT);
  for(;;);
}

//-----------------------------------------------------------------------------
/// \brief  OsGetSystemTicks
///
/// \descr  Get the system tick counter
///
/// \param  void
///
/// \return uint64 
//-----------------------------------------------------------------------------
uint64 OsGetSystemTicksCounter(void)
{
  return(OCB_Cfg.OsSysTickCounter);
}


//-----------------------------------------------------------------------------
/// \brief  OsGetSystemTicksElapsedTime
///
/// \descr  Get the system elapsed time relative to the prvTicks
///
/// \param  prvTicks : perivous system tick
///
/// \return uint64 
//-----------------------------------------------------------------------------
uint64 OsGetSystemTicksElapsedTime(uint64 prvTicks)
{
  return((uint64)(OCB_Cfg.OsSysTickCounter - prvTicks));
}

//-----------------------------------------------------------------------------
/// \brief  osErrTaskExitWithoutTerminate
///
/// \descr  Catch abnormal exit from task function
///
/// \param  void 
///
/// \return void 
//-----------------------------------------------------------------------------
void osErrTaskExitWithoutTerminate(void)
{
  DISABLE_INTERRUPTS();
  for(;;);
}

//-----------------------------------------------------------------------------
/// \brief  osInternalError
///
/// \descr  Catch all internal OS errors
///
/// \param  err 
///
/// \return void 
//-----------------------------------------------------------------------------
void _osInternalError(OsStatusType err)
{
  DISABLE_INTERRUPTS();
  #if(ERRORHOOK)
    osErrorHook(err);
  #else
    for(;;);
  #endif  
}

//-----------------------------------------------------------------------------
/// \brief  osGetMaximumStackUsage
///
/// \descr  Calculate the maximum stack usage during runtime
///
/// \param  TaskId 
///
/// \return uint32 
//-----------------------------------------------------------------------------
uint32 osGetMaximumStackUsage(uint32 TaskId)
{
  uint32* pStack = (uint32*)NULL;
  uint32 Bottom = OCB_Cfg.pTcb[TaskId]->pstack_bot;
  uint32 Top    = OCB_Cfg.pTcb[TaskId]->pstack_top;

  #define ALIGN4_ADDRESS(x) ((x) & (0xFFFFFFFCUL))

  while(sizeof(uint32) != (Top - Bottom))
  {
    pStack = (uint32*)(ALIGN4_ADDRESS(Bottom + ((Top - Bottom + sizeof(uint32)) / 2)));

    if(*pStack != OS_STACK_MAGIC_MARKER)
    {
       Top = (uint32)pStack;
    }
    else
    {
      Bottom = (uint32)pStack;
    }
  }
  return((uint32)(OCB_Cfg.pTcb[TaskId]->pstack_top) - Top + sizeof(uint32));
}

