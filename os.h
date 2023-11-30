#ifndef OS_H
#define OS_H

#define _GNU_SOURCE
#define OS_THREADNUM (10 + 1)
#include "utils.h"
#include "clk.h"
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <sys/sysinfo.h>


/* TYPE DEFINITION */
typedef void *(*t_osThreadhandler) (void *);
typedef struct
{
    int         idx;
    pthread_t   tid;

}   t_osThreadParams;

int    OS_CreateThread(pthread_t *thread, const pthread_attr_t *attr,
                       t_osThreadhandler func,
                       void *arg);

void    OS_InitSchedInterrupt(struct sigaction *sa);
void    OS_Sequencer(void);
void    OS_ShowThreadInfo(void);
void    OS_CallFunc(t_osThreadhandler func);
#endif /* OS_H*/