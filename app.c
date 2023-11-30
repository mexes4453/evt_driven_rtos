#include "app.h"

void    *APP_TaskBlue(void *ptr)
{
    t_osThreadParams *threadParams = (t_osThreadParams *)ptr;
    threadParams->tid = pthread_self();
    while (1)
    {
        UTILS_PrintTxt("Blue thread - ");
        OS_ShowThreadInfo();
    }
    return (ptr);
}


