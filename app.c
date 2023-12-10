#include "app.h"

void    *APP_TaskBlue(void *ptr)
{
    t_osThreadParams *threadParams = (t_osThreadParams *)ptr;
    threadParams->tid = pthread_self();

    while (!threadParams->stateExit)
    {
        sem_wait(threadParams->semExec);
        OS_ShowThreadInfo(threadParams, COL_BLUE);
        /* event pump or do more work here */
    }
    return (ptr);
}





void    *APP_TaskYellow(void *ptr)
{
    t_osThreadParams *threadParams = (t_osThreadParams *)ptr;
    threadParams->tid = pthread_self();

    while (!threadParams->stateExit)
    {
        sem_wait(threadParams->semExec);
        OS_ShowThreadInfo(threadParams, COL_YELLOW);
        /* event pump or do more work here */
    }
    return (ptr);
}