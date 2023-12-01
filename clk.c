
#include "clk.h"

static timer_t      timerId;
extern int          exitSig;
extern int          execCounter;
extern sem_t        blueSem;

int CLK_InitTimer(struct sigevent *sigev)
{
    struct itimerspec   ts;
    int res;

    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = (CLK_NS / CLK_CYCLE_MS) - 1;
    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = (CLK_NS / CLK_CYCLE_MS) - 1;

    bzero(sigev, sizeof(struct sigevent));
    sigev->sigev_notify = SIGEV_SIGNAL;
    sigev->sigev_signo = SIGALRM;

    /* create a timer */
    res = timer_create(CLK_ID, sigev, &timerId);
    if (res < 0) PERROR("timer_create", -2);

    UTILS_PrintTxt("timerId: ");
    UTILS_PrintInt((uint64_t)timerId);
    UTILS_PrintTxt("\n");
    
    /* arm the timer */
    res = timer_settime(timerId, 0, &ts, NULL);
    if (res < 0) PERROR("timer_settime", -2);

    return (res);
}


void    CLK_DisableTimer(void)
{
    struct itimerspec   ts;
    int res;
    uint64_t l_timerId = (uint64_t)timerId;

    /* setting the timer value to 0 to stop timer */
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = 0;
    res = timer_settime(timerId, 0, &ts, NULL);
    if (res < 0) PERROR("timer_settime", -2);

    UTILS_PrintTxt("timerId: ");
    UTILS_PrintInt(l_timerId);
    UTILS_PrintTxt("\n");
    res = timer_delete(timerId);
    if (res < 0) PERROR("timer_delete", -127);
    UTILS_PrintTxt("disabled Timer\n");
}



void CLK_SigHandler(int sig, siginfo_t *siginfo, void *contextInfo)
{
    
    switch (sig)
    {
        case SIGALRM: /* call the sequencer within here */
        {
            //UTILS_PrintTxt("handling the sig info interrupt \n");
            //CLK_ShowTimeMs();
            execCounter++;
            sem_post(&blueSem);
            break ;
        }
        case SIGINT:
        {
            ++exitSig;
            break ;
        }
        default:
        {
        }
    }
    if (siginfo && contextInfo)
    {
        //UTILS_PrintTxt("Sign info: ");
        //UTILS_PrintInt((uint64_t)siginfo->si_signo);
        //UTILS_PrintTxt("\n");
    }
}


void    CLK_ShowTimeMs(void)
{
    struct timespec ts;
    bzero(&ts, sizeof(struct timespec));

    int res = clock_gettime(CLK_ID,  &ts);
    if (res < 0) PERROR("clock_gettime\n", -2);

    UTILS_PrintTxt("Time: ");
    UTILS_PrintInt((uint64_t)ts.tv_sec);
    UTILS_PrintTxt("s \n");
}