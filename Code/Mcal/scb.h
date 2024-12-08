/******************************************************************************************
  Filename    : scb.h
  
  Core        : ARM Cortex®-M4F
  
  MCU         : STM32F446RE
    
  Author      : Chalandi Amine
 
  Owner       : Chalandi Amine
  
  Date        : 26.10.2020
  
  Description : ARM system control block
  
******************************************************************************************/
#ifndef __SCB_H__
#define __SCB_H__

//==================================================
// ACTLR
//==================================================
typedef union
{
  struct
  {
    unsigned long u1DISMCYCINT:1;
    unsigned long u1DISDEFWBUF:1;
    unsigned long u1DISFOLD:1;
    unsigned long :5;
    unsigned long u1DISFPCA:1;
    unsigned long u1DISOOFP:1;
    unsigned long :6;
    unsigned long :32;
  }bits;

  unsigned long u32Register;
}sActlrReg;

//==================================================
// CPUID
//==================================================
typedef union
{
  struct
  {
    unsigned long u4REVISION:4;
    unsigned long u12PARTNO:12;
    unsigned long u4CONSTANT:4;
    unsigned long u4VARIANT:4;
    unsigned long u8IMPLEMENTER:8;
  }bits;

  unsigned long u32Register;
}sCpuidReg;

//==================================================
// ICSR
//==================================================
typedef union
{
  struct
  {
    unsigned long u9VECTACTIVE:9;
    unsigned long :2;
    unsigned long u1RETTOBASE:1;
    unsigned long u7VECTPENDING:7;
    unsigned long :3;
    unsigned long u1ISRPENDING:1;
    unsigned long :2;
    unsigned long u1PENDSTCLR:1;
    unsigned long u1PENDSTSET:1;
    unsigned long u1PENDSVCLR:1;
    unsigned long u1PENDSVSET:1;
    unsigned long :2;
    unsigned long u1NMIPENDSET:1;
  }bits;

  unsigned long u32Register;
}sIcsrReg;

//==================================================
// VTOR
//==================================================
typedef union
{
  struct
  {
    unsigned long :9;
    unsigned long u20TBLOFF:20;
    unsigned long u1TBLBASE:1;
    unsigned long :2;
  }bits;

  unsigned long u32Register;
}sVtorReg;

//==================================================
// AIRCR
//==================================================
typedef union
{
  struct
  {
    unsigned long u1VECTRESET:1;
    unsigned long u1VECTCLRACTIVE:1;
    unsigned long u1SYSRESETREQ:1;
    unsigned long :5;
    unsigned long u3PRIGROUP:3;
    unsigned long :4;
    unsigned long u1ENDIANESS:1;
    unsigned long u16VECTKEYSTAT:16;
  }bits;

  unsigned long u32Register;
}sAircrReg;

//==================================================
// SCR
//==================================================
typedef union
{
  struct
  {
    unsigned long :1;
    unsigned long u1SLEEPONEXIT:1;
    unsigned long u1SLEEPDEEP:1;
    unsigned long :1;
    unsigned long u1SEVEONPEND:1;
    unsigned long :27;
  }bits;

  unsigned long u32Register;
}sScrReg;

//==================================================
// CCR
//==================================================
typedef union
{
  struct
  {
    unsigned long u1NONBASETHRDENA:1;
    unsigned long u1USERSETMPEND:1;
    unsigned long :1;
    unsigned long u1UNALIGN_TRP:1;
    unsigned long u1DIV_0_TRP:1;
    unsigned long :3;
    unsigned long u1BFHFNMIGN:1;
    unsigned long u1STKALIGN:1;
    unsigned long :22;
  }bits;

  unsigned long u32Register;
}sCcrReg;

//==================================================
// SHPR1
//==================================================
typedef union
{
  struct
  {
    unsigned long u8PRI_4:8;
    unsigned long u8PRI_5:8;
    unsigned long u8PRI_6:8;
    unsigned long :8;
  }bits;

  unsigned long u32Register;
}sShpr1Reg;

//==================================================
// SHPR2
//==================================================
typedef union
{
  struct
  {
    unsigned long :8;
    unsigned long :8;
    unsigned long :8;
    unsigned long u8PRI_11:8;
  }bits;

  unsigned long u32Register;
}sShpr2Reg;

//==================================================
// SHPR3
//==================================================
typedef union
{
  struct
  {
    unsigned long :8;
    unsigned long :8;
    unsigned long u8PRI_14:8;
    unsigned long u8PRI_15:8;
  }bits;

  unsigned long u32Register;
}sShpr3Reg;

//==================================================
// SHCSR
//==================================================
typedef union
{
  struct
  {
    unsigned long u1MEMFAULTACT:1;
    unsigned long u1BUSFAULTACT:1;
    unsigned long :1;
    unsigned long u1USGFAULTACT:1;
    unsigned long :3;
    unsigned long u1SVCALLACT:1;
    unsigned long u1MONITORACT:1;
    unsigned long :1;
    unsigned long u1PENDSVACT:1;
    unsigned long u1SYSTICKACT:1;
    unsigned long u1USGFAULTPENDED:1;
    unsigned long u1MEMFAULTPENDED:1;
    unsigned long u1BUSFAULTPENDED:1;
    unsigned long u1SVCALLPENDED:1;
    unsigned long u1MEMFAULTENA:1;
    unsigned long u1BUSFAULTENA:1;
    unsigned long u1USGFAULTENA:1;
    unsigned long :13;
  }bits;

  unsigned long u32Register;
}sShcsrReg;

//==================================================
// CFSR
//==================================================
typedef union
{
  struct
  {
    unsigned long u1IACCVIOL:1;
    unsigned long u1DACCVIOL:1;
    unsigned long :1;
    unsigned long u1MUNSTKERR:1;
    unsigned long u1MSTKERR:1;
    unsigned long u1MLSPERR:1;
    unsigned long :1;
    unsigned long u1MMARVALID:1;
    unsigned long u1IBUSERR:1;
    unsigned long u1PRECISERR:1;
    unsigned long u1IMPRECISERR:1;
    unsigned long u1UNSTKERR:1;
    unsigned long u1STKERR:1;
    unsigned long u1LSPERR:1;
    unsigned long :1;
    unsigned long u1BFARVALID:1;
    unsigned long u1UNDEFINSTR:1;
    unsigned long u1INVSTATE:1;
    unsigned long u1INVPC:1;
    unsigned long u1NOCP:1;
    unsigned long :4;
    unsigned long u1UNALIGNED:1;
    unsigned long u1DIVBYZERO:1;
    unsigned long :6;
  }bits;

  unsigned long u32Register;
}sCfsrReg;

//==================================================
// HFSR
//==================================================
typedef union
{
  struct
  {
    unsigned long :1;
    unsigned long u1VECTTBL:1;
    unsigned long :28;
    unsigned long u1FORCED:1;
    unsigned long u1DEBUG_VT:1;
  }bits;

  unsigned long u32Register;
}sHfsrReg;

//==================================================
// MMAR
//==================================================
typedef union
{
  struct
  {
    unsigned long u32MMFAR:32;
  }bits;

  unsigned long u32Register;
}sMmarReg;

//==================================================
// BFAR
//==================================================
typedef union
{
  struct
  {
    unsigned long u32BFAR:32;
  }bits;

  unsigned long u32Register;
}sBfarReg;

//==================================================
// AFSR
//==================================================
typedef union
{
  struct
  {
    unsigned long u32AFSR:32;
  }bits;

  unsigned long u32Register;
}sAfsrReg;


//==================================================
// SCB Registers
//==================================================
#define ACTLR  ((volatile sActlrReg*)(0xE000E008UL))
#define CPUID  ((volatile sCpuidReg*)(0xE000ED00UL))
#define ICSR   ((volatile sIcsrReg*) (0xE000ED04UL))
#define VTOR   ((volatile sVtorReg*) (0xE000ED08UL))
#define AIRCR  ((volatile sAircrReg*)(0xE000ED0CUL))
#define SCR    ((volatile sScrReg*)  (0xE000ED10UL))
#define CCR    ((volatile sCcrReg*)  (0xE000ED14UL))
#define SHPR1  ((volatile sShpr1Reg*)(0xE000ED18UL))
#define SHPR2  ((volatile sShpr2Reg*)(0xE000ED1CUL))
#define SHPR3  ((volatile sShpr3Reg*)(0xE000ED20UL))
#define SHCSR  ((volatile sShcsrReg*)(0xE000ED24UL))
#define CFSR   ((volatile sCfsrReg*) (0xE000ED28UL))
#define HFSR   ((volatile sHfsrReg*) (0xE000ED2CUL))
#define MMAR   ((volatile sMmarReg*) (0xE000ED34UL))
#define BFAR   ((volatile sBfarReg*) (0xE000ED38UL))
#define AFSR   ((volatile sAfsrReg*) (0xE000ED3CUL))

#endif
