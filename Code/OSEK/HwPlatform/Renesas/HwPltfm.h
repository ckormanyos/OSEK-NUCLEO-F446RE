// *****************************************************************************
// Filename    : HwPltfm.h
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : Hardware Platform definition for Renesas F1x
// 
// *****************************************************************************

#ifndef __HARDWARE_PLATFORM_H__
#define __HARDWARE_PLATFORM_H__

#define OS_GEN_NOT
#include"OsGenCfg.h"
#undef  OS_GEN_NOT

#define ENABLE_INTERRUPTS() __EI()
#define DISABLE_INTERRUPTS() __DI()

#define EXCEPTION_CAUSE_CODE_MASK 0x0000FFFFUL
#define ICSR_MASK 0x03UL

#ifdef R7F701581

  #define FIC       *(volatile unsigned short*)(0xFFFEEA7AUL)
  #define FEINTF    *(volatile unsigned long*)(0xFFC00100UL)
  #define FEINTFC   *(volatile unsigned long*)(0xFFC00108UL)
  #define FEINTFMSK *(volatile unsigned int*)(0xFFC00104UL)
  #define ICOSTM0   *(volatile unsigned short*)(0xFFFFB0A8UL)

  #define OSTM0_BASE  0xFFD70000UL
  #define OSTMx_BASE OSTM0_BASE

  #define ICxxx(x) *(volatile unsigned short*)((x > 31) ? (2*x + 0xFFFEB000UL) : (2*x + 0xFFFEEA00UL))

#endif


#ifdef R7F701035

  #define FIC       *(volatile unsigned short*)(0xFFFF907AUL)
  #define FEINTF    *(volatile unsigned long*)(0xFFF80300UL)
  #define FEINTFC   *(volatile unsigned long*)(0xFFF80308UL)
  #define FEINTFMSK *(volatile unsigned int*)(0xFFF80304UL)
  #define ICOSTM0   *(volatile unsigned short*)(0xFFFFA098UL)

  #define OSTM0_BASE  0xFFEC0000UL
  #define OSTMx_BASE OSTM0_BASE

  #define ICxxx(x) *(volatile unsigned short*)((x > 31) ? (2*x + 0xFFFFA000UL) : (2*x + 0xFFFF9000UL))

#endif

#define OSTMxCMP  *(volatile unsigned int*)(OSTMx_BASE + 0x00UL)
#define OSTMxCNT  *(volatile unsigned int*)(OSTMx_BASE + 0x04UL)
#define OSTMxTE   *(volatile unsigned char*)(OSTMx_BASE + 0x10UL)
#define OSTMxTS   *(volatile unsigned char*)(OSTMx_BASE + 0x14UL)
#define OSTMxTT   *(volatile unsigned char*)(OSTMx_BASE + 0x18UL)
#define OSTMxCTL  *(volatile unsigned char*)(OSTMx_BASE + 0x20UL)
#define OSTMxEMU  *(volatile unsigned char*)(OSTMx_BASE + 0x24UL)

#define PER_FREQ_MHZ    (CPU_FREQ_MHZ / 2U)
#define OSTMx_RELOAD    (SYS_TICK_US * PER_FREQ_MHZ) - 1U


void Ostm_Init(void);
void Ostm_Start(void);
void Ostm_Stop(void);

#endif
