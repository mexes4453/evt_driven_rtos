
#ifndef AO_H
#define AO_H
/* Active object = thread + queue + statemachine */


#include "os.h"
#include "utils.h"
#include "queue.h"
#include "fsm.h"
#include "evt.h"

#define AO_TRUE (1)

typedef enum enStatusActiveObj
{
	AO_STATUS_FAIL = -1,
	AO_STATUS_SUCCESS,
}   t_enStatusActiveObj;




/** 
 * Forward declaration of active object class
 * This allows for event handler hook function prototyping prior to its
 * use in the proper active object class definition below
 */
typedef struct sActiveObj t_ActiveObj;



/**
 * @brief 
 * Active object class: thread + queue + statemachine
 * 
 */
struct sActiveObj
{
    t_fsm               fsm;
	pthread_t           thread;
	t_Queue		        xEvtQueue;
    t_osThreadParams    threadParams;
};


t_enStatusActiveObj    AO__Ctor(t_ActiveObj * const me, f_StateHandler state_init, int cpuIdx, int prior);
t_enStatusActiveObj    AO__Post(t_ActiveObj * const me, void *pData);
void                   *AO__EvtPump(void *pObj);


/* User-defined Event class */
#define AO_TOTAL_TIME_EVT (10)
typedef struct sEventTime
{
    t_event     super;
    t_ActiveObj *ao;
    int         tickCounter;
    bool        interval;
} t_eventTime;


void    AO_EventTime__Ctor(t_eventTime * const me, t_signal sig, t_ActiveObj * const ao);
void    AO_EventTime__Enable(t_eventTime *const me, int tickCount, int interval);
void    AO_EventTime__Disable(t_eventTime *const me);
void    AO_EventTime__Tick(void);



#endif /* AO_H */