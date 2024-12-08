/******************************************************************************************
  Filename    : IntVect.c
  
  Core        : ARM Cortex®-M4F
  
  MCU         : STM32F446RE
    
  Author      : Chalandi Amine
 
  Owner       : Chalandi Amine
  
  Date        : 26.10.2020
  
  Description : Interrupt Vector Table for STM32F446RE
  
******************************************************************************************/

//=============================================================================
// linker symbol
//=============================================================================
extern void __SYS_STACK_TOP(void);

//=============================================================================
// extern function prototype
//=============================================================================
extern void Startup_Init(void);
extern void OsDispatcher(void);
extern void OsCat2IsrWrapper(void);
extern void IsrTimer(void);
extern void MemFault(void);
//=============================================================================
// local function prototype
//=============================================================================
void undefinedHandler(void);
void HardFaultdHandler(void);
void NmiHandler(void);
void OsIsr_DummyIntFunc(void);

void ThisIsAnIsrFunction(void) __attribute__((weak, alias("undefinedHandler")));



//=============================================================================
// types definition
//=============================================================================
typedef void (*InterruptHandler)(void);

//=============================================================================
// Interrupt vector table
//=============================================================================
const InterruptHandler __attribute__((section (".intvect"), aligned(512))) InterruptVectorTable[] =
{
  (InterruptHandler)&__SYS_STACK_TOP,    // 0x0000 --> Initial SP
  (InterruptHandler)&Startup_Init,       // 0x0004 --> Reset
  (InterruptHandler)&NmiHandler,         // 0x0008 --> NMI
  (InterruptHandler)&HardFaultdHandler,  // 0x000C --> HardFault
  (InterruptHandler)&MemFault,           // 0x0010 --> MemManage
  (InterruptHandler)&ThisIsAnIsrFunction,   // 0x0014 --> BusFault
  (InterruptHandler)&undefinedHandler,   // 0x0018 --> UsageFault
  (InterruptHandler)&undefinedHandler,   // 0x001C --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x0020 --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x0024 --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x0028 --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x002C --> SVCall
  (InterruptHandler)&undefinedHandler,   // 0x0030 --> Debug Monitor
  (InterruptHandler)&undefinedHandler,   // 0x0034 --> Reserved
  (InterruptHandler)&OsDispatcher,       // 0x0038 --> PendSV
  (InterruptHandler)&OsCat2IsrWrapper,   //(InterruptHandler)&IsrTimer,           // 0x003C --> Systick
  (InterruptHandler)&OsIsr_DummyIntFunc,   // 0x0040 --> WWDG
  (InterruptHandler)&undefinedHandler,   // 0x0044 --> PVD
  (InterruptHandler)&undefinedHandler,   // 0x0048 --> TAMP_STAMP
  (InterruptHandler)&undefinedHandler,   // 0x004C --> RTC_WKUP
  (InterruptHandler)&undefinedHandler,   // 0x0050 --> FLASH
  (InterruptHandler)&undefinedHandler,   // 0x0054 --> RCC
  (InterruptHandler)&undefinedHandler,   // 0x0058 --> EXTI0
  (InterruptHandler)&undefinedHandler,   // 0x005C --> EXTI1
  (InterruptHandler)&undefinedHandler,   // 0x0060 --> EXTI2
  (InterruptHandler)&undefinedHandler,   // 0x0064 --> EXTI3
  (InterruptHandler)&undefinedHandler,   // 0x0068 --> EXTI4
  (InterruptHandler)&undefinedHandler,   // 0x006C --> DMA1_Stream0
  (InterruptHandler)&undefinedHandler,   // 0x0070 --> DMA1_Stream1
  (InterruptHandler)&undefinedHandler,   // 0x0074 --> DMA1_Stream2
  (InterruptHandler)&undefinedHandler,   // 0x0078 --> DMA1_Stream3
  (InterruptHandler)&undefinedHandler,   // 0x007C --> DMA1_Stream4
  (InterruptHandler)&undefinedHandler,   // 0x0080 --> DMA1_Stream5
  (InterruptHandler)&undefinedHandler,   // 0x0084 --> DMA1_Stream6
  (InterruptHandler)&undefinedHandler,   // 0x0088 --> ADC
  (InterruptHandler)&undefinedHandler,   // 0x008C --> CAN1_TX
  (InterruptHandler)&undefinedHandler,   // 0x0090 --> CAN1_RX0
  (InterruptHandler)&undefinedHandler,   // 0x0094 --> CAN1_RX1
  (InterruptHandler)&undefinedHandler,   // 0x0098 --> CAN1_SCE
  (InterruptHandler)&undefinedHandler,   // 0x009C --> EXTI9_5
  (InterruptHandler)&undefinedHandler,   // 0x00A0 --> TIM1_BRK_TIM9
  (InterruptHandler)&undefinedHandler,   // 0x00A4 --> TIM1_UP_TIM10
  (InterruptHandler)&undefinedHandler,   // 0x00A8 --> TIM1_TRG_COM_TIM11
  (InterruptHandler)&undefinedHandler,   // 0x00AC --> TIM1_CC
  (InterruptHandler)&undefinedHandler,   // 0x00B0 --> TIM2
  (InterruptHandler)&undefinedHandler,   // 0x00B4 --> TIM3
  (InterruptHandler)&undefinedHandler,   // 0x00B8 --> TIM4
  (InterruptHandler)&undefinedHandler,   // 0x00BC --> I2C1_EV
  (InterruptHandler)&undefinedHandler,   // 0x00C0 --> I2C1_ER
  (InterruptHandler)&undefinedHandler,   // 0x00C4 --> I2C2_EV
  (InterruptHandler)&undefinedHandler,   // 0x00C8 --> I2C2_ER
  (InterruptHandler)&undefinedHandler,   // 0x00CC --> SPI1
  (InterruptHandler)&undefinedHandler,   // 0x00D0 --> SPI2
  (InterruptHandler)&undefinedHandler,   // 0x00D4 --> USART1
  (InterruptHandler)&undefinedHandler,   // 0x00D8 --> USART2
  (InterruptHandler)&undefinedHandler,   // 0x00DC --> USART3
  (InterruptHandler)&undefinedHandler,   // 0x00E0 --> EXTI15_10
  (InterruptHandler)&undefinedHandler,   // 0x00E4 --> RTC_Alarm
  (InterruptHandler)&undefinedHandler,   // 0x00E8 --> OTG_FS WKUP
  (InterruptHandler)&undefinedHandler,   // 0x00EC --> TIM8_BRK_TIM12
  (InterruptHandler)&undefinedHandler,   // 0x00F0 --> TIM8_UP_TIM13
  (InterruptHandler)&undefinedHandler,   // 0x00F4 --> TIM8_TRG_COM_TIM14
  (InterruptHandler)&undefinedHandler,   // 0x00F8 --> TIM8_CC
  (InterruptHandler)&undefinedHandler,   // 0x00FC --> DMA1_Stream7
  (InterruptHandler)&undefinedHandler,   // 0x0100 --> FMC
  (InterruptHandler)&undefinedHandler,   // 0x0104 --> SDIO
  (InterruptHandler)&undefinedHandler,   // 0x0108 --> TIM5
  (InterruptHandler)&undefinedHandler,   // 0x010C --> SPI3
  (InterruptHandler)&undefinedHandler,   // 0x0110 --> UART4
  (InterruptHandler)&undefinedHandler,   // 0x0114 --> UART5
  (InterruptHandler)&undefinedHandler,   // 0x0118 --> TIM6_DAC
  (InterruptHandler)&undefinedHandler,   // 0x011C --> TIM7
  (InterruptHandler)&undefinedHandler,   // 0x0120 --> DMA2_Stream0
  (InterruptHandler)&undefinedHandler,   // 0x0124 --> DMA2_Stream1
  (InterruptHandler)&undefinedHandler,   // 0x0128 --> DMA2_Stream2
  (InterruptHandler)&undefinedHandler,   // 0x012C --> DMA2_Stream3
  (InterruptHandler)&undefinedHandler,   // 0x0130 --> DMA2_Stream4
  (InterruptHandler)&undefinedHandler,   // 0x0134 --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x0138 --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x013C --> CAN2_TX
  (InterruptHandler)&undefinedHandler,   // 0x0140 --> CAN2_RX0
  (InterruptHandler)&undefinedHandler,   // 0x0144 --> CAN2_RX1
  (InterruptHandler)&undefinedHandler,   // 0x0148 --> CAN2_SCE
  (InterruptHandler)&undefinedHandler,   // 0x014C --> OTG_FS
  (InterruptHandler)&undefinedHandler,   // 0x0150 --> DMA2_Stream5
  (InterruptHandler)&undefinedHandler,   // 0x0154 --> DMA2_Stream6
  (InterruptHandler)&undefinedHandler,   // 0x0158 --> DMA2_Stream7
  (InterruptHandler)&undefinedHandler,   // 0x015C --> USART6
  (InterruptHandler)&undefinedHandler,   // 0x0160 --> I2C3_EV
  (InterruptHandler)&undefinedHandler,   // 0x0164 --> I2C3_ER
  (InterruptHandler)&undefinedHandler,   // 0x0168 --> OTG_HS_EP1_OUT
  (InterruptHandler)&undefinedHandler,   // 0x016C --> OTG_HS_EP1_IN
  (InterruptHandler)&undefinedHandler,   // 0x0170 --> OTG_HS_WKUP
  (InterruptHandler)&undefinedHandler,   // 0x0174 --> OTG_HS
  (InterruptHandler)&undefinedHandler,   // 0x0178 --> DCMI
  (InterruptHandler)&undefinedHandler,   // 0x017C --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x0180 --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x0184 --> FPU
  (InterruptHandler)&undefinedHandler,   // 0x0188 --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x018C --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x0190 --> SPI4
  (InterruptHandler)&undefinedHandler,   // 0x0194 --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x0198 --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x019C --> SAI1
  (InterruptHandler)&undefinedHandler,   // 0x01A0 --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x01A4 --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x01A8 --> Reserved
  (InterruptHandler)&undefinedHandler,   // 0x01AC --> SAI2
  (InterruptHandler)&undefinedHandler,   // 0x01B0 --> QuadSPI
  (InterruptHandler)&undefinedHandler,   // 0x01B4 --> HDMI-CEC
  (InterruptHandler)&undefinedHandler,   // 0x01B8 --> SPDIF-Rx
  (InterruptHandler)&undefinedHandler,   // 0x01BC --> FMPI2C1
  (InterruptHandler)&undefinedHandler    // 0x01C0 --> FMPI2C1 error
};


void undefinedHandler(void)
{
  for(;;);
}

void HardFaultdHandler(void)
{
  for(;;);
}

void NmiHandler(void)
{
  for(;;);
}

void MemFault(void)
{
    while (1)
    {
        __asm("NOP");
    }
}
