
#ifndef __PLATFORM_TYPES_H__
#define __PLATFORM_TYPES_H__

typedef unsigned char uint8;
typedef signed char sint8;
typedef unsigned short uint16;
typedef signed short sint16;
typedef unsigned long uint32;
typedef signed long sint32;
typedef unsigned long long uint64;
typedef signed long long sint64;

typedef void (*pFunc)(void);

typedef enum
{
  FALSE = 0,
  TRUE
}boolean;

#define NULL    (void*)0

#endif
