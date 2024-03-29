#ifndef OS_H
#define OS_H

#define _GNU_SOURCE
#define _XOPEN_SOURCE 600
#define OS_THREADNUM (2)

#if 1
#define __thread__ 
#endif

#include "utils.h"
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <sys/sysinfo.h>
#include <semaphore.h>
#include <stdbool.h>
#include "clk.h"

/* ENUMERATION */
typedef enum e_StateOs
{
    OS_STATE_READY = 0,
    OS_STATE_RUNNING,
    OS_STATE_BLOCKED,
    OS_STATE_SLEEP,
} t_enStateOs;

/* TYPE DEFINITION */
typedef void *(*t_osThreadhandler) (void *);

typedef struct s_osThreadParams
{
    int                 idx;
    pthread_t           tid;
    sem_t               *semExec;
    char                *name;
    int                 prio;
    int                 delayCnt;
    bool                stateExit;
    t_enStateOs         stateExec;
    pthread_attr_t      attr;
    struct sched_param  schedParam;
}   t_osThreadParams;

#include "ao.h"

typedef struct s_osThreadTable
{
    char                *name;
    pthread_t           *thread;
    t_osThreadParams    *params;
    sem_t               *semExec;
    int                 cpuIdx;
    int                 prio;
    t_osThreadhandler   func;
}   t_osThreadTable;

extern sigset_t sigSetDefault;

int     OS_CreateAllThreads(t_osThreadTable   *threadTable, int tableSize);
void    OS_InitSchedInterrupt(struct sigaction *sa);
void    OS_InitThreadParams(t_osThreadParams *threadParam, int cpuIdx, int prio);
void    OS_InitAllThreadParams(t_osThreadTable   *threadTable, int tableSize);
void    OS_Sequencer(void);
void    OS_ShowThreadInfo(t_osThreadParams *params, char *color);
void    OS_CallFunc(t_osThreadhandler func);
void    OS_BlockSignals(void);
void    OS_ClkSigHandler(int sig, siginfo_t *siginfo, void *contextInfo);

#define OS_SIGNALS_UNBLOCK() sigprocmask(SIG_SETMASK, &sigSetDefault, NULL);
#define OS_SIGNALS_BLOCK() OS_BlockSignals();

#endif /* OS_H*/