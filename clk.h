#ifndef CLK_H
#define CLK_H

/*  #define _XOPEN_SOURCE 600 */
#include "utils.h"
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <semaphore.h>


#define CLK_CYCLE_MS (1)
#define CLK_NS (1000000000)
#define CLK_ID (CLOCK_MONOTONIC)

int     CLK_InitTimer(struct sigevent *sigev);
void    CLK_DisableTimer(void);
void    CLK_ShowTimeMs(void);





#endif /* CLK_H */ 