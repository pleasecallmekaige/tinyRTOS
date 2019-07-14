#ifndef __TYPEDEF__
#define __TYPEDEF__

#undef NULL 
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif

/**
 * Common data type
 */
typedef unsigned char           uint8;
typedef unsigned short int      uint16;
typedef unsigned int            uint32;
typedef unsigned char           int8;
typedef unsigned short int      int16;
typedef unsigned int            int32;

typedef uint32*                  OS_STACK_PTR;
#endif