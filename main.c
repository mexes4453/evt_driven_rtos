#include "os.h"
#include "clk.h"
#include "app.h"
#include "queue.h"
#include "ao.h"

int g_exitSig = 0;
int g_execCounter = 0;
//t_Queue              msgQueue;

#ifdef __thread__
//sem_t               semExec[APP_THREAD_MAX];
t_colorLed          colorLed;
t_ActiveObj         *AO_ColorLed = &(colorLed.super);

#endif

int main (void)
{
    static struct sigaction     sa;
    struct sigevent             sigevt;




    UTILS_ASSERT(AO_ColorLed, "invalid ptr to AO");


#ifdef __thread__
    //int         idx = 0;
#if 0
    pthread_t           appThreads[APP_THREAD_MAX];
    t_osThreadParams    threadParams[APP_THREAD_MAX];
    t_osThreadTable     appThreadTable[APP_THREAD_MAX] = { 
        {
            "LED_B",                     /* Thread name */
            &(appThreads[APP_LED_BLUE]),         /* Thread ptr */ 
            &(threadParams[APP_LED_BLUE]),       /* Thread parameter ptr */ 
            &(semExec[APP_LED_BLUE]),            /* Execution semaphore */ 
            3,                              /* CPU index number */
            98,                             /* Thread priority */ 
            APP_TaskBlue                    /* Thread function or handler */
        },
        {
            "LED_Y",                                /* Thread name */
            &(appThreads[APP_LED_YELLOW]),          /* Thread ptr */ 
            &(threadParams[APP_LED_YELLOW]),        /* Thread parameter ptr */ 
            &(semExec[APP_LED_YELLOW]),             /* Execution semaphore */ 
            3,                                      /* CPU index number */
            97,                                     /* Thread priority */ 
            APP_TaskYellow                          /* Thread function or handler */
        }
    };
    int threadTableSize = sizeof(appThreadTable) / sizeof(t_osThreadTable);
#endif
    colorLed_Ctor(&colorLed, 3, 98);
    //OS_InitAllThreadParams(appThreadTable, threadTableSize);
#endif


    OS_InitSchedInterrupt(&sa);
    CLK_InitTimer(&sigevt);


#ifdef __thread__
    /* OS_CallFunc(hello);  Testing function */

    //OS_CreateAllThreads(appThreadTable, threadTableSize);

    /* Enable sequencer */
#endif 

    while (1)
    {
        pause();
        //QUEUE__Put(&msgQueue, (void *)&g_execCounter);
        //ft_printf("Queue level: %d\n", QUEUE__GetLevel(&msgQueue));
        if (g_exitSig)
        {   


#ifdef __thread__
#if 0
            for (idx = 0; idx < threadTableSize; idx++)
            {
                threadParams[idx].stateExit = true;
                sem_post(&semExec[idx]);
                printf("blue thread id: %ld\n", threadParams[idx].tid);
            }
#endif 
#endif 
            UTILS_PRINTF(COL_DEFAULT);
            UTILS_PRINTF("Interrupt count: %d\n", g_execCounter);
            UTILS_PRINTF("Total: %d; available: %d\n", get_nprocs_conf(), get_nprocs());
            UTILS_PRINTF("max_prior: %d; min_prior: %d\n", sched_get_priority_max(SCHED_FIFO),
                                                     sched_get_priority_min(SCHED_FIFO));
            break ;
        }
    }


#ifdef __thread__
    // pthread_kill(blueThread, SIGTERM);

    /*  wait for all threads to join back the main before exiting. */
    /*  The threads are not expected to rejoin as they should run forever. */
    //pthread_join(blueThread, NULL);
    pthread_join(colorLed.super.thread, NULL);
    pthread_attr_destroy(&(colorLed.super.threadParams.attr));
#if 0
    for (idx = 0; idx < threadTableSize; idx++)
    {
        pthread_join(appThreads[idx], NULL);
        sem_destroy(&semExec[idx]);
        pthread_attr_destroy(&(threadParams[idx].attr));
    }
#endif 
#endif 


    CLK_DisableTimer();
    return (0);
}