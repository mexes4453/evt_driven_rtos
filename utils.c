#include "utils.h"

void UTILS_PrintTxt(char *msg)
{
    if (msg)
    {
        write(1, msg, strlen(msg));
    }
}


void UTILS_PrintInt(uint64_t nbr)
{
    char chr;
    if (nbr >= 10)
    {
        UTILS_PrintInt(nbr / 10);
    }
    chr = (nbr % 10) + 0x30;
    write(1, &chr, sizeof(char));
}