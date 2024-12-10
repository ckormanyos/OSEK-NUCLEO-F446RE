/******************************************************************************************
  Filename    : SysTimer.c
  
  Core        : ARM Cortex®-M4F
  
  MCU         : STM32F446RE
    
  Author      : Chalandi Amine
 
  Owner       : Chalandi Amine
  
  Date        : 26.10.2020
  
  Description : System timer driver implementation
  
******************************************************************************************/

//=========================================================================================
// Types definition
//=========================================================================================
typedef union
{
  struct
  {
    unsigned long u1ENABLE:1;
    unsigned long u1TICKINT:1;
    unsigned long u1CLOCKSRC:1;
    unsigned long :13;
    unsigned long u1COUNTFLAG:1;
    unsigned long :15;
  }bits;

  unsigned long u32Register;

}stStkCtrl;

typedef union
{
  struct
  {
    unsigned long u24RELOAD:24;
    unsigned long :8;
  }bits;

  unsigned long u32Register;

}stStkLoad;

typedef union
{
  struct
  {
    unsigned long u24CURRENT:24;
    unsigned long :8;
  }bits;

  unsigned long u32Register;

}stStkVal;

typedef union
{
  struct
  {
    unsigned long u24TENMS:24;
    unsigned long :6;
    unsigned long u1SKEW:1;
    unsigned long u1NOREF:1;
  }bits;

  unsigned long u32Register;

}stStkCalib;

//=========================================================================================
// Definitions
//=========================================================================================
#define SYS_TICK_BASE_REG (0xE000E010UL)

#define pSTK_CTRL   ((volatile stStkCtrl* const) (SYS_TICK_BASE_REG + 0x00))
#define pSTK_LOAD   ((volatile stStkLoad* const) (SYS_TICK_BASE_REG + 0x04))
#define pSTK_VAL    ((volatile stStkVal* const)  (SYS_TICK_BASE_REG + 0x08))
#define pSTK_CALIB  ((volatile stStkCalib* const)(SYS_TICK_BASE_REG + 0x0C))

#define AHB_FREQ_MHZ      (180U / 8U)
#define SYS_TICK_US(x)    ((unsigned long)(AHB_FREQ_MHZ * (x)) - 1UL)
#define SYS_TICK_MS(x)    ((unsigned long)(AHB_FREQ_MHZ * (x) * 1000UL) - 1UL)

#define SYS_TICK_CLKSRC_AHB          1U
#define SYS_TICK_CLKSRC_AHB_DIV_8    0U
#define SYS_TICK_ENABLE_INT          1U
#define SYS_TICK_ENABLE_TIMER        1U

//=========================================================================================
// Prototypes
//=========================================================================================
void SysTickTimer_Init(void);
void SysTickTimer_Start(void);
void SysTickTimer_Stop(void);

//=========================================================================================
// Functions
//=========================================================================================

//-----------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//-----------------------------------------------------------------------------
void SysTickTimer_Init(void)
{
  pSTK_CTRL->u32Register     = 0;
  pSTK_LOAD->bits.u24RELOAD  = SYS_TICK_MS(10);
  pSTK_VAL->u32Register      = 0;
  pSTK_CTRL->bits.u1CLOCKSRC = SYS_TICK_CLKSRC_AHB_DIV_8;
  pSTK_CTRL->bits.u1TICKINT  = SYS_TICK_ENABLE_INT;
}

//-----------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//-----------------------------------------------------------------------------
void SysTickTimer_Start(void)
{
  pSTK_CTRL->bits.u1ENABLE = SYS_TICK_ENABLE_TIMER;
}

//-----------------------------------------------------------------------------
/// \brief
///
/// \descr
///
/// \param
///
/// \return
//-----------------------------------------------------------------------------
void SysTickTimer_Stop(void)
{
  pSTK_CTRL->bits.u1ENABLE = 0U;
}
