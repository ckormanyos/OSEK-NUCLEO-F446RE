// *****************************************************************************************************************
// Filename    : OsAsm.h
// 
// Core        : RENESAS RH850G3KH F1K
//
// Compiler    : GHS
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : OSEK/ECC1 Context switch and ISR category 2 wrapper
// 
// *****************************************************************************************************************


//------------------------------------------------------------------------------------------------------------------
/// \brief  OsGetCurrentSP
///
/// \descr  Get the current stack pointer register value
///
/// \param  void
///
/// \return Current stack pointer register value
//------------------------------------------------------------------------------------------------------------------
__asm unsigned int OsGetCurrentSP(void)
{
  mov sp, r10
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsGetPSW
///
/// \descr  Get the program status register value
///
/// \param  void
///
/// \return Current program status register value
//------------------------------------------------------------------------------------------------------------------
__asm unsigned int OsGetPSW(void)
{
  stsr PSW, r10
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsGetICSR
///
/// \descr  Get the interrupt control status register value
///
/// \param  void
///
/// \return the interrupt control status register value
//------------------------------------------------------------------------------------------------------------------
__asm unsigned int OsGetICSR(void)
{
  stsr ICSR, r10
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osDispatch
///
/// \descr  Generate a software trap
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
__asm void osDispatch(void)
{
   trap 0
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsGetFEIC
///
/// \descr  Get the FEIC register value
///
/// \param  void
///
/// \return the FEIC register value
//------------------------------------------------------------------------------------------------------------------
__asm unsigned int OsGetFEIC(void)
{
  stsr FEIC, r10
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsGetEIIC
///
/// \descr  Get the EIIC register value
///
/// \param  void
///
/// \return the EIIC register value
//------------------------------------------------------------------------------------------------------------------
__asm unsigned int OsGetEIIC(void)
{
  stsr EIIC, r10
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osGetPMR
///
/// \descr  Get current prio mask level
///
/// \param  void
///
/// \return Current Level
//------------------------------------------------------------------------------------------------------------------
__asm uint32 osGetPMR(void)
{
  stsr pmr,r10
  syncm
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osSetPMR
///
/// \descr  Set the prio mask level
///
/// \param  Current Level to be set
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
__asm void osSetPMR(uint32 level)
{
%reg level
  ldsr level, pmr
  syncm
%con level
  mov level, r10
  ldsr r10,pmr
  syncm
%error
}
