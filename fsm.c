#include "fsm.h"


static t_event const evtEntry = { SIG_ENTRY };
static t_event const evtExit = { SIG_EXIT };

void    FSM__Ctor(t_fsm * const me, f_StateHandler state_init)
{
    me->state = state_init;
}



void    FSM__Init(t_fsm * const me, t_event const * const evt)
{
    UTILS_ASSERT((me->state != (f_StateHandler)0), ERR_MSG_NULL_PTR_HANDLER);
    me->state(me, evt);
    me->state(me, &evtEntry);
}



void    FSM__Dispatch(t_fsm * const me, t_event const * const evt)
{

    t_enStatusFsm   status;
    f_StateHandler  prevState;

    prevState = me->state;

    UTILS_ASSERT((me->state != (f_StateHandler)0), ERR_MSG_NULL_PTR_HANDLER)
    UTILS_ASSERT((evt != (t_event *)0), ERR_MSG_NULL_PTR_EVT)
    status = me->state(me, evt);


    if (status == FSM_STATUS_TRANSIT)
    {
        /*
           Check that the exit action of previous state is executed 
           before entry to the new state 
        */
        UTILS_ASSERT((prevState != (f_StateHandler)0), ERR_MSG_NULL_PTR_HANDLER)
        prevState(me, &evtExit);
        
        /* Execute the entry action for new state transited into */
        me->state(me, &evtEntry);
    }
}