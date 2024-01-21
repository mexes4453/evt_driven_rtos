#include "app.h"

//extern t_Queue msgQueue;
void    *APP_TaskBlue(void *ptr)
{
    t_osThreadParams *threadParams = (t_osThreadParams *)ptr;
    threadParams->tid = pthread_self();

    while (!threadParams->stateExit)
    {
        //sem_wait(threadParams->semExec);
        //CLK_ShowTimeMs();
        OS_ShowThreadInfo(threadParams, COL_BLUE);
        //QUEUE__Put(&msgQueue, (void *)&(threadParams->tid));
        //ft_printf("Queue level: %d\n", QUEUE__GetLevel(&msgQueue));
        /* event pump or do more work here */
    }
    return (NULL);
}





void    *APP_TaskYellow(void *ptr)
{
    t_osThreadParams *threadParams = (t_osThreadParams *)ptr;
    threadParams->tid = pthread_self();
    //int    *pData = NULL;
    //t_enStatusQueue result;

    while (!threadParams->stateExit)
    {
        //sem_wait(threadParams->semExec);
        //CLK_ShowTimeMs();
        OS_ShowThreadInfo(threadParams, COL_YELLOW);
        /*
        result = QUEUE__Get(&msgQueue, (void **)&pData);
        ft_printf("read state: %d\n", result);
        UTILS_ASSERT((result != QUEUE_STATUS_FAIL), "queue read failed\n");
        if (result == QUEUE_STATUS_SUCCESS)
        {
            ft_printf("received data: %d\n", *pData);
            ft_printf("Queue level: %d\n", QUEUE__GetLevel(&msgQueue));
        }
        */
        /* event pump or do more work here */
    }
    return (NULL);
}






void colorLed_dispatch(t_colorLed * const me, t_Event const * const evt)
{
    if (evt->sig == SIG_INIT)
    {
        me->state = STATE_WHITE;
        me->blinkCounter = 3;
        ft_printf(COL_DEFAULT);
        AO_EventTime__Enable(&(me->timer), 1, 1);
    }
    ft_printf("blinkCounter: %d\n", me->blinkCounter);
    switch (me->state)
    {
        case STATE_WHITE:
        {
            switch (evt->sig)
            {
                case SIG_TIMER:
                {
                    /* set text color */
                    if (me->blinkCounter == 0)
                    {
                        me->state = STATE_BLUE;
                        me->blinkCounter = 5;
                        ft_printf(COL_BLUE);
                    }
                    else
                    {
                        ft_printf("OFF\n");
                        --(me->blinkCounter);
                    }
                    break ;
                }
            }
            break ;
        }
        case STATE_BLUE:
        {
            switch (evt->sig)
            {
                case SIG_TIMER:
                {
                    /* set text color */
                    if (me->blinkCounter == 0)
                    {
                        me->state = STATE_MAGENTA;
                        me->blinkCounter = 5;
                        ft_printf(COL_MAGENTA);
                    }
                    else
                    {
                        ft_printf("ON\n");
                        --(me->blinkCounter);
                    }
                    break ;
                }
            }
            break ;
        }
        case STATE_MAGENTA:
        {
            switch (evt->sig)
            {
                case SIG_TIMER:
                {
                    /* set text color */
                    if (me->blinkCounter == 0)
                    {
                        me->state = STATE_YELLOW;
                        me->blinkCounter = 5;
                        ft_printf(COL_YELLOW);
                    }
                    else
                    {
                        ft_printf("ON\n");
                        --(me->blinkCounter);
                    }
                    break ;
                }
            }
            break ;
        }
        case STATE_YELLOW:
        {
            switch (evt->sig)
            {
                case SIG_TIMER:
                {
                    /* set text color */
                    if (me->blinkCounter == 0)
                    {
                        me->state = STATE_WHITE;
                        me->blinkCounter = 5;
                        ft_printf(COL_DEFAULT);
                        AO_EventTime__Disable(&(me->timer));
                    }
                    else
                    {
                        ft_printf("ON\n");
                        --(me->blinkCounter);
                    }
                    break ;
                }
            }
            break ;
        }
        default:
        {
            /* Do nothing */
            break ;
        }
    }
}

void colorLed_Ctor(t_colorLed * const me, int cpuIdx, int prior)
{
    AO__Ctor(&(me->super), (f_EventHandler)colorLed_dispatch, cpuIdx, prior);
    AO_EventTime__Ctor(&(me->timer), SIG_TIMER, &(me->super));
}

