//===============================================================================================================================
// includes
//===============================================================================================================================
#include "STM32F446RE_Reg.h"
#include "GPIO.h"

//===============================================================================================================================
// Types definitions
//===============================================================================================================================

/* RCC_PLLCFGR REGISTER */
typedef  struct
{
  unsigned int PLLM:6;
  unsigned int PLLN:9;
  unsigned int :1;
  unsigned int PLLP:2;
  unsigned int :4;
  unsigned int PLLSRC:1;
  unsigned int :1;
  unsigned int PLLQ:4;
  unsigned int PLLR:3;
  unsigned int :1;
}RCC_PLLCFGR_bit;

typedef union
{
  RCC_PLLCFGR_bit bit;
  unsigned int raw;
}RCC_PLLCFGR_Reg;


/* RCC_CR REGISTER */
typedef struct
{
  unsigned int HSION:1;
  unsigned int HSIRDY:1;
  unsigned int :1;
  unsigned int HSITRIM:5;
  unsigned int HSICAL:8;
  unsigned int HSEON:1;
  unsigned int HSERDY:1;
  unsigned int HSEBYP:1;
  unsigned int CSSON:1;
  unsigned int :4;
  unsigned int PLLON:1;
  unsigned int PLLRDY:1;
  unsigned int PLLI2SON:1;
  unsigned int PLLI2SRDY:1;
  unsigned int PLLSAION:1;
  unsigned int PLLSAIRDY:1;
  unsigned int :2;
}
RCC_CR_bit;

typedef union
{
  RCC_CR_bit bit;
  unsigned int raw;
}RCC_CR_Reg;


/* RCC_CFGR REGISTER */

typedef struct
{
  unsigned int SW:2;
  unsigned int SWS:2;
  unsigned int HPRE:4;
  unsigned int :2;
  unsigned int PPRE1:3;
  unsigned int PPRE2:3;
  unsigned int RTCPRE:5;
  unsigned int MCO1:2;
  unsigned int I2SSCR:1;
  unsigned int MCO1PRE:3;
  unsigned int MCO2PRE:3;
  unsigned int MCO2:2;
} 
RCC_CFGR_bit;

typedef union
{
  RCC_CFGR_bit bit;
  unsigned int raw;
}RCC_CFGR_Reg;

//===============================================================================================================================
// ROM Globals
//===============================================================================================================================
static volatile RCC_PLLCFGR_Reg* const pRCC_PLLCFGR = (RCC_PLLCFGR_Reg*)RCC_PLLCFGR_ADR;
static volatile RCC_CR_Reg*      const pRCC_CR_Reg  = (RCC_CR_Reg*)RCC_CR_ADR;
static volatile RCC_CFGR_Reg*    const pRCC_CFGR    = (RCC_CFGR_Reg*)RCC_CFGR_ADR;

//===============================================================================================================================
// Fucntions
//===============================================================================================================================
void STM32F446RE_InitClock(void)
{
  //The maximum frequency of the AHB domain is 180 MHz. 
  //The maximum allowed frequency of the high-speed APB2 domain is 90 MHz. (AHB/2)
  //The maximum allowed frequency of the low-speed APB1 domain is 45 MHz.  (AHB/4)

  /* clear Cfgr */
  pRCC_CFGR->raw = 0;

  /* MCO2 source */
  pRCC_CFGR->bit.MCO2 = 0; //00: System clock (SYSCLK) selected

  /* MCO2 PRE*/
  pRCC_CFGR->bit.MCO2PRE = 0; //0: no division

  /* set AHB to 180MHz */
  pRCC_CFGR->bit.HPRE = 0;

  /* set APB1 to 45MHz */
  pRCC_CFGR->bit.PPRE1 = 5; //AHB clock divided by 4

  /* set APB2 to 90MHz */
  pRCC_CFGR->bit.PPRE2 = 4; //AHB clock divided by 2

  /* Disable the PLL */
  pRCC_CR_Reg->bit.PLLON = 0;
  
  /* wait until the PLL becomes off */
  while(pRCC_CR_Reg->bit.PLLRDY == 1);

  /* set the PLL output to 180MHz */
  pRCC_PLLCFGR->bit.PLLSRC = 0;    //HSI source (16MHz)
  pRCC_PLLCFGR->bit.PLLM   = 8U;   //VCO input  (2MHz)
  pRCC_PLLCFGR->bit.PLLN   = 180U; //VCO output (360MHz) : between 100 and 432 MHz
  pRCC_PLLCFGR->bit.PLLP   = 0U;   //PLL output (180MHz) : Max 180MHz
  pRCC_PLLCFGR->bit.PLLQ   = 8U;   //PLL USB    (45MHz)
  pRCC_PLLCFGR->bit.PLLR   = 2U;   //PLL I2S    (90MHz)

  /* Enable the PLL */
  pRCC_CR_Reg->bit.PLLON = 1;

  /* wait until the PLL becomes on */
  while(pRCC_CR_Reg->bit.PLLRDY == 0);
  
  /* Set the PLL as system clock */
  pRCC_CFGR->bit.SW = 2;

  /* wait until the PLL becomes the system clock */
  while(pRCC_CFGR->bit.SWS != 2);

  /* Enable the clock for GPIO PORT A */
  RCC_AHB1ENR |= (1UL<<0U);

  /* Enable the clock for GPIO PORT D */
  RCC_AHB1ENR |= (1UL<<3U);

  /* Enable the clock for GPIO PORT C */
  RCC_AHB1ENR |= (1UL<<2U);

  /* Output the SysClock on MCO2 */
  PC9_MCO2_OUTPUT();
}
