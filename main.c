#include "os.h"
#include "clk.h"
#include "app.h"

int exitSig = 0;
int execCounter = 0;
sem_t      blueSem;

int main (void)
{
    static struct sigaction     sa;
    struct sigevent             sigevt;
    sem_init(&blueSem, 0, 0);

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

    bzero(&blueThreadParams, sizeof(t_osThreadParams));
    bzero(&blueThreadSchedParams, sizeof(blueThreadSchedParams));
    bzero(&blueThread, sizeof(pthread_t));
    bzero(&blueThreadAttr, sizeof(pthread_attr_t));

    CPU_ZERO(&cpuSet);
    CPU_SET((2), &cpuSet);
    if (pthread_attr_init(&blueThreadAttr) != 0) PERROR("attr_init", -127);
    if (pthread_attr_setinheritsched(&blueThreadAttr, PTHREAD_INHERIT_SCHED) != 0) PERROR("attr_init", -127);
    if (pthread_attr_setschedpolicy(&blueThreadAttr, SCHED_FIFO) != 0) PERROR("attr_init", -127); ;
    if (pthread_attr_setaffinity_np(&blueThreadAttr, sizeof(cpu_set_t), &cpuSet)  != 0) PERROR("attr_init", -127); ;;

    blueThreadSchedParams.sched_priority = sched_get_priority_max(SCHED_FIFO) - 1;
    pthread_attr_setschedparam(&blueThreadAttr, &blueThreadSchedParams);

    blueThreadParams.idx = 0;   
    blueThreadParams.sema = &blueSem;   
    OS_CreateThread(&blueThread,
     &blueThreadAttr,
      //NULL,
      &APP_TaskBlue, 
      (void *)&blueThreadParams);


#endif 

    while (1)
    {
        pause();
        if (exitSig)
        {
#ifdef __thread__
            pthread_kill(blueThread, SIGKILL);
    /*  wait for all threads to join back the main before exiting. */
    /*  The threads are not expected to rejoin as they should run forever. */
    //pthread_join(blueThread, NULL);
            pthread_join(blueThread, NULL);
#endif 
            CLK_DisableTimer();
            sem_destroy(&blueSem);
            pthread_attr_destroy(&blueThreadAttr);
            printf("Interrupt count: %d\n", execCounter);
            printf("Total: %d; available: %d\n", get_nprocs_conf(), get_nprocs());
            printf("max_prior: %d; min_prior: %d\n", sched_get_priority_max(SCHED_FIFO),
                                                     sched_get_priority_min(SCHED_FIFO));
            printf("blue thread id: %ld\n", blueThreadParams.tid);
            break ;
        }
    }
    return (0);
}