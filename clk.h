#ifndef CLK_H
#define CLK_H

#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

/*  #define _XOPEN_SOURCE 600 */

#define CLK_CYCLE_MS (1)
#define CLK_NS (1000000000)
#define CLK_ID (CLOCK_MONOTONIC)

void    CLK_Signal( void *ptrThread);
int     CLK_TimerInit(struct sigevent *sigev);
void    CLK_SigHandler(int sig, siginfo_t *siginfo, void *contextInfo);
void    CLK_ShowTimeMs(void);





#endif /* CLK_H */ 