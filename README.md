# 一个最小RTOS需要哪些东西
<!-- TOC -->

- [一个最小RTOS需要哪些东西](#一个最小rtos需要哪些东西)
    - [系统要求](#系统要求)
        - [调度方式为抢占式调度](#调度方式为抢占式调度)
        - [栈生长方向](#栈生长方向)
    - [1、需要一个能运行的裸机程序](#1需要一个能运行的裸机程序)
    - [2、首先实现最基本任务切换功能](#2首先实现最基本任务切换功能)
        - [任务控制块TCB](#任务控制块tcb)
        - [全局变量和数据结构](#全局变量和数据结构)
        - [任务初始化](#任务初始化)
        - [系统心跳（时钟节拍）](#系统心跳时钟节拍)
        - [任务栈结构](#任务栈结构)
        - [内核态和线程态的切换](#内核态和线程态的切换)
    - [3、任务间通信（事件）](#3任务间通信事件)
        - [事件控制块](#事件控制块)
        - [空事件控制块](#空事件控制块)
        - [信号量](#信号量)
        - [互斥信号量](#互斥信号量)
        - [消息邮箱](#消息邮箱)
- [遇到的困难](#遇到的困难)

<!-- /TOC -->

## 系统要求  
### 调度方式为抢占式调度  
调度的概念，通俗的说就是系统在多个任务中选择合适的任务执行。系统如何知道何时该执行哪个任务？可以为每个任务安排一唯一的优先级别，当同时有 多个任务就绪时，优先运行优先级较高的任务。  
  
所谓“抢占式调度 ”是指一旦就绪状态中出现优先权更高的任务，便立即剥夺当前任务的运行权，把把CPU分配给更高优先级的任务。样这样CPU总是执行处于就绪条件下优先级最高的任务。  

### 栈生长方向  
本项目采用的芯片是STM32F103ZET,栈生长方向向下。  

## 1、需要一个能运行的裸机程序  
一个能点亮LED的程序

## 2、首先实现最基本任务切换功能  
### 任务控制块TCB  
- 当前任务的状态（就绪，执行，阻塞）  
- 任务的优先级  
- 阻塞节拍数  
- 任务保存的栈顶指针指向任务栈。
（每个任务又要有一个自己的任务栈）  
- 预定义好一个长度位32的数组。（该系统目前只支持最多32个任务）

### 全局变量和数据结构
- 任务就绪表  
- 任务优先级表  
- 当前任务TCB指针  
- 当前任务优先级  
- 当前最高优先级的就绪任务TCB指针  
- 当前最高优先级的就绪任务


### 任务初始化  
- 将所有全局变量和数据结构初始化
- 创建IDLE，并赋予最低优先级和永远就绪状态


### 系统心跳（时钟节拍） 
采用一个定时器产生时钟节拍  
中断处理  
遍历每一个任务  
查看当前系统的每个不为STOP的任务的delay时间。
把每个不为0的delay减一。  
当有delay减到0时，把该任务置位就绪


### 任务栈结构  
    *(p_Stack)   = (uint32)0x01000000;//xpsr
    *(--p_Stack) = (uint32)task;//pc
    *(--p_Stack) = (uint32)0xFFFFFFFD;//lr
    *(--p_Stack) = (uint32)0xc;//R12
    *(--p_Stack) = (uint32)0x3;//R3
    *(--p_Stack) = (uint32)0x2;//R2
    *(--p_Stack) = (uint32)0x1;//R1
    *(--p_Stack) = (uint32)0x0;//R0
与硬件平台有关，该代码是CM3内核的任务栈结构，注意xpsr的T位要置1，否则PendSV异常返回时会触发用法fault。

### 内核态和线程态的切换
中断和异常服务程序中只能使用MSP（主堆栈），
CONTROL[0]
仅当在特权级下操作时才允许写该位。一旦进入了用户级，唯一返回特权级的途径，就
是触发一个（软）中断，再由服务例程改写该位。
CONTROL 寄存器也是通过 MRS 和 MSR 指令来操作的：
MRS R0, CONTROL
MSR CONTROL, R0

## 3、任务间通信（事件）  
### 事件控制块  
事件类型：信号量，消息邮箱
信号量计数器
消息指针
任务等待表  
```c
typedef struct
{
    uint8  OSEventtype; //事件类型
    uint16 OSEventCnt;  //信号量计数
    void * OSEventPtr;  //消息指针
    uint32 OSEventTbl;  //任务等待表
}
OS_ECB;
```  
事件类型有  
OS_EVENT_TYPE_SEM   信号量  
OS_EVENT_TYPE_MUTEX 互斥信号量  
OS_EVENT_TYPE_MBOX  消息邮箱  

### 空事件控制块
系统初始化时先创建固定个数的空事件控制块。
用OSEventPtr链接起来。  
创建事件时从空链表内取出来。

### 信号量
OS_ECB中的OSEventtype为OS_EVENT_TYPE_SEM，且采用OSEventCnt信号量计数，消息指针不使用为空。
信号量的操作  
- 创建信号量  
- 请求信号量  
- 发送信号量
- 查询信号量状态
- 删除信号量


### 互斥信号量

### 消息邮箱




# 遇到的困难 
本想全部采用MSP  
开一个系统栈  
基于中断的任务切换时，发现平台没有提供总的中断入口，而是由硬件直接跳转到ISR。 
任务切换不能在中断还未完全退出时进行   
无法统计中断嵌套层数，就不知道什么时候切换任务。  

解决办法，任务切换方式改为cm3内核提供的PendSV异常，把PendSV异常的优先级设置为最低。这样就会在最后一个中断ISR完成后进入PenSV的Handler函数，在这个异常处理函数中进行任务的切换。

在异常返回时，xPSR中的EPSR的标志位需要注意，没有设置正确的话会触发，用法Fault。
EPSR的T位使用一条可相互作用的指令来清零这里写入的PC的位0为0。也可以使用异常出栈操作来清零，被压栈的T位为0。
当T位为零时执行异常返回指令会引起INVSTATE异常。
说以初始化的时候，T位要置1.

在异常中使用R4-R11，需要先push 返回前再pop




