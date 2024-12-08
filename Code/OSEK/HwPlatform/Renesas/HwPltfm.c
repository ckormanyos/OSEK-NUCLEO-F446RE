// *****************************************************************************
// Filename    : HwPltfm.c
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : Hardware Platform implementation for Renesas F1x
// 
// *****************************************************************************
#include<OsTypes.h>
#include<OsHwPltfm.h>

//------------------------------------------------------------------------------------------------------------------
// Extern variables
//------------------------------------------------------------------------------------------------------------------
__attribute__((weak)) const Isr_t IsrLookupTable[];
__attribute__((weak)) ISR(Undefined);



//------------------------------------------------------------------------------------------------------------------
/// \brief  osInitInterrupts
///
/// \descr  Init the cat2 interrupts
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void osInitInterrupts(void)
{ 
  extern const unsigned int osNbrOfInterrupts;

  for(uint32 interrupt=0; interrupt < osNbrOfInterrupts; interrupt++)
  {
    if(IsrLookupTable[interrupt].IsrFunc != pISR(Undefined))
    {
      /* Init the EI level interrupt control registers */
      ICxxx(interrupt) = 0x0040U + (uint16)IsrLookupTable[interrupt].Prio;
    }
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osMaskNestedIntPrio
///
/// \descr  Mask the interrupt prio level
///
/// \param  uint32 PrioLevel
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
#pragma asm

  .section ".text"
  .align 4
  .globl _osMaskNestedIntPrio

_osMaskNestedIntPrio:
                prepare {r24-r25,lp}, 0
                mov 1,r24
                shl r6,r24
                not r24,r25
                add 1,r25
                mov OS_CAT1_PRIO_MASK, r24    -- make the interrupts cat1 non-maskable
                and r24,r25
                ldsr r25, pmr
                dispose 0, {r24-r25,lp}, [lp]

#pragma endasm

//------------------------------------------------------------------------------------------------------------------
/// \brief  osMaskNonNestedIntPrio
///
/// \descr  Mask the interrupt prio level
///
/// \param  uint32 PrioLevel
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
#pragma asm

  .section ".text"
  .align 4
  .globl _osMaskNonNestedIntPrio

_osMaskNonNestedIntPrio:
                         prepare {r24-r25,lp}, 0
                         shl r6,r24
                         not r24,r25
                         mov OS_CAT1_PRIO_MASK, r24    -- make the interrupts cat1 non-maskable
                         and r24,r25
                         ldsr r25, pmr
                         dispose 0, {r24-r25,lp}, [lp]

#pragma endasm
