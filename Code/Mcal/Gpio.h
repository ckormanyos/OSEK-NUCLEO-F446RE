#ifndef __GPIO_H__
#define __GPIO_H__


//===============================================================================================================================
// Defines
//===============================================================================================================================
#define GPIOA_BASE 0x40020000UL
#define GPIOB_BASE 0x40020400UL
#define GPIOC_BASE 0x40020800UL
#define GPIOD_BASE 0x40020C00UL
#define GPIOE_BASE 0x40021000UL

#define GPIOA_MODER   (GPIOA_BASE + 0x00UL)
#define GPIOA_OSPEEDR (GPIOA_BASE + 0x08UL)
#define GPIOA_ODR     (GPIOA_BASE + 0x14UL)

#define GPIOD_MODER   (GPIOD_BASE + 0x00UL)
#define GPIOD_OSPEEDR (GPIOD_BASE + 0x08UL)
#define GPIOD_ODR     (GPIOD_BASE + 0x14UL)

#define GPIOC_MODER   (GPIOC_BASE + 0x00UL)
#define GPIOC_OSPEEDR (GPIOC_BASE + 0x08UL)
#define GPIOC_ODR     (GPIOC_BASE + 0x14UL)

//===============================================================================================================================
// Type definitions
//===============================================================================================================================

typedef struct
{
  unsigned int MODER0 :2;
  unsigned int MODER1 :2;
  unsigned int MODER2 :2;
  unsigned int MODER3 :2;
  unsigned int MODER4 :2;
  unsigned int MODER5 :2;
  unsigned int MODER6 :2;
  unsigned int MODER7 :2;
  unsigned int MODER8 :2;
  unsigned int MODER9 :2;
  unsigned int MODER10:2;
  unsigned int MODER11:2;
  unsigned int MODER12:2;
  unsigned int MODER13:2;
  unsigned int MODER14:2;
  unsigned int MODER15:2;
}GPIOx_MODER_bit;

typedef struct
{
  unsigned int ODR0 :1;
  unsigned int ODR1 :1;
  unsigned int ODR2 :1;
  unsigned int ODR3 :1;
  unsigned int ODR4 :1;
  unsigned int ODR5 :1;
  unsigned int ODR6 :1;
  unsigned int ODR7 :1;
  unsigned int ODR8 :1;
  unsigned int ODR9 :1;
  unsigned int ODR10:1;
  unsigned int ODR11:1;
  unsigned int ODR12:1;
  unsigned int ODR13:1;
  unsigned int ODR14:1;
  unsigned int ODR15:1;
  unsigned int :15;
}GPIOx_ODR_bit;

typedef struct
{
  unsigned int OSPEEDR0 :2;
  unsigned int OSPEEDR1 :2;
  unsigned int OSPEEDR2 :2;
  unsigned int OSPEEDR3 :2;
  unsigned int OSPEEDR4 :2;
  unsigned int OSPEEDR5 :2;
  unsigned int OSPEEDR6 :2;
  unsigned int OSPEEDR7 :2;
  unsigned int OSPEEDR8 :2;
  unsigned int OSPEEDR9 :2;
  unsigned int OSPEEDR10:2;
  unsigned int OSPEEDR11:2;
  unsigned int OSPEEDR12:2;
  unsigned int OSPEEDR13:2;
  unsigned int OSPEEDR14:2;
  unsigned int OSPEEDR15:2;
}GPIOx_OSPEEDR_bit;

#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

#define GPIO_SPEED  0

//===============================================================================================================================
// ROM Globals
//===============================================================================================================================
static volatile GPIOx_MODER_bit*   const pGPIOD_MODER   = (volatile GPIOx_MODER_bit* const)GPIOD_MODER;
static volatile GPIOx_ODR_bit*     const pGPIOD_ODR     = (volatile GPIOx_ODR_bit* const)GPIOD_ODR;
static volatile GPIOx_OSPEEDR_bit* const pGPIOD_OSPEEDR = (volatile GPIOx_OSPEEDR_bit* const)GPIOD_OSPEEDR;

static volatile GPIOx_MODER_bit*   const pGPIOC_MODER   = (volatile GPIOx_MODER_bit* const)GPIOC_MODER;
static volatile GPIOx_ODR_bit*     const pGPIOC_ODR     = (volatile GPIOx_ODR_bit* const)GPIOC_ODR;
static volatile GPIOx_OSPEEDR_bit* const pGPIOC_OSPEEDR = (volatile GPIOx_OSPEEDR_bit* const)GPIOC_OSPEEDR;

static volatile GPIOx_MODER_bit*   const pGPIOA_MODER   = (volatile GPIOx_MODER_bit* const)GPIOA_MODER;
static volatile GPIOx_ODR_bit*     const pGPIOA_ODR     = (volatile GPIOx_ODR_bit* const)GPIOA_ODR;
static volatile GPIOx_OSPEEDR_bit* const pGPIOA_OSPEEDR = (volatile GPIOx_OSPEEDR_bit* const)GPIOA_OSPEEDR;

//===============================================================================================================================
// LEDs Macros
//===============================================================================================================================
#define LED_GREEN_CFG()     pGPIOA_OSPEEDR->OSPEEDR5 = GPIO_SPEED; pGPIOA_MODER->MODER5 = GPIO_OUTPUT; pGPIOA_ODR->ODR5 = 0
#define LED_GREEN_ON()      pGPIOA_ODR->ODR5 = 1
#define LED_GREEN_OFF()     pGPIOA_ODR->ODR5 = 0
#define LED_GREEN_TOGGLE()  pGPIOA_ODR->ODR5 ^= 1

//===============================================================================================================================
// IO Macros
//===============================================================================================================================
#define PA3_TOGGLE()       pGPIOA_OSPEEDR->OSPEEDR3 = GPIO_SPEED; pGPIOA_MODER->MODER3 = GPIO_OUTPUT; pGPIOA_ODR->ODR3 ^= 1

#define PC9_MCO2_OUTPUT()  pGPIOC_MODER->MODER9 = 2; pGPIOC_OSPEEDR->OSPEEDR9 = 1;



#endif
