#ifndef FSM_H
#define FSM_H

#include "evt.h"
#include "utils.h"

#define ERR_MSG_NULL_PTR_EVT "Error: Null pointer to event"
#define ERR_MSG_NULL_PTR_HANDLER "Error: Null pointer to handler"

typedef enum enStatusFsm
{
    FSM_STATUS_INIT,
	FSM_STATUS_IGNORED,
    FSM_STATUS_TRANSIT,
    FSM_STATUS_HANDLED,
}   t_enStatusFsm;

typedef struct s_fsm t_fsm;


/**
 * @brief 
 * hook function prototype for handling the events.
 * This prototype function would contain the state-machine which would
 * discriminate based on the event type and perform corresponding action
 * and update the active object state if state transition occurs due to
 * executed action.
 */
typedef t_enStatusFsm (*f_StateHandler)(t_fsm * const me, t_event const * const evt);

#define TRANS(_target) (((t_fsm *)me)->state=(f_StateHandler)_target, FSM_STATUS_TRANSIT)

struct s_fsm
{
    f_StateHandler state;
};

void    FSM__Ctor(t_fsm * const me, f_StateHandler state_init);
void    FSM__Init(t_fsm * const me, t_event const * const evt);
void    FSM__Dispatch(t_fsm * const me, t_event const * const evt);

#endif /* FSM_H */