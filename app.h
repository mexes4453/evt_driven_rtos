#ifndef APP_H
#define APP_H

#include "utils.h"
#include "os.h"
#include "queue.h"
#include "ao.h"
#include "fsm.h"

#define APP_THREAD_MAX (OS_THREADNUM)


#define APP_RED     "RED"
#define APP_GREEN   "GREEN"
#define APP_BLUE    "BLUE"
#define APP_YELLOW  "YELLOW"
#define APP_MAGENTA "MAGENTA"
#define COLORLED_ALARMTICKCOUNT (20)

enum enAppSignals
{
    SIG_TIMER = SIG_USER,
    SIG_MAX
};

typedef struct s_colorLed t_colorLed;


struct s_colorLed
{
    t_ActiveObj    super;  /* inheritance */
    t_eventTime    timer;
    int            blinkCounter;
};

void                APP_LedOn(char *colorCodeTxt, char * colorTxt);
void                APP_LedOff(char *colorCodeTxt, char * colorTxt);
void                colorLed_Ctor(t_colorLed * const me, int cpuIdx, int prior);
t_enStatusFsm colorLed__Initial(t_colorLed * const me, t_event const * const evt);
t_enStatusFsm colorLed__WaitForTrigger(t_colorLed * const me, t_event const * const evt);
t_enStatusFsm colorLed__AlarmLow(t_colorLed * const me, t_event const * const evt);
t_enStatusFsm colorLed__AlarmMedium(t_colorLed * const me, t_event const * const evt);
t_enStatusFsm colorLed__AlarmHigh(t_colorLed * const me, t_event const * const evt);
t_enStatusFsm colorLed__AlarmExplode(t_colorLed * const me, t_event const * const evt);
t_enStatusFsm colorLed__Pause(t_colorLed * const me, t_event const * const evt);


#endif // OS_H