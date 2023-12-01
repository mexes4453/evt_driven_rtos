#include "app.h"

void    *APP_TaskBlue(void *ptr)
{
    t_osThreadParams *threadParams = (t_osThreadParams *)ptr;
    threadParams->tid = pthread_self();
    while (!threadParams->stateExit)
    {
        sem_wait(threadParams->sema);
        OS_ShowThreadInfo(threadParams);
        CLK_ShowTimeMs();
        /* event pump or do more work here */
    }
    return (ptr);
}


