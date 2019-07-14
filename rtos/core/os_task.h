#ifndef __OS_TASK__
#define __OS_TASK__

#include "typedef.h"

#define OS_TASKS_MAXNUM 32

#define  OS_ENTER_CRITICAL()  __ASM("CPSID i\n") //Disable Interrupt
#define  OS_EXIT_CRITICAL()   __ASM("CPSIE i\n") //Enable Interrupt

#define IDLETaskStackSize 100
uint32 IDLETaskStack[IDLETaskStackSize];

typedef enum {
    RUNING = 0,
    READY,
    BLOCKING,
    STOP
}TASK_STAT;

/**
 * System task control block struct
 */
typedef struct os_tcb {
    OS_STACK_PTR  TaskStackPtr;//The stack pointer
    OS_STACK_PTR  TaskStackPtrBottom;//The stack bottom pointer

    TASK_STAT  TaskStatus;//Task status
    uint8  TaskPriority;//Task priority
    uint32 TaskDelay;//Task Delay time, In system beats

    struct os_tcb* OSTCBNextPtr;
    struct os_tcb* OSTCBPrevPtr;
}OS_TCB;


int8  OSTaskCreate (void (*task)(void *p_arg), void *p_arg, OS_STACK_PTR p_Stack, uint8 prio);

#endif