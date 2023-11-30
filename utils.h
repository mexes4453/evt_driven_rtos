#ifndef UTILS_H
#define UTILS_H
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>




/* MACROS */
#define PERROR(msg,errNo)\
{\
    perror(msg); exit(errNo); \
}\

void UTILS_PrintTxt(char *msg);
void UTILS_PrintInt(uint64_t nbr);


#endif /* UTILS_H */