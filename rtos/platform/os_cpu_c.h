#ifndef __OS_CPU_C__
#define __OS_CPU_C__

void SysTick_Handler(void);

void PendSV_Handler(void);

void OSStartHighRdy();

void OS_TASK_SW();

#endif