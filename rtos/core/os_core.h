#ifndef __OS_CORE__
#define __OS_CORE__

#ifdef CORE_GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif

#include "typedef.h"
#include "os_task.h"

EXTERN OS_TCB OSTCBArray[OS_TASKS_MAXNUM];

EXTERN uint32 OSReadyTbl;

EXTERN uint8  OSPrioCur;

EXTERN uint8  OSPrioHighRdy;

EXTERN OS_TCB*  OSTCBCurPtr;

EXTERN OS_TCB*  OSTCBHighRdyPtr;

uint8  OSInit (void);

uint8  OSStart (void);

void OSSched (void);

void OSDelay(uint32 time);

void IDLE(void* arg);

#endif