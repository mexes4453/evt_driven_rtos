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

int OS_CreateAllThreads(t_osThreadTable   *threadTable, int tableSize)
{
    int res;
    int idx = 0;

    while (idx < tableSize)
    {

        res = pthread_create(threadTable[idx].thread, 
                             &(threadTable[idx].params->attr),
                             threadTable[idx].func,
                             (void *)threadTable[idx].params);
        if  (res != 0)
        {
            PERROR("create_thread", 1);
        }
        idx++;
    }
    return (res);
}


void    OS_InitThreadParams(t_osThreadParams *threadParam, int cpuIdx, int prio)
{
    cpu_set_t   cpuSet;

    CPU_ZERO(&cpuSet);
    CPU_SET((cpuIdx), &cpuSet);

    /* assertion - cpu and prio */
    assert(cpuIdx < get_nprocs());
    assert((sched_get_priority_min(SCHED_FIFO) <= prio) && 
           (prio <= sched_get_priority_max(SCHED_FIFO)));
    
    /* Initialise the thread attributes */
    assert(pthread_attr_init(&(threadParam->attr)) == 0);
    assert(pthread_attr_setinheritsched(&(threadParam->attr), PTHREAD_INHERIT_SCHED) == 0);
    assert(pthread_attr_setschedpolicy(&(threadParam->attr), SCHED_FIFO) == 0);
    assert(pthread_attr_setaffinity_np(&(threadParam->attr), sizeof(cpu_set_t), &cpuSet)  == 0);
    threadParam->schedParam.sched_priority = prio;
    pthread_attr_setschedparam(&(threadParam->attr), &(threadParam->schedParam));

    /* Initialise the thread custom-defined parameter */
    threadParam->prio = prio;
    threadParam->stateExit = false;
    threadParam->stateExec = OS_STATE_READY;
    sem_init(threadParam->semExec, 0, 0);

}


void    OS_InitAllThreadParams(t_osThreadTable   *threadTable, int tableSize)
{
    int idx = 0;

    while (idx < tableSize)
    {
        bzero(threadTable[idx].params, sizeof(t_osThreadParams));
        threadTable[idx].params->idx = idx;
        threadTable[idx].params->name = threadTable[idx].name;
        threadTable[idx].params->semExec = threadTable[idx].semExec;
        OS_InitThreadParams(threadTable[idx].params,
                            threadTable[idx].cpuIdx,
                            threadTable[idx].prio);
        idx++;
    }
}


void    OS_ShowThreadInfo(t_osThreadParams *params, char *color)
{
    uint64_t tid = (uint64_t)params->tid;

    UTILS_PrintTxt(color);
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