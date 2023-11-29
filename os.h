#ifndef OS_H
#define OS_H

#include <pthread.h>
#include <time.h>
#include "utils.h"


typedef void *(*t_osThreadhandler) (void *);

int    OS_CreateThread(pthread_t *thread, const pthread_attr_t *attr,
                                           t_osThreadhandler func,
                                           void *arg);


void    OS_Sequencer(void);
void    OS_ShowThreadInfo(void);
#endif /* OS_H*/