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
    cpu_set_t           cpuSet;
    pthread_t           blueThread;
    pthread_attr_t      blueThreadAttr;
    struct sched_param  blueThreadSchedParams;
    t_osThreadParams    blueThreadParams;

#endif 

    OS_InitSchedInterrupt(&sa);
    CLK_InitTimer(&sigevt);




#ifdef __thread__
    /* OS_CallFunc(hello);  Testing function */
    CPU_ZERO(&cpuSet);
    CPU_SET((1), &cpuSet);
    bzero(&blueThreadParams, sizeof(t_osThreadParams));
    bzero(&blueThreadSchedParams, sizeof(blueThreadSchedParams));
    bzero(&blueThreadAttr, sizeof(pthread_attr_t));

    pthread_attr_init(&blueThreadAttr);
    pthread_attr_setinheritsched(&blueThreadAttr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&blueThreadAttr, SCHED_FIFO);
    pthread_attr_setaffinity_np(&blueThreadAttr, sizeof(cpu_set_t), &cpuSet);

    blueThreadSchedParams.sched_priority = sched_get_priority_max(SCHED_FIFO) - 1;
    pthread_attr_setschedparam(&blueThreadAttr, &blueThreadSchedParams);

    blueThreadParams.idx = 0;
    OS_CreateThread(&blueThread,
    // &blueThreadAttr,
      NULL,
      &APP_TaskBlue, 
      NULL);
           //         (void *)&blueThreadParams);

#endif 

    while (1)
    {
        pause();
        if (exitSig)
        {
#ifdef __thread__
            pthread_kill(blueThread, SIGKILL);
#endif 
            CLK_DisableTimer();
            printf("Interrupt count: %d\n", execCounter);
            printf("Total: %d; available: %d\n", get_nprocs_conf(), get_nprocs());
            printf("max_prior: %d; min_prior: %d\n", sched_get_priority_max(SCHED_FIFO),
                                                   sched_get_priority_min(SCHED_FIFO));
            break ;
        }
    }

#ifdef __thread__
    /*  wait for all threads to join back the main before exiting. */
    /*  The threads are not expected to rejoin as they should run forever. */
    pthread_join(blueThread, NULL);
#endif 
    return (0);
}