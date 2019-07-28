#ifndef  __OS_EVENT_H__
#define  __OS_EVENT_H__

#include "typedef.h"

#define OS_ERR_NONE       0u
#define OS_ERR_EVENT_TYPE 1u
#define OS_ERR_EVENT_TIMEOUT      2u

#define OS_ERR_SEM_OVF      3u

#define  OS_STAT_PEND_OK              0u
#define  OS_STAT_PEND_TO              1u

typedef enum {
    SEM = 0,
    MUTEX,
    MBOX,
    UNUSED
}EVENT_TYPE;

typedef struct
{
    uint8  OSEventType; //事件类型
    uint16 OSEventCnt;  //信号量计数
    void * OSEventPtr;  //消息指针
    uint32 OSEventTbl;  //任务等待表
}
OS_ECB;

void OSInitEvent(void);

void OS_EventWaiteListInit(OS_ECB *pevent);

void OS_EventTaskWait(OS_ECB *pevent);

void OS_EventTaskRdy(OS_ECB *pevent);

void OS_EventTO(OS_ECB *pevent);

/******************************
 * 信号量 
 * ****************************/
/*创建信号量 */
OS_ECB *OSSemCreate(int16 cnt);

/*请求信号量 */
void OSSemPend(OS_ECB *pevent, uint16 timeout, uint8 *err);

/*发送信号量 */
uint8 OSSemPost(OS_ECB *pevent);

/*删除信号量 */
OS_ECB *OSSemDel(OS_ECB *pevent);

/******************************
 * 互斥信号量
 * ****************************/

/******************************
 * 消息邮箱
 * ****************************/

#endif