#include "app.h"
#include "queue.h"
extern t_Queue msgQueue;
void    *APP_TaskBlue(void *ptr)
{
    t_osThreadParams *threadParams = (t_osThreadParams *)ptr;
    threadParams->tid = pthread_self();

    while (!threadParams->stateExit)
    {
        sem_wait(threadParams->semExec);
        //CLK_ShowTimeMs();
        OS_ShowThreadInfo(threadParams, COL_BLUE);
        //QUEUE__Put(&msgQueue, (void *)&(threadParams->tid));
        ft_printf("Queue level: %d\n", QUEUE__GetLevel(&msgQueue));
        /* event pump or do more work here */
    }
    return (NULL);
}





void    *APP_TaskYellow(void *ptr)
{
    t_osThreadParams *threadParams = (t_osThreadParams *)ptr;
    threadParams->tid = pthread_self();
    int    *pData = NULL;
    t_enStatusQueue result;

    while (!threadParams->stateExit)
    {
        sem_wait(threadParams->semExec);
        //CLK_ShowTimeMs();
        OS_ShowThreadInfo(threadParams, COL_YELLOW);
        result = QUEUE__Get(&msgQueue, (void **)&pData);
        ft_printf("read state: %d\n", result);
        UTILS_ASSERT((result != QUEUE_STATUS_FAIL), "queue read failed\n");
        if (result == QUEUE_STATUS_SUCCESS)
        {
            ft_printf("received data: %d\n", *pData);
            ft_printf("Queue level: %d\n", QUEUE__GetLevel(&msgQueue));
        }
        /* event pump or do more work here */
    }
    return (NULL);
}