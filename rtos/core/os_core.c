#include "typedef.h"
#include "os_task.h"
#include "os_core.h"
#include "os_cpu_c.h"
#include "usart.h"

OS_TCB OSTCBArray[32];

uint32 OSReadyTbl = 0;


uint8  OSPrioCur = 0;

uint8  OSPrioHighRdy = 0;

OS_TCB*  OSTCBCurPtr = NULL;

OS_TCB*  OSTCBHighRdyPtr = NULL;

/**
  * author: xumingkai
  * @brief  Update the OSPrioHighRdy value to the current highest priority task's priority
  * 
  * @note   None
  * @param  None
  * @retval None
  */
void OSUpdateHighReadyPrio()
{
    uint8 OSNextTaskPrio;
    for( OSNextTaskPrio = 0;(OSNextTaskPrio < OS_TASKS_MAXNUM) && (!(OSReadyTbl & (0x01<<OSNextTaskPrio))); OSNextTaskPrio ++ );
    OSPrioHighRdy = OSNextTaskPrio;
}

/**
  * author: xumingkai
  * @brief  Initializes the OS
  *         Precreate the TCB array
  * 
  * @note   This function should be used before ISStart and CreateTask.
  * @param  None
  * @retval None
  */
uint8  OSInit (void)
{
    for(int i = 0; i < OS_TASKS_MAXNUM; ++i){
        OSTCBArray[i].TaskStatus = STOP;//设置所有的任务初始化为STOP
    }
    if(OSTaskCreate(IDLE, NULL, &IDLETaskStack[IDLETaskStackSize-1], 31) == 0){
        OS_DEBUG("Task IDLE create successful!\r\n");
    }
    else{
        OS_ERROR("Task IDLE create fail! file %s on line %d\r\n", (uint8 *)__FILE__, __LINE__);
        while(1);
        return -1;
    }
    OSPrioCur = 0xff;//当前还没有处于任何任务下
    return 0;
}

/**
  * author: xumingkai
  * @brief  Start the kernel of the system
  * 
  * @note   None
  * @param  None
  * @retval None
  */
uint8  OSStart (void)
{
    OSUpdateHighReadyPrio();
    OSPrioCur = OSPrioHighRdy;
    OSTCBHighRdyPtr = &OSTCBArray[OSPrioHighRdy];
    OSStartHighRdy();
    return 0;
}

/**
  * author: xumingkai
  * @brief  The task sched function, replace current task as the BHighRdyPtr task
  * 
  * @note   None
  * @param  None
  * @retval None
  */
void OSSched (void)
{
    OS_ENTER_CRITICAL();
    OSUpdateHighReadyPrio(); /* Find the highest priority task in the thread table */
    if(OSPrioHighRdy != OSPrioCur) /* If the task is not currently running, proceed to task scheduling */
    {
        OSTCBCurPtr = &OSTCBArray[OSPrioCur]; /* The purpose is to get the top pointer of the stack with the TCB address of the task cited in compilation */
        OSTCBHighRdyPtr = &OSTCBArray[OSPrioHighRdy];
        OSPrioCur = OSPrioHighRdy; /* Update new OSPrioCur */
        OS_TASK_SW(); /* Scheduling Tasks */
    }
    OS_EXIT_CRITICAL();
}

/**
  * author: xumingkai
  * @brief  Delay function
  * 
  * @note   None
  * @param  None
  * @retval None
  */
void OSDelay(uint32 time)
{
    OS_ENTER_CRITICAL();
    OSTCBArray[OSPrioCur].TaskDelay = time;
    OSReadyTbl &= ~(1<<OSPrioCur);
    OS_EXIT_CRITICAL();
    OSSched();
}

/**
  * author: xumingkai
  * @brief  The IDLE task function
  * 
  * @note   None
  * @param  None
  * @retval None
  */
void IDLE(void* arg)
{
    //(void*) arg;
    for(;;){
        int32 delay = 1000000;
        OS_ENTER_CRITICAL();
        OS_DEBUG("I'm IDLE\r\n");
        OS_EXIT_CRITICAL();
        while(delay--);
    }
}