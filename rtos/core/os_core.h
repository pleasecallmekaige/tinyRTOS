#ifndef __OS_CORE__
#define __OS_CORE__

#include "typedef.h"

uint8  OSInit (void);

uint8  OSStart (void);

void OSSched (void);

void OSDelay(uint32 time);

void IDLE(void* arg);

#endif