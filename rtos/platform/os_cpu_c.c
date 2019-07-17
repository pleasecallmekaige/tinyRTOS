#include "os_cpu_c.h"
#include "typedef.h"
#include "os_task.h"
#include "usart.h"

extern void OSSched (void);

extern uint32 OSReadyTbl;

extern OS_TCB OSTCBArray[OS_TASKS_MAXNUM];

void SysTick_Handler(void)
{
    for(int i=0; i < OS_TASKS_MAXNUM; ++i){
        if(OSTCBArray[i].TaskStatus != STOP && OSTCBArray[i].TaskDelay !=0){
            if((--OSTCBArray[i].TaskDelay) == 0){
                OSReadyTbl |= (1<<OSTCBArray[i].TaskPriority);
                OSSched();
                //OS_DEBUG("task %d 's delay is %d\r\n", i, OSTCBArray[i].TaskDelay);
            }
        }
    }
}