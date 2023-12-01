#ifndef UTILS_H
#define UTILS_H
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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


/* FUNCTIONS */
void UTILS_PrintTxt(char *msg);
void UTILS_PrintInt(uint64_t nbr);


#endif /* UTILS_H */