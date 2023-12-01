#include "os.h"
//extern struct sigaction    sa;
void    OS_InitSchedInterrupt(struct sigaction *sa)
{
    
    sigemptyset(&(sa->sa_mask));
    sa->sa_sigaction =  CLK_SigHandler;
    sa->sa_flags = SA_SIGINFO;
    sigaction(SIGALRM, sa, NULL);
    sigaction(SIGINT, sa, NULL);
}

int OS_CreateThread(pthread_t *thread, const pthread_attr_t *attr,
                    t_osThreadhandler func,
                    void *arg)
{
    int res = pthread_create(thread, attr, func, arg);
    if  (res != 0)
        PERROR("create_thread", 1);

    return (res);
}


void    OS_ShowThreadInfo(t_osThreadParams *params)
{
    uint64_t tid = (uint64_t)params->tid;

    UTILS_PrintTxt(COL_BLUE);
    UTILS_PrintTxt("-- ");
    UTILS_PrintTxt(params->name);
    UTILS_PrintTxt(" -- ID(");
    UTILS_PrintInt(tid);
    UTILS_PrintTxt(") - CPU(");
    UTILS_PrintInt((uint64_t)sched_getcpu());
    UTILS_PrintTxt(") - PRIO(");
    UTILS_PrintInt((uint64_t)params->prio);
    UTILS_PrintTxt(")\n");
    UTILS_PrintTxt(COL_DEFAULT);
}

void    OS_CallFunc(t_osThreadhandler func)
{
    void    *nullPtr = func(NULL);
    UTILS_PrintTxt("calling from main thread\n");
    if (nullPtr){};
}