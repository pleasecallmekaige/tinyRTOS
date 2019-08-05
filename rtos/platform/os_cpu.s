    .syntax unified
    .thumb

.global  PendSV_Handler
.global  OS_TASK_SW
.global  OSStartHighRdy
.extern  OSTCBCurPtr
.extern  OSTCBHighRdyPtr

.equ  NVIC_INT_CTRL,    0xE000ED04                     @ Interrupt control state register.
.equ  NVIC_PENDSVSET,   0x10000000                     @ Value to trigger PendSV exception.
.equ  OS_NVIC_SYSPRI2,               0xE000ED20        @ System Handler Priority Register 2. 
.equ  OS_NVIC_PENDSV_SYSTICK_PRI,    0xFFFF0000        @ SysTick + PendSV priority level (lowest). 

.equ  SHCSRvalue,    0x70000
.equ  SHCSR,         0xE000ED24


    .type OSStartHighRdy, %function
OSStartHighRdy:
    ldr     r0, =OS_NVIC_SYSPRI2                         @ Set the PendSV exception priority 设置PendSV的优先级为最低
    ldr     r1, =OS_NVIC_PENDSV_SYSTICK_PRI
    str     r1, [r0]

    ldr     r0, =SHCSR                                   @ 打开用法fault,总线fault,存储器管理fault的服务例程使能
    ldr     r1, =SHCSRvalue
    str     r1, [r0]

    movs    r0, #0                                              @ Set the PSP to 0 for initial context switch call
    msr     psp, r0


    ldr     r0, =NVIC_INT_CTRL                                  @ Trigger the PendSV exception (causes context switch)
    ldr     r1, =NVIC_PENDSVSET
    str     r1, [r0]

    cpsie   i                                                   @ Enable interrupts at processor level
OSStartHang:
    b       OSStartHang                                         @ Should never get here



    .type OS_TASK_SW, %function
OS_TASK_SW:
    ldr     r0, =NVIC_INT_CTRL                                  @ Trigger the PendSV exception (causes context switch)
    ldr     r1, =NVIC_PENDSVSET
    str     r1, [r0]
    bx      lr


    .section	.text.PendSV_Handler,"ax",%progbits
    .type PendSV_Handler, %function
PendSV_Handler:
    push {r4,r5,r6,r7}

    mrs  r0, psp
    cbz  r0, PendSV_Handler_NoSave


SaveCurSPtoTCB:
    ldr r4,=OSTCBCurPtr
    ldr r5,[r4]
    mrs r4,psp
    str r4,[r5]

PendSV_Handler_NoSave:
    ldr r6,=OSTCBHighRdyPtr
    ldr r7,[r6]
    ldr r6,[r7]

    msr psp, r6
    ldr r4,=OSTCBCurPtr
    str r7,[r4]

    orr lr, lr, #0x04

    pop {r4,r5,r6,r7}

    bx lr