#include"OsHwPltfm.h"


//-----------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  - 
///
/// \return void 
//-----------------------------------------------------------------------------
void Ostm_Init(void)
{
  /* Set the interval timer mode */
  OSTMxCTL = 0U;
  
  /* set the counter register */
  OSTMxCMP = OSTMx_RELOAD;

  /* Enable the interrupt of OSTMx */
  //ICOSTM0 = 0x47U;
}

//-----------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  - 
///
/// \return void 
//-----------------------------------------------------------------------------
void Ostm_Start(void)
{
  OSTMxTS = 1U;
}


//-----------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  - 
///
/// \return void 
//-----------------------------------------------------------------------------
void Ostm_Stop(void)
{
  OSTMxTT = 1U;
}
