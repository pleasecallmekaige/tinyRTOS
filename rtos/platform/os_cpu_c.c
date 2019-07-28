#include "os_cpu_c.h"
#include "os_core.h"
#include "typedef.h"
#include "os_task.h"
#include "os_event.h"
#include "usart.h"

extern void OSSched (void);

extern uint32 OSReadyTbl;

extern OS_TCB OSTCBArray[OS_TASKS_MAXNUM];

void SysTick_Handler(void)
{
    for (int i=0; i < OS_TASKS_MAXNUM; ++i) {
        if (OSTCBArray[i].TaskStatus != STOP && OSTCBArray[i].TaskDelay !=0) {
            if((--OSTCBArray[i].TaskDelay) == 0){
                if (OSTCBArray[i].TaskStatus == PEND) {
                    OSTCBArray[i].TaskPendStatus = OS_STAT_PEND_TO;
                } else {
                    OSTCBArray[i].TaskPendStatus = OS_STAT_PEND_OK;
                }
                if (OSTCBArray[i].TaskStatus == BLOCKING || OSTCBArray[i].TaskStatus == PEND) {
                    OSReadyTbl |= (1<<OSTCBArray[i].TaskPriority);
                    OSTCBArray[i].TaskStatus = READY;
                }
            }
        }
    }
    OSSched();
    OSTCBCurPtr->TaskStatus = RUNING;
}