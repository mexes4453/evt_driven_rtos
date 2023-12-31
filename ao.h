
#ifndef AO_H
#define AO_H
/* Active object = thread + queue + statemachine */


#include "os.h"
#include "utils.h"
#include "queue.h"

#define AO_TRUE (1)

typedef enum enStatusActiveObj
{
	AO_STATUS_FAIL = -1,
	AO_STATUS_SUCCESS,
	AO_STATUS_IGNORED
}   t_enStatusActiveObj;



/* define data type for signal of type int16_t */
typedef unsigned int t_signal;      



/** 
  * Event base class 
  * All other events will be a subclass of this base class.
  * They would add their own additional properties and method.
  */
typedef struct sEvent
{
    t_signal sig;
}   t_Event; 



enum enSignalsReserved
{
    SIG_INIT,
    SIG_USER
};


/** 
 * Forward declaration of active object class
 * This allows for event handler hook function prototyping prior to its
 * use in the proper active object class definition below
 */
typedef struct sActiveObj t_ActiveObj;


/**
 * @brief 
 * hook function prototype for handling the events.
 * This prototype function would contain the state-machine which would
 * discriminate based on the event type and perform corresponding action
 * and update the active object state if state transition occurs due to
 * executed action.
 */
typedef void (*f_EventHandler)(t_ActiveObj * const me, t_Event const * const evt);


/**
 * @brief 
 * Active object class: thread + queue + statemachine
 * 
 */
typedef struct sActiveObj
{
	pthread_t           thread;
	t_Queue		        xEvtQueue;
	f_EventHandler	    dispatch;
    t_osThreadParams    threadParams;
}   t_ActiveObj;


t_enStatusActiveObj    AO__Ctor(t_ActiveObj * const me, f_EventHandler dispatch, int cpuIdx, int prior);
t_enStatusActiveObj    AO__Post(t_ActiveObj * const me, void *pData);
void                   *AO__EvtPump(void *pObj);


/* User-defined Event class */
#define AO_TOTAL_TIME_EVT (10)
typedef struct sEventTime
{
    t_Event     super;
    t_ActiveObj *ao;
    int         tickCounter;
    bool        interval;
} t_EventTime;


void    AO_EventTime__Ctor(t_EventTime * const me, t_signal sig, t_ActiveObj * const ao);
void    AO_EventTime__Enable(t_EventTime *const me, int tickCount, int interval);
void    AO_EventTime__Disable(t_EventTime *const me);
void    AO_EventTime__Tick(void);



#endif /* AO_H */