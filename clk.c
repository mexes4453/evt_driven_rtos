#include <string.h>
#include "clk.h"


void CLK_Signal( void *ptrThread)
{
    if (ptrThread)
        write(1, "nothing\n", 8 );
}


int CLK_TimerInit(struct sigevent *sigev)
{
    timer_t             timerId;
    struct itimerspec   ts;
    int res;

    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = (CLK_NS / CLK_CYCLE_MS) - 1;
    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = (CLK_NS / CLK_CYCLE_MS) - 1;

    /* create a timer */
    res = timer_create(CLK_ID, sigev, &timerId);
    if (res < 0) PERROR("timer_create", -2);
    
    /* arm the timer */
    res = timer_settime(timerId, 0, &ts, NULL);
    if (res < 0) PERROR("timer_settime", -2);

    return (res);
}


void CLK_SigHandler(int sig, siginfo_t *siginfo, void *contextInfo)
{
    /* call the sequencer within here */
    if (sig == SIGALRM) 
    {
        printf("handling the sig info interrupt \n");
        CLK_ShowTimeMs();
    }

    if (siginfo && contextInfo)
    {
        printf("Sign info: %d\n", siginfo->si_signo);
    }
    
}


void    CLK_ShowTimeMs(void)
{
    struct timespec ts;
    /* bzero(&ts, sizeof(struct timespec)); */

    int res = clock_gettime(CLOCK_MONOTONIC,  &ts);
    if (res < 0) PERROR("clock_gettime\n", -2);

    printf("Time: %lds\n", ts.tv_sec);
}