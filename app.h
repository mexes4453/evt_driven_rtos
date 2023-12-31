#ifndef APP_H
#define APP_H

#include "utils.h"
#include "os.h"
#include "queue.h"
#include "ao.h"

#define APP_THREAD_MAX (OS_THREADNUM)

typedef enum    e_appTaskId
{
    APP_LED_BLUE = 0,
    APP_LED_YELLOW,
    APP_MAX
}   t_appTaskId;


void    *APP_TaskBlue(void *ptr);
void    *APP_TaskYellow(void *ptr);



enum enAppSignals
{
    SIG_TIMER = SIG_USER,
};

typedef struct colorLed
{
    t_ActiveObj     super;  /* inheritance */
    /* additional members */
    enum enState {
        STATE_WHITE,        /* 0  <= x < 10 ticks */
        STATE_BLUE,         /* 10 <= x > 20 ticks */
        STATE_MAGENTA,      /* 20 <= x > 35 ticks */
        STATE_YELLOW,       /* 35 <= x > 55 ticks */
        STATE_RED,          /* 55 <= x > 60 ticks */
    } state;
    t_EventTime    timer;
    int            blinkCounter;

} t_colorLed;
void colorLed_dispatch(t_colorLed * const me, t_Event const * const evt);
void colorLed_Ctor(t_colorLed * const me, int cpuIdx, int prior);
#endif // OS_H