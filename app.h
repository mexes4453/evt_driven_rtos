#ifndef APP_H
#define APP_H

#include "os.h"
#include "utils.h"

#define APP_THREAD_MAX (OS_THREADNUM)

typedef enum    e_appTaskId
{
    APP_LED_BLUE = 0,
    APP_LED_YELLOW,
    APP_MAX
}   t_appTaskId;


void    *APP_TaskBlue(void *ptr);
void    *APP_TaskYellow(void *ptr);

#endif // OS_H