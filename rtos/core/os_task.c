#include "typedef.h"
#include "os_task.h"
#include "stm32f103xe.h"


extern uint32 OSReadyTbl;

extern OS_TCB OSTCBArray[OS_TASKS_MAXNUM];

/**
  * author: xumingkai
  * @brief  Create the Task
  *         
  *         
  * @note   This function should be used only after OSInit.
  * @param  task function, 
  *         p_arg function param
  *         ptos stack point
  *         prio priority of task
  * @retval 0:create successful -1:create fail
  */
int8  OSTaskCreate (void (*task)(void *p_arg), void *p_arg, OS_STACK_PTR p_Stack, uint8 prio)
{
    if(prio < 32 && (OSReadyTbl&(1<<prio)) != 0){
        //Task Create fail, because the prio is exist.
        return -1;
    }

    OSReadyTbl |= (1<<prio);//Set the bit of priority
    OSTCBArray[prio].TaskDelay = 0;
    OSTCBArray[prio].TaskPriority = prio;
    OSTCBArray[prio].TaskStatus = READY;


    *(p_Stack)   = (uint32)0x01000000;//xpsr
    *(--p_Stack) = (uint32)task;//pc
    *(--p_Stack) = (uint32)0xFFFFFFFD;//lr
    *(--p_Stack) = (uint32)0xc;//R12
    *(--p_Stack) = (uint32)0x3;//R3
    *(--p_Stack) = (uint32)0x2;//R2
    *(--p_Stack) = (uint32)0x1;//R1
    *(--p_Stack) = (uint32)0x0;//R0

    OSTCBArray[prio].TaskStackPtr = p_Stack;
    return 0;
}
