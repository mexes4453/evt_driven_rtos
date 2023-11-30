#include "os.h"
#include "clk.h"
#include "app.h"

int exitSig = 0;
int execCounter = 0;
int main (void)
{
    static struct sigaction     sa;
    struct sigevent             sigevt;
    
    

#ifdef __thread__
    //cpu_set_t           cpuSet;
    pthread_t           threadBlue;
    t_osThreadParams    threadParamsBlue;
#endif 

    OS_InitSchedInterrupt(&sa);
    CLK_InitTimer(&sigevt);
    bzero(&threadParamsBlue, sizeof(t_osThreadParams));


#ifdef __thread__
    /* OS_CallFunc(hello); */
    //CPU_ZERO(&cpuSet);
    //CPU_SET(1, &cpuSet);
    threadParamsBlue.tid = OS_CreateThread(&threadBlue, NULL, &APP_TaskBlue, NULL);
    if  (threadParamsBlue.tid > 0)
        PERROR("create thread", -1);
    threadParamsBlue.tid = threadBlue;

    //sched_setaffinity(threadBlue, sizeof(cpuSet), &cpuSet);
#endif 

    while (1)
    {
        pause();
        if (exitSig)
        {
#ifdef __thread__
            pthread_kill(threadBlue, SIGKILL);
#endif 
            CLK_DisableTimer();
            printf("Interrupt count: %d\n", execCounter);
            printf("Total: %d; available: %d\n", get_nprocs_conf(), get_nprocs());
            break ;
        }
    }

#ifdef __thread__
    /*  wait for all threads to join back the main before exiting. */
    /*  The threads are not expected to rejoin as they should run forever. */
    pthread_join(threadBlue, NULL);
#endif 
    return (0);
}