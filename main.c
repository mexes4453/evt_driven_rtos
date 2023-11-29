#include "os.h"
#include "clk.h"

/* FUNCTION */



void    *hello(void *)
{
    while (1)
    {
        printf("Hello my friend\n");
        /* CLK_ShowTimeMs(); */
    }
    return (NULL);
}

void    callFunc(t_osThreadhandler func)
{
    void    *nullPtr = func(NULL);
    printf("calling from main thread\n");
    if (nullPtr){};
}

int main (void)
{
    struct sigaction    sa;
    struct sigevent     sigevt;
    cpu_set_t           cpuSet;
#ifdef __thread__
    pthread_t           greetThread;
#endif 

    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction =  CLK_SigHandler;
    sa.sa_flags = SA_SIGINFO;

    sigevt.sigev_notify = SIGEV_SIGNAL;
    sigevt.sigev_signo = SIGALRM;

    sigaction(SIGALRM, &sa, NULL);
    CLK_TimerInit(&sigevt);

#ifdef __thread__
    /* callFunc(hello); */
    CPU_ZERO(&cpuSet);
    CPU_ZERO(1, &cpuSet);
    pthread_setaffinity_np(greetThread, sizeof(cpuSet), &cpuSet);
    if (OS_CreateThread(&greetThread, NULL, hello, NULL) < 0)
        PERROR("create thread", -1);

    /*  wait for all threads to join back the main before exiting. */
    /*  The threads are not expected to rejoin as they should run forever. */
    pthread_join(greetThread, NULL);
#endif 

    while (1)
    {
        pause();
    }
    return (0);
}