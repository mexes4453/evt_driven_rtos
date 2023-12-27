
#ifndef AO_H
#define AO_H
/* Active object = thread + queue + statemachine */
#define AO_TRUE (1)

typedef enum enStatusActiveObj
{
	AO_STATUS_FAIL = -1,
	AO_STATUS_SUCCESS,
	AO_STATUS_IGNORED
}   t_enStatusActiveObj;

#if 0
typedef struct sActiveObj
{
	pthread_t    thread;
	t_Queue		 xEvtQueue;
	evtHandler	 dispatch;
	/* Private data */
	
}   t_ActiveObj;


t_enStatusActiveObj    AO__Ctor(t_ActiveObj * const me, evtHandler dispatch);
t_enStatusActiveObj    AO__Post(t_ActiveObj * const me, void *pData);
t_enStatusActiveObj    AO__EvtPump(void *pObj);



t_enStatusActiveObj    AO__Put(t_ActiveObj * const me, void *pData)
{
	t_enStatusActiveObj result = AO_STATUS_FAIL;
	
	result = QUEUE__Put(&(me->xEvtQueue), pData);
	return (result);
}



t_enStatusActiveObj    AO__EvtPump(void *pObj)
{
	t_activeObj       *me = (t_ActiveObj *)pObj;
	void	          *pData = NULL;
	t_enStatusQueue   result = AO_STATUS_FAIL;
	
	while (AO_TRUE)
	{
		result = QUEUE__Get(&(me->xEvtQueue), &pData);
		UTILS_ASSERT((result == QUEUE_STATUS_SUCCESS))
		me->dispatch(me, pData);
	}
}
/* Active object - End */
#endif

/* Event class */

#endif /* AO_H */