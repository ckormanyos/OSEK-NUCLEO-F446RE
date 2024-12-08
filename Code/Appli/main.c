
/*******************************************************************************************************************
** Includes
*******************************************************************************************************************/
#include"OsAPIs.h"

/*******************************************************************************************************************
** Defines
*******************************************************************************************************************/


/*******************************************************************************************************************
** Globals 
*******************************************************************************************************************/


/*******************************************************************************************************************
** Static functions
*******************************************************************************************************************/

/*********************************************************************
*
*       Prototypes (to be placed in a header file such as SWO.h)
*/
void SWO_PrintChar  (char c);
void SWO_PrintString(const char *s);

/*********************************************************************
*
*       Defines for Cortex-M debug unit
*/
#define ITM_STIM_U32 (*(volatile unsigned int*)0xE0000000)    // Stimulus Port Register word acces
#define ITM_STIM_U8  (*(volatile         char*)0xE0000000)    // Stimulus Port Register byte acces
#define ITM_ENA      (*(volatile unsigned int*)0xE0000E00)    // Trace Enable Ports Register
#define ITM_TCR      (*(volatile unsigned int*)0xE0000E80)    // Trace control register

/*********************************************************************
*
*       SWO_PrintChar()
*
* Function description
*   Checks if SWO is set up. If it is not, return,
*    to avoid program hangs if no debugger is connected.
*   If it is set up, print a character to the ITM_STIM register
*    in order to provide data for SWO.
* Parameters
*   c:    The Chacracter to be printed.
* Notes
*   Additional checks for device specific registers can be added.
*/
void SWO_PrintChar(char c) {
  //
  // Check if ITM_TCR.ITMENA is set
  //
  if ((ITM_TCR & 1) == 0) {
    return;
  }
  //
  // Check if stimulus port is enabled
  //
  if ((ITM_ENA & 1) == 0) {
    return;
  }
  //
  // Wait until STIMx is ready,
  // then send data
  //
  while ((ITM_STIM_U8 & 1) == 0);
  ITM_STIM_U8 = c;
}

/*********************************************************************
*
*       SWO_PrintString()
*
* Function description
*   Print a string via SWO.
*
*/
void SWO_PrintString(const char *s) {
  //
  // Print out character per character
  //
  while (*s) {
    SWO_PrintChar(*s++);
  }
}

//USART6 Registers
#define USART6_SR    0x40011400UL
#define USART6_DR    0x40011404UL
#define USART6_BRR   0x40011408UL
#define USART6_CR1   0x4001140CUL
#define USART6_CR2   0x40011410UL
#define USART6_CR3   0x40011414UL
#define USART6_GTPR  0x40011418UL

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
int main(void)
{
  OS_StartOS(APP_MODE_DEFAULT);

  return(0);
}

