#include "app.h"

void    *APP_TaskBlue(void *ptr)
{
    t_osThreadParams *threadParams = (t_osThreadParams *)ptr;
    threadParams->tid = pthread_self();
    while (1)
    {
        sem_wait(threadParams->sema);
        UTILS_PrintTxt("Blue thread - ");
        OS_ShowThreadInfo(threadParams);
        CLK_ShowTimeMs();
        UTILS_PrintTxt("CPU (");
        UTILS_PrintInt((uint64_t)sched_getcpu());
        UTILS_PrintTxt(")\n");

    }
    return (ptr);
}


