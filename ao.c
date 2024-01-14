#include "ao.h"

t_enStatusActiveObj AO__Ctor(t_ActiveObj * const me, f_EventHandler dispatch, int cpuIdx, int prior)
{
	t_enStatusActiveObj result = AO_STATUS_FAIL;

    me->dispatch = dispatch;                /* State-machine: Assign state-machine func */
    QUEUE__Init(&(me->xEvtQueue), false);   /* Event queue:   Initialise the event queue */

    /* thread: initialise thread parameters */
    OS_InitThreadParams(&(me->threadParams), cpuIdx, prior);

    /* thread: create thread */
    result = pthread_create(&(me->thread), 
                            &(me->threadParams.attr),
                            AO__EvtPump,
                            (void *)me);
    if  (result != AO_STATUS_SUCCESS)
    {
        PERROR("create_thread", 1);
    }
    return (result);
}



t_enStatusActiveObj    AO__Post(t_ActiveObj * const me, void *pData)
{
	t_enStatusActiveObj result = AO_STATUS_FAIL;
	
	result = (t_enStatusActiveObj) QUEUE__Put(&(me->xEvtQueue), pData);
    //ft_printf("AFTER POST QUEUE -> state: %d, entries: %d\n", me->xEvtQueue.state, QUEUE__GetLevel(&(me->xEvtQueue)));
	return (result);
}



void    *AO__EvtPump(void *pObj)
{
	t_ActiveObj       *me = (t_ActiveObj *)pObj;
	void	          *pData = NULL;
	t_enStatusQueue   result = AO_STATUS_FAIL;
    t_Event const     initEvt = { SIG_INIT };
    //int readstart = 20;
	
    me->dispatch(me, &initEvt);
	
	while (AO_TRUE)
	{
        /*
        for (int x=0; x<190000000; x++)
        {
            for (int y=0; y<1; y++);
        }
        --readstart;

        //ft_printf("readstart: %d\n", readstart);
        if (readstart <= 0)
        {
        */
            //ft_printf("EVT PUMP QUEUE -> state: %d, entries: %d\n", me->xEvtQueue.state, QUEUE__GetLevel(&(me->xEvtQueue)));
            OS_BlockSignals();
		    result = QUEUE__Get(&(me->xEvtQueue), &pData);
            OS_UNBLOCK_SIGNALS()
            //ft_printf("EVT PUMP get QUEUE -> state: %d, entries: %d\n", me->xEvtQueue.state, QUEUE__GetLevel(&(me->xEvtQueue)));
		//UTILS_ASSERT((result == QUEUE_STATUS_SUCCESS), "Queue read failed\n")
            if (result == QUEUE_STATUS_SUCCESS)
            {
                //ft_printf("sig value: %d\n", ((t_Event *)pData)->sig);
                if (((t_Event *)pData)->sig == SIG_SHUTDOWN) 
                { 
                    break ;
                }
                else
                {
		            me->dispatch(me, (t_Event *)pData);
                }
            }
            else
            {
                //ft_printf("Queue read status: %d\n", result);
            }

        //}

	}
    return (NULL);
}



/* Buffer to store pointer to timers */
static  t_EventTime    *l_evtTimers[AO_TOTAL_TIME_EVT]; 

/* Stores number of all timers object created in the application */
static  uint8_t        l_evtTimeCnt = 0;   

void    AO_EventTime__Ctor(t_EventTime * const me, t_signal sig, t_ActiveObj * const ao)
{
    me->super.sig = sig;
    me->ao = ao;
    me->tickCounter = 0;
    me->interval = 0;

    UTILS_ASSERT((l_evtTimeCnt < AO_TOTAL_TIME_EVT), "evt timer count limit\n")
    l_evtTimers[l_evtTimeCnt] = me;
    ++l_evtTimeCnt;
}

void    AO_EventTime__Enable(t_EventTime *const me, int tickCount, int interval)
{
    me->tickCounter = tickCount;
    me->interval = interval;
}
void    AO_EventTime__Disable(t_EventTime *const me)
{
    me->tickCounter = 0;
}

void    AO_EventTime__Tick(void)
{
    t_EventTime  *t;
    uint8_t      idx;

    t = NULL;
    idx = 0;
    while (idx < l_evtTimeCnt)
    {
        t = l_evtTimers[idx];
        UTILS_ASSERT(t, "No timer found\n")
        if (t->tickCounter > 0) /* is the timer at idx enabled ? */
        {
            --(t->tickCounter);
            if (t->tickCounter == 0) /* did it just hit 0 ? renable or not */
            {
                AO__Post(t->ao, (void *)&(t->super));
                //ft_printf("posting timeout sig\n");
                t->tickCounter = t->interval;
            }

        }
        idx++;
    }
}