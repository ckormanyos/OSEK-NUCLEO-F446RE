/******************************************************************************************
  Filename    : Memory_Map.ld
  
  Core        : ARM Cortex®-M4F
  
  MCU         : STM32F446RE
    
  Author      : Chalandi Amine
 
  Owner       : Chalandi Amine
  
  Date        : 26.10.2020
  
  Description : Linker description file for STM32F446RE
  
******************************************************************************************/

/******************************************************************************************
 ELF Entrypoint
******************************************************************************************/
ENTRY(Startup_Init)

/******************************************************************************************
 Globals
******************************************************************************************/
__SYS_STACK_SIZE = 1K;


/******************************************************************************************
 Memory configuration
******************************************************************************************/

MEMORY
{
  ROM(rx)  : ORIGIN = 0x08000000, LENGTH = 2M
  RAM(rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/******************************************************************************************
 Sections definition
******************************************************************************************/
SECTIONS
{
  /* Interrupt vector table */
  .intvect :
  {
    . = ALIGN(4);
    PROVIDE(__INTVECT_BASE_ADDRESS = .) ;
    *(.intvect)
    KEEP(*(.intvect))
  } > ROM

  /* Program code (text) */
  .text :
  {
    . = ALIGN(4);
    *(.text)
  } > ROM

  /* Read-only data (.rodata) */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
  } > ROM

 /* Section for constructors */
  .ctors :
  {
    . = ALIGN(4);
    __CTOR_LIST__ = . ;
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
    LONG(-1) ;
    __CTOR_END__ = . ;
  }  > ROM


  /* Section for destructors */
  .dtors :
  {
    . = ALIGN(4);
    __DTOR_LIST__ = . ;
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array))
    LONG(-1) ;
    __DTOR_END__ = . ;
  } > ROM

  /* Runtime clear table */
  .clear_sec :
  {
    . = ALIGN(4);
    PROVIDE(__RUNTIME_CLEAR_TABLE = .) ;
    LONG(0 + ADDR(.bss));   LONG(SIZEOF(.bss));
    LONG(0 + ADDR(.sbss));  LONG(SIZEOF(.sbss));
    LONG(0 + ADDR(.zbss));  LONG(SIZEOF(.zbss));
    LONG(0 + ADDR(.bbss));  LONG(SIZEOF(.bbss));
    LONG(-1);                 LONG(-1);
  } > ROM

  /* Runtime copy table */	
  .copy_sec :
  {
    . = ALIGN(4);
    PROVIDE(__RUNTIME_COPY_TABLE = .) ;
    LONG(LOADADDR(.data));  LONG(0 + ADDR(.data));  LONG(SIZEOF(.data));
    LONG(LOADADDR(.sdata)); LONG(0 + ADDR(.sdata)); LONG(SIZEOF(.sdata));
    LONG(LOADADDR(.zdata)); LONG(0 + ADDR(.zdata)); LONG(SIZEOF(.zdata));
    LONG(LOADADDR(.bdata)); LONG(0 + ADDR(.bdata)); LONG(SIZEOF(.bdata));
    LONG(-1);                 LONG(-1);                  LONG(-1);
  } > ROM 

  /* The ROM-to-RAM initialized data section */
  .data : ALIGN(4)
  {
    *(.data)
    . = ALIGN(32);
  } > RAM  AT>ROM

  /* The uninitialized (zero-cleared) bss section */
  .bss : ALIGN(4)
  {
    *(.bss)
    . = ALIGN(32);
  } > RAM

  /* The ROM-to-RAM initialized sdata section */
  .sdata :
  {
    . = ALIGN(4);
    *(.sdata)
  } > RAM  AT>ROM

  /* The uninitialized (zero-cleared) sbss section */
  .sbss :
  {
    . = ALIGN(4);
    *(.sbss)
  } > RAM

  /* The ROM-to-RAM initialized zdata section */
  .zdata :
  {
    . = ALIGN(4);
    *(.zdata)
  } > RAM  AT>ROM

  /* The uninitialized (zero-cleared) zbss section */
  .zbss :
  {
    . = ALIGN(4);
    *(.zbss)
  } > RAM

  /* The ROM-to-RAM initialized bdata section */
  .bdata :
  {
    . = ALIGN(4);
    *(.bdata)
  } > RAM  AT>ROM

  /* The uninitialized (zero-cleared) bbss section */
  .bbss :
  {
    . = ALIGN(4);
    *(.sbss)
  } > RAM

  /* user-system mode stack definition */
  .stack_sys :
  {
    . = ALIGN(MAX(__SYS_STACK_SIZE , .), 8);
    PROVIDE(__SYS_STACK_TOP = .) ;
  } > RAM
  
}
