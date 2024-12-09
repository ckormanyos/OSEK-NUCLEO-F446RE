chalandi/OSEK-NUCLEO-F446RE
==================

This repository implements an OSEK-like, bare-metal operating system for ARM(R) Cortex(R)-M4. The goals are to achieve near-compatibility with OSEK (ECC1/BCC1) and to provide an intuitive, esay-to-understand sample application that can be extended for your own projects.

Features include:

  - OSEK-like OS implementation with support of most common features.
  - The sample application runs on the STM32F446RE-NUCLEO Board.
  - Tasks and events interact to produce a blinky LED show featuring a straightforware LED blinky show.
  - Use a simple self-written bare-metal startup.
  - Power, clock and port initialization.
  - 1ms timebase derived from the ARM(R) SysTick.
  - Implementation in C99 with absolute minimal use of assembly.
  - A clear and easy-to-understand build system based on free GNUmake/GCC completes this fun and educational project.

This repository provides keen insight on writing your own bare metal operating system from scratch on a modern microcontroller using entirely free tools and open standards.
