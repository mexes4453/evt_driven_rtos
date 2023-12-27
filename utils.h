#ifndef UTILS_H
#define UTILS_H
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <signal.h>
#include "./ft_printf/ft_printf.h"

# define COL_BLUE       "\033[0;34m"
# define COL_RED        "\033[0;31m"
# define COL_YELLOW     "\033[0;33m"
# define COL_MAGENTA    "\033[0;35m"
# define COL_DEFAULT    "\033[0m"


/* MACROS */
#define PERROR(msg,errNo)\
{\
    perror(msg); exit(errNo); \
}\



#define UTILS_PRINTF(stringFmt, ...)\
    (ft_printf((stringFmt), ##__VA_ARGS__)) \



/* FUNCTIONS */
void UTILS_PrintTxt(char *msg);
void UTILS_PrintInt(uint64_t nbr);



/*
 * This macro emulates the memcpy function in <string.h>
 * It copies the byte data from source address to destination
 * address given the number of bytes to copy.
 * @dstAddr : ptr to destination buffer
 * @srcAddr : ptr to source buffer
 * @byteCnt : Number of bytes to copy from src to dst.
 * -----------------
 * !!! ATTENTION !!!
 * -----------------
 * Only use when enough buffer is provided for dst.
 */
#define UTILS_MEMCPY(dstAddr,srcAddr,byteCnt)\
    if (((dstAddr) != NULL) && ((srcAddr) != NULL))\
    {\
        for (int idx=0; idx<(byteCnt); idx++)\
        {\
            (dstAddr)[idx] = (srcAddr)[idx];\
        }\
    }\



/*
 * This macro emulates the memcpy function in <string.h>
 * It write the given value to destination address, given the
 * number of bytes to write.
 * @addr    : ptr to destination buffer
 * @value   : data to write
 * @byteCnt : Number of bytes
 * -----------------
 * !!! ATTENTION !!!
 * -----------------
 * Only use when enough buffer is provided for dst.
 */
#define UTILS_MEMSET(addr,value,byteCnt)\
    if ((addr) != NULL)\
    {\
        for (int unsigned idx=0; idx<(byteCnt); idx++)\
        {\
            (addr)[idx] = value;\
        }\
    }\



/* Update this macro using minimalistic printf */
#define UTILS_ASSERT(condition,msg)\
    if (!(condition))\
	{\
        ft_printf((msg));\
        kill(getpid(), SIGINT);\
	}\



#endif /* UTILS_H */
