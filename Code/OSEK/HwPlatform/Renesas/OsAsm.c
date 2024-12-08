// *****************************************************************************************************************
// Filename    : OsAsm.c
// 
// Core        : RENESAS RH850G3KH F1K
//
// Compiler    : GHS
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 18.04.2018
// 
// Description : OSEK/ECC1 Context switch and ISR category 2 wrapper for Renesas RH850
// 
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------------------------------------------
void OsDispatcher(void);
void OsCat2IsrWrapper(void);
void OsCat2FEIsrWrapper(void);

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsDispatcher
///
/// \descr  Context switcher
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
#pragma asm

  .align 4
  .globl  _OsDispatcher

_OsDispatcher:
  pushsp r1-r2             -- Store the current context into the stack
  pushsp r6-r31
#if !defined(__SoftwareFloat__) && !defined(__NoFloat__)
  stsr fpsr,r24            -- Store the FPU context
  stsr fpepc,r25
  pushsp r24-r25 
#endif
  stsr eipsw,r26
  stsr eipc,r27
  pushsp r26-r27 
  mov sp,r6                -- Prepare the input parameter for the function OS_Dispatcher
  jarl _OS_Dispatcher,lp   -- Call the dispatcher to switch the context
  mov r10,sp               -- Set the new stack pointer of the active task
  popsp r26-r27            -- Restore the context from the active task
  ldsr r27,eipc 
  ldsr r26,eipsw
#if !defined(__SoftwareFloat__) && !defined(__NoFloat__)
  popsp r24-r25
  ldsr r24,fpsr            -- Restore the FPU context
  ldsr r25,fpepc
#endif
  popsp r6-r31  
  popsp r1-r2  
  eiret
#pragma endasm

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsCat2IsrWrapper
///
/// \descr  Wrapper for all category 2 EI level interrupts
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
#pragma asm

  .align 4
  .globl  _OsCat2IsrWrapper

_OsCat2IsrWrapper:

  pushsp r1-r2                      -- Store the current context into the stack
  pushsp r6-r31
#if !defined(__SoftwareFloat__) && !defined(__NoFloat__)
  stsr fpsr,r24                     -- Store the FPU context
  stsr fpepc,r25
  pushsp r24-r25
#endif
  stsr eipsw,r26
  stsr eipc,r27                     -- store the return PC into the stack
  pushsp r26-r27
  mov sp,r6                         -- prepare the input parameter for the function OsStoreStackPointer
  mov sp,r1
  pushsp r1-r1                      -- store the stack pointer with the saved context (used for nesting levels)
  jarl _OsStoreStackPointer,lp      -- Save the stack pointer of the current task
  jarl _OsRunCat2Isr,lp             -- Call the ISR (lookup table)
  jarl _OsGetSavedStackPointer,lp   -- Restore the stack pointer of the current task
  mov r10,r6
  jarl _OsIntCallDispatch,lp        -- Call dispatcher if needed
  popsp r1-r1
  mov r1,sp                         -- Restore the stack pointer from the saved context (used for nesting levels)
  cmp r10,r0                        -- check if we still in nesting levels context or not ?
  bz __NestingLevelsOnGoing
  mov r10,sp                        -- Set the new stack pointer of the active task
__NestingLevelsOnGoing:
  popsp r26-r27                     -- Restore the context from the active task
  ldsr r27,eipc 
  ldsr r26,eipsw
#if !defined(__SoftwareFloat__) && !defined(__NoFloat__)
  popsp r24-r25
  ldsr r24,fpsr                     -- Restore the FPU context
  ldsr r25,fpepc
#endif
  popsp r6-r31  
  popsp r1-r2 
  eiret

#pragma endasm

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsCat2FEIsrWrapper
///
/// \descr  Wrapper for all category 2 FE level interrupts
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
#pragma asm

  .align 4
  .globl  _OsCat2FEIsrWrapper

_OsCat2FEIsrWrapper:

  pushsp r1-r2                      -- Store the current context into the stack
  pushsp r6-r31
#if !defined(__SoftwareFloat__) && !defined(__NoFloat__)
  stsr fpsr,r24                     -- Store the FPU context
  stsr fpepc,r25
  pushsp r24-r25 
#endif
  stsr fepsw,r26
  stsr fepc,r27
  pushsp r26-r27 
  mov sp,r6                         -- prepare the input parameter for the function OsStoreStackPointer
  jarl _OsStoreStackPointer,lp      -- Save the stack pointer of the current task
  jarl _OsRunCat2FEIsr,lp           -- Call the ISR (lookup table)
  jarl _OsGetSavedStackPointer,lp   -- Restore the stack pointer of the current task
  mov r10,r6
  jarl _OsIntCallDispatch,lp        -- Call dispatcher if needed
  mov r10,sp                        -- Set the new stack pointer of the active task
  popsp r26-r27                     -- Restore the context from the active task
  ldsr r27,fepc 
  ldsr r26,fepsw
#if !defined(__SoftwareFloat__) && !defined(__NoFloat__)
  popsp r24-r25
  ldsr r24,fpsr                     -- Restore the FPU context
  ldsr r25,fpepc
#endif
  popsp r6-r31
  popsp r1-r2
  feret

#pragma endasm

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsStartNewTask
///
/// \descr  This function start an OSEK Task for the 1st execution
///
/// \param  uint32 StackFramePtr, pFunc TaskFuncPtr
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
#pragma asm

  .align 4
  .globl  _OsStartNewTask

_OsStartNewTask:

  mov r6,sp                              -- set the new stack pointer
  ldsr r7,eipc                           -- set the PC register to TaskFuncPtr
  stsr eipsw,r2                          -- load the saved PSW register value
  mov 0x400180E0UL,r1                    -- set the PSW mask
  and r1,r2                              -- reset the flags SAT,CY,OV,S and Z
  ldsr r2,eipsw                          -- set the new PSW register
  mov _osErrTaskExitWithoutTerminate,lp  -- catch abnormal task exit
  eiret                                  -- return from OsDispatcher

#pragma endasm
