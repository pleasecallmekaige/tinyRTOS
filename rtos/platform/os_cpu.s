.thumb
.syntax unified
.global  OS_TASK_SW
.global  OSStartHighRdy
.extern  OSTCBCurPtr
.extern  OSTCBHighRdyPtr
.extern  OS_TASK_SW

@ .equ  NVIC_INT_CTRL,    0xE000ED04                     @ Interrupt control state register.
@ .equ  NVIC_SYSPRI14,    0xE000ED22                     @ System priority register (priority 14).PendSV的优先级配置地址
@ .equ  NVIC_PENDSV_PRI,  0xFF                           @ PendSV priority value (lowest).
@ .equ  NVIC_PENDSVSET,   0x10000000                     @ Value to trigger PendSV exception.

@ OSStartHighRdy:
@     ldr     R0, =NVIC_SYSPRI14                         @ Set the PendSV exception priority 设置PendSV的优先级为最低
@     ldr     R1, =NVIC_PENDSV_PRI
@     strb    R1, [R0]

@     ldr r4,=OSTCBHighRdyPtr
@     ldr r5,[r4]
@     ldr sp,[r5]
@     b   POP_ALL

OS_TASK_SW:
    mov  r4, pc
    push {r4}
    mrs  r4, primask
    push {r4}
    mrs  r4, xpsr
    push {r4}
    push {r0}
    push {r1}
    push {r2}
    push {r3}
    push {r12}
    push {lr}
SaveCurSPtoTCB:
    ldr r4,=OSTCBCurPtr
    ldr r5,[r4]
    str sp,[r5]

OSStartHighRdy:
    ldr r4,=OSTCBHighRdyPtr
    ldr r5,[r4]
    ldr sp,[r5]
    b   POP_ALL

POP_ALL:
    pop {r4}
    mov lr, r4
    pop {r12}
    pop {r3}
    pop {r2}
    pop {r1}
    pop {r0}
    pop {r4}
    msr xpsr, r4
    pop {r4}
    msr primask, r4
    pop {pc}