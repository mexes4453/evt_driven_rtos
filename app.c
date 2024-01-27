#include "app.h"


void APP_LedOn(char *colorCodeTxt, char * colorTxt)
{
    UTILS_ASSERT((colorCodeTxt != NULL) && (colorTxt != NULL), "string is null");
    UTILS_PRINTF("%s%s_LED - ON\n", colorCodeTxt, colorTxt);
}



void APP_LedOff(char *colorCodeTxt, char * colorTxt)
{
    UTILS_ASSERT((colorCodeTxt != NULL) && (colorTxt != NULL), "string is null");
    UTILS_PRINTF("%s%s_LED - OFF\n", colorCodeTxt, colorTxt);
}



void colorLed_Ctor(t_colorLed * const me, int cpuIdx, int prior)
{
    AO__Ctor(&(me->super), (f_StateHandler)colorLed__Initial, cpuIdx, prior);
    AO_EventTime__Ctor(&(me->timer), SIG_TIMER, &(me->super));
}



t_enStatusFsm colorLed__Initial(t_colorLed * const me, t_event const * const evt)
{
    UTILS_ASSERT((evt != (t_event *)0), "Error: Null pointer to event")
    //AO_EventTime__Enable(&(me->timer), 1, 0);
    t_enStatusFsm status = (TRANS(colorLed__WaitForTrigger));
    return (status);
}



t_enStatusFsm colorLed__WaitForTrigger(t_colorLed * const me, t_event const * const evt)
{
    t_enStatusFsm status;
    
    switch (evt->sig)
    {
        case SIG_ENTRY:
        {
            AO_EventTime__Enable(&(me->timer), 1, 0);
            APP_LedOn(COL_GREEN, APP_GREEN);
            status = FSM_STATUS_HANDLED;
            break ;
        }
        case SIG_EXIT:
        {
            APP_LedOff(COL_DEFAULT, APP_GREEN);
            status = FSM_STATUS_HANDLED;
            break ;
        }
        case SIG_TIMER:
        {
            UTILS_PRINTF("%s_LED: ON.  Press Ctrl + z to trigger alarm\n", APP_GREEN);
            AO_EventTime__Enable(&(me->timer), 1, 0);
            status = FSM_STATUS_HANDLED;
            //status = TRANS(colorLed__WaitForTrigger);
            break ;
        }
        case SIG_TRIGGER_CTRL_Z:
        {
            me->blinkCounter = COLORLED_ALARMTICKCOUNT;
            //TRANS(ALARM_LOW)
            status = TRANS(colorLed__AlarmLow);
            //status = FSM_STATUS_HANDLED;
            break ;
        }
        default:
        {
            status = FSM_STATUS_IGNORED;
        }
    }
    return status;
}



t_enStatusFsm colorLed__AlarmLow(t_colorLed * const me, t_event const * const evt)
{
    t_enStatusFsm status;
    
    switch (evt->sig)
    {
        case SIG_ENTRY:
        {
            APP_LedOn(COL_BLUE, APP_BLUE);
            AO_EventTime__Enable(&(me->timer), 3, 0);
            status = FSM_STATUS_HANDLED;
            break ;
        }
        case SIG_EXIT:
        {
            APP_LedOff(COL_DEFAULT, APP_BLUE);
            status = FSM_STATUS_HANDLED;
            break ;
        }
        case SIG_TIMER:
        {
            UTILS_PRINTF("%s_LED: ON.  Low alarm!\n", APP_BLUE);
            status = TRANS(colorLed__Pause);
            break ;
        }
        default:
        {
            status = FSM_STATUS_IGNORED;
        }
    }
    return status;
}




t_enStatusFsm colorLed__AlarmMedium(t_colorLed * const me, t_event const * const evt)
{
    t_enStatusFsm status;
    
    switch (evt->sig)
    {
        case SIG_ENTRY:
        {
            APP_LedOn(COL_MAGENTA, APP_MAGENTA);
            AO_EventTime__Enable(&(me->timer), 2, 0);
            status = FSM_STATUS_HANDLED;
            break ;
        }
        case SIG_EXIT:
        {
            APP_LedOff(COL_DEFAULT, APP_MAGENTA);
            status = FSM_STATUS_HANDLED;
            break ;
        }
        case SIG_TIMER:
        {
            UTILS_PRINTF("%s_LED: ON.  Medium alarm!\n", APP_MAGENTA);
            status = TRANS(colorLed__Pause);
            break ;
        }
        default:
        {
            status = FSM_STATUS_IGNORED;
        }
    }
    return status;

}




t_enStatusFsm colorLed__AlarmHigh(t_colorLed * const me, t_event const * const evt)
{
    t_enStatusFsm status;
    
    switch (evt->sig)
    {
        case SIG_ENTRY:
        {
            APP_LedOn(COL_YELLOW, APP_YELLOW);
            AO_EventTime__Enable(&(me->timer), 1, 0);
            status = FSM_STATUS_HANDLED;
            break ;
        }
        case SIG_EXIT:
        {
            APP_LedOff(COL_DEFAULT, APP_YELLOW);
            status = FSM_STATUS_HANDLED;
            break ;
        }
        case SIG_TIMER:
        {
            UTILS_PRINTF("%s_LED: ON.  High alarm!\n", APP_YELLOW);
            status = TRANS(colorLed__Pause);
            break ;
        }
        default:
        {
            status = FSM_STATUS_IGNORED;
        }
    }
    return status;
}



t_enStatusFsm colorLed__AlarmExplode(t_colorLed * const me, t_event const * const evt)
{
    t_enStatusFsm status;
    
    switch (evt->sig)
    {
        case SIG_ENTRY:
        {
            APP_LedOn(COL_RED, APP_RED);
            AO_EventTime__Enable(&(me->timer), 1, 0);
            status = FSM_STATUS_HANDLED;
            break ;
        }
        case SIG_EXIT:
        {
            APP_LedOff(COL_DEFAULT, APP_RED);
            status = FSM_STATUS_HANDLED;
            break ;
        }
        case SIG_TIMER:
        {
            UTILS_PRINTF("%s_LED: ON.  BOOM!!!\n", APP_RED);
            AO_EventTime__Disable(&(me->timer));
            status = TRANS(colorLed__Initial);
            break ;
        }
        default:
        {
            status = FSM_STATUS_IGNORED;
        }
    }
    return status;
}



t_enStatusFsm colorLed__Pause(t_colorLed * const me, t_event const * const evt)
{
    t_enStatusFsm status;
    
    switch (evt->sig)
    {
        case SIG_ENTRY:
        {
            AO_EventTime__Enable(&(me->timer), 1, 1);
            status = FSM_STATUS_HANDLED;
            break ;
        }
        case SIG_TIMER:
        {
            --(me->blinkCounter);
            if ((me->blinkCounter) > COLORLED_ALARMTICKCOUNT - 5)
            {
                status = TRANS(colorLed__AlarmLow);
                
            }
            else if ((me->blinkCounter) > COLORLED_ALARMTICKCOUNT - 10)
            {
                status = TRANS(colorLed__AlarmMedium);
            }
            else if ((me->blinkCounter) > 0)
            {
                status = TRANS(colorLed__AlarmHigh);
            }
            else if ((me->blinkCounter) == 0)
            {
                status = TRANS(colorLed__AlarmExplode);
            }
            else
            {
                status = TRANS(colorLed__WaitForTrigger);
            }
            break ;
        }
        default:
        {
            status = FSM_STATUS_IGNORED;
        }
    }
    return status;
}
