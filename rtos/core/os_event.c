#include "os_core.h"
#include "os_event.h"

OS_ECB OSECBArray[OS_TASKS_MAXNUM];
OS_ECB *OSEventFreeList;


inline void OS_EventWaiteListInit(OS_ECB *pevent)
{
    pevent->OSEventTbl = 0;
}

void OS_EventTaskWait(OS_ECB *pevent)
{
    OS_ENTER_CRITICAL();
    pevent->OSEventTbl |= (0x01<<OSPrioCur); // 把事件等待表中对应的任务位设置位1
    OSReadyTbl &= ~(0x01<<OSPrioCur); // 把任务就绪表中对应的位清0
    OSTCBArray[OSPrioCur].TaskStatus = PEND; // TCB中对应的任务状态设为阻塞
    OS_EXIT_CRITICAL();
    OSSched();
}

static uint8 OSGetHighWaitePrio(OS_ECB *pevent)
{
    uint8 NextTaskPrio;
    OS_ENTER_CRITICAL();
    for( NextTaskPrio = 0;(NextTaskPrio < OS_TASKS_MAXNUM) && (!(pevent->OSEventTbl & (0x01<<NextTaskPrio))); NextTaskPrio ++ );
    OS_EXIT_CRITICAL();
    return NextTaskPrio;
}

void OS_EventTaskRdy(OS_ECB *pevent)
{
    uint8 HighWaitePrio;
    HighWaitePrio = OSGetHighWaitePrio(pevent); // 获取事件等待表中优先级最高的任务
    OS_ENTER_CRITICAL();
    pevent->OSEventTbl &= ~(0x01<<HighWaitePrio); // 把事件等待表中对应的任务位设清0
    OSReadyTbl |= (0x01<<HighWaitePrio); // 把任务就绪表中对应的位置1
    OSTCBArray[HighWaitePrio].TaskStatus = READY; // TCB中对应的任务状态设为就绪
    OS_EXIT_CRITICAL();
    OSSched();
}

void OSInitEvent(void)
{
    for (int i = 0; i < OS_TASKS_MAXNUM-1; ++i) {
        OSECBArray[i].OSEventPtr = &OSECBArray[i+1];
        OSECBArray[i].OSEventCnt = 0;
        OSECBArray[i].OSEventType = UNUSED;
    }
    OSECBArray[OS_TASKS_MAXNUM-1].OSEventPtr = NULL;
    OSEventFreeList = &OSECBArray[0];
}

// 必须在OSStart之前调用，任务调度开始时后不能调用。
OS_ECB *OSSemCreate(int16 cnt)
{
    OS_ECB *pevent = NULL;
    pevent = OSEventFreeList;
    if (OSEventFreeList != NULL) {
        OSEventFreeList = OSEventFreeList->OSEventPtr;
    }
    if (pevent != NULL) {
        pevent-> OSEventType = SEM;
        pevent-> OSEventCnt = cnt;
        pevent->OSEventPtr = NULL;
        OS_EventWaiteListInit(pevent);
    }
    return pevent;
}

void OS_EventTO(OS_ECB *pevent)
{
    OS_ENTER_CRITICAL();
    pevent->OSEventTbl &= ~(1<<OSPrioCur); // 如果超时的话,把当前的任务在等待表中清除掉。
    OS_EXIT_CRITICAL();
}

void OSSemPend(OS_ECB *pevent, uint16 timeout, uint8 *perr)
{
    if (pevent->OSEventType != SEM) {
        *perr = OS_ERR_EVENT_TYPE;
        return;
    }

    OS_ENTER_CRITICAL();
    if (pevent->OSEventCnt > 0u) {
        pevent->OSEventCnt--;
        OS_EXIT_CRITICAL();
        *perr = OS_ERR_NONE;
        return;
    }
    OSTCBCurPtr->TaskPendStatus = OS_STAT_PEND_OK;
    OSTCBCurPtr->TaskDelay = timeout;
    OS_EXIT_CRITICAL();

    OS_EventTaskWait(pevent);

    OS_ENTER_CRITICAL();
    switch (OSTCBCurPtr->TaskPendStatus) {
        case OS_STAT_PEND_OK:
            *perr = OS_ERR_NONE;
        break;
        case OS_STAT_PEND_TO:
        default:
            OS_EventTO(pevent);
            *perr = OS_ERR_EVENT_TYPE;
        break;
    }
    OSTCBCurPtr->TaskStatus = RUNING;
    OSTCBCurPtr->TaskPendStatus = OS_STAT_PEND_OK;
    OS_EXIT_CRITICAL();
}

uint8 OSSemPost(OS_ECB *pevent)
{
    if (pevent->OSEventType != SEM) {   /* Validate event block type                     */
        return (OS_ERR_EVENT_TYPE);
    }
    OS_ENTER_CRITICAL();
    if (pevent->OSEventTbl != 0u) {                   /* See if any task waiting for semaphore         */
                                                      /* Ready HPT waiting on event                    */
        OS_EventTaskRdy(pevent);
        OS_EXIT_CRITICAL();
        // OSSched();                                   /* Find HPT ready to run                         */
        return (OS_ERR_NONE);
    }
    if (pevent->OSEventCnt < 65535u) {                /* Make sure semaphore will not overflow         */
        pevent->OSEventCnt++;                         /* Increment semaphore count to register event   */
        OS_EXIT_CRITICAL();
        return (OS_ERR_NONE);
    }
    OS_EXIT_CRITICAL();                               /* Semaphore value has reached its maximum       */
    return (OS_ERR_SEM_OVF);
}