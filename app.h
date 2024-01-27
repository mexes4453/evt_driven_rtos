#ifndef APP_H
#define APP_H

#include "utils.h"
#include "os.h"
#include "queue.h"
#include "ao.h"

#define APP_THREAD_MAX (OS_THREADNUM)


#define APP_RED     "RED"
#define APP_GREEN   "GREEN"
#define APP_BLUE    "BLUE"
#define APP_YELLOW  "YELLOW"
#define APP_MAGENTA "MAGENTA"
#define COLORLED_ALARMTICKCOUNT (20)
#define TRANS(_target) (me->state=_target, AO_STATUS_TRANSIT)

enum enAppSignals
{
    SIG_TIMER = SIG_USER,
    SIG_MAX
};

typedef struct s_colorLed t_colorLed;

typedef t_enStatusActiveObj (*f_StateHandler)(t_colorLed * const me, t_Event const * const evt);

struct s_colorLed
{
    t_ActiveObj    super;  /* inheritance */
    t_EventTime    timer;
    int            blinkCounter;
    f_StateHandler state;
};

void                APP_LedOn(char *colorCodeTxt, char * colorTxt);
void                APP_LedOff(char *colorCodeTxt, char * colorTxt);
void                colorLed_Ctor(t_colorLed * const me, int cpuIdx, int prior);
void                colorLed_dispatch(t_colorLed * const me, t_Event const * const evt);
t_enStatusActiveObj colorLed__Initial(t_colorLed * const me, t_Event const * const evt);
t_enStatusActiveObj colorLed__WaitForTrigger(t_colorLed * const me, t_Event const * const evt);
t_enStatusActiveObj colorLed__AlarmLow(t_colorLed * const me, t_Event const * const evt);
t_enStatusActiveObj colorLed__AlarmMedium(t_colorLed * const me, t_Event const * const evt);
t_enStatusActiveObj colorLed__AlarmHigh(t_colorLed * const me, t_Event const * const evt);
t_enStatusActiveObj colorLed__AlarmExplode(t_colorLed * const me, t_Event const * const evt);
t_enStatusActiveObj colorLed__Pause(t_colorLed * const me, t_Event const * const evt);


#endif // OS_H