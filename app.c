#include "app.h"

void    *APP_TaskBlue(void *ptr)
{
    while (1)
    {
        UTILS_PrintTxt("Blue thread - ");
        OS_ShowThreadInfo();
    }
    return (ptr);
}


