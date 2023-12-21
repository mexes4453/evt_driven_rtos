#ifndef UTILS_H
#define UTILS_H
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "./ft_printf/ft_printf.h"

# define COL_BLUE       "\033[0;34m"
# define COL_RED        "\033[0;31m"
# define COL_YELLOW     "\033[0;33m"
# define COL_MAGENTA    "\033[0;35m"
# define COL_DEFAULT    "\033[0m"


/* MACROS */
#define PERROR(msg,errNo)\
{\
    perror(msg); exit(errNo); \
}\

#define UTILS_PRINTF(stringFmt, ...)\
    (ft_printf(stringFmt, ##__VA_ARGS__)) \



/* FUNCTIONS */
void UTILS_PrintTxt(char *msg);
void UTILS_PrintInt(uint64_t nbr);


/*
 * This macro emulates the memcpy function in <string.h>
 * It copies the byte data from source address to destination
 * address given the number of bytes to copy.
 * @dstAddr : ptr to destination buffer
 * @srcAddr : ptr to source buffer
 * @byteCnt : Number of bytes to copy from src to dst.
 * -----------------
 * !!! ATTENTION !!!
 * -----------------
 * Only use when enough buffer is provided for dst.
 */
#define UTILS_MEMCPY(dstAddr,srcAddr,byteCnt)\
    if ((dstAddr) != NULL) && ((srcAddr) != NULL))\
    {\
        for (int idx=0; idx<(byteCnt); idx++)\
        {\
            (dstAddr)[idx] = (srcAddr)[idx];\
        }\
    }\


/*
 * This macro emulates the memcpy function in <string.h>
 * It write the given value to destination address, given the
 * number of bytes to write.
 * @addr    : ptr to destination buffer
 * @value   : data to write
 * @byteCnt : Number of bytes
 * -----------------
 * !!! ATTENTION !!!
 * -----------------
 * Only use when enough buffer is provided for dst.
 */
#define UTILS_MEMSET(addr,value,byteCnt)\
    if ((addr) != NULL)\
    {\
        for (int idx=0; idx<(byteCnt); idx++)\
        {\
            (addr)[idx] = (value);\
        }\
    }\



/* Update this macro using minimalistic printf */
#define UTILS_ASSERT(condition,msg,msgCnt)\
    if (!(condition))\
	{\
	    write(2, (msg), (msgCnt);\
		while (1){}\
	}\
	


#define QUEUE_TRANS(_target)(((tQueue *)me)->state = _target)
#define QUEUE_PUT_DATA(data)\
	(me->data)[idxPost] = (data);\
	me->idxPost = (me->idxPost + 1) % (me->size);\
	(me->entryCnt)++;\



#define QUEUE_FETCH_DATA(ppData)\
	*(ppData) = (me->data)[idxGet];\
	me->idxGet = (me->idxGet + 1) % (me->size);\
	(me->entryCnt)--;\
	


#define QUEUE_MUTEX_LOCK(pQueue)\
	while (((tQueue *)(pQueue))->mtx){}\
	((tQueue *)(pQueue))->mtx = 1;\


	
#define QUEUE_MUTEX_UNLOCK(pQueue)\
	((tQueue *)(pQueue))->mtx = 0;\



/* message queue

	+ mq_create
	+ mq_send
	+ mq_recv
	+ mq_clear
	+ mq_getAttr
	+ mq_setAttr
	+ mq_isFull
	+ mq_isEmpty
	typedef struct 
	{
		int prio,
		int size,
		bool block,
	}	queueAttr;
	

*/

/* for OS_QUEUE */
// Requires a forward declaration of the Active object

typedef enum enStatusQueue
{
	QUEUE_STATUS_FAIL = -1,
	QUEUE_STATUS_SUCCESS,
	QUEUE_STATUS_IGNORED
}   t_enStatusQueue;


typedef enum enStateQueue
{
	QUEUE_STATE_EMPTY,
	QUEUE_STATE_NOT_EMPTY,
	QUEUE_STATE_NOT_FULL,
	QUEUE_STATE_FULL
}   tenStateQueue;


typedef struct sQueue
{
    tenStateQueue    state;
    uint8_t          entryCnt;
	uint8_t          size;
	uint8_t          idxPost;
	uint8_t          idxGet;
	uint8_t          mtx;
	bool             optionBlock;
	void             *data[size];
}   t_Queue;


void              QUEUE__Init(t_Queue * const me);
t_enStatusQueue   QUEUE__Put(t_Queue * const me, void *pData);
t_enStatusQueue   QUEUE__Get(t_Queue * const me, void **ppData);
void              QUEUE__Clear(t_Queue * const me);


void    QUEUE__Init(t_Queue * const me, bool optionBlock)
{
	/*
	me->entryCnt = 0;
	me->size = 0;
	me->idxPost = 0;
	me->idxGet = 0;
	me->optionBlock = 0;
	*/
	UTILS_MEMSET(me,0,sizeof(t_Queue))
	me->state = QUEUE_STATE_EMPTY;
	me->optionBlock = optionBlock;
}


void    QUEUE__Clear(t_Queue * const me)
{
	QUEUE_MUTEX_LOCK(me)
	UTILS_MEMSET(me->data,0,size)
	me->idxPost = 0;
	me->idxGet = 0;
	me->entryCnt = 0;
	me->state = QUEUE_STATE_EMPTY;
	QUEUE_MUTEX_UNLOCK(me)
}


t_enStatusQueue    QUEUE__Put(t_Queue * const me, void *pData)
{
	t_enStatusQueue result = QUEUE_STATUS_FAIL;
	QUEUE_MUTEX_LOCK(me)
	
	switch (me->state)
	{
		case   QUEUE_STATE_EMPTY:
		{
			QUEUE_PUT_DATA(pData)
			QUEUE_TRANS(QUEUE_STATE_NOT_EMPTY)
			result = QUEUE_STATUS_SUCCESS;
			break ;
		}
		case   QUEUE_STATE_NOT_EMPTY:
		case   QUEUE_STATE_NOT_FULL:	
		{
			UTILS_ASSERT((me->entryCnt < me->size), "error! queue post: NF\n", 23)
			/* Put data in Queue */
			QUEUE_PUT_DATA(pData)
			
			/* Change queue state */
			if (me->entryCnt >= me->size)
			{
				QUEUE_TRANS(QUEUE_STATE_FULL)
			}
			else
			{
				QUEUE_TRANS(QUEUE_STATE_NOT_FULL)
			}
			result = QUEUE_STATUS_SUCCESS;
			break ;
		}
		default:	/* QUEUE_STATE_FULL */
		{
			UTILS_ASSERT((me->entryCnt >= me->size), "error! queue post: FULL\n", 25)
			QUEUE_TRANS(QUEUE_STATE_FULL);
			if (optionBlock) /* Block and wait till there is space for input */
			{
				while (me->entryCnt >= me->size){}	
				QUEUE_PUT_DATA(pData)
				result = QUEUE_STATUS_SUCCESS;
			}
			else /* simple ignore data */
			{
				result = QUEUE_STATUS_IGNORED;
			}
			break ;
		}
	}
	QUEUE_MUTEX_UNLOCK(me)
	return (result);
}


t_enStatusQueue    QUEUE__Get(t_Queue * const me, void **ppData)
{
	t_enStatusQueue result = QUEUE_STATUS_FAIL;
	
	QUEUE_MUTEX_LOCK(me);
	switch (me->state)
	{
		case   QUEUE_STATE_FULL: /* block or return fail */
		{
			QUEUE_FETCH_DATA(ppData)
			QUEUE_TRANS(QUEUE_STATE_NOT_FULL)
			result = QUEUE_STATUS_SUCCESS;
			break ;
		}
		case   QUEUE_STATE_NOT_EMPTY:
		case   QUEUE_STATE_NOT_FULL:	
		{
			UTILS_ASSERT((me->entryCnt > 0), "error! queue get: NF\n", 21)
			/* Fetch data in Queue */
			QUEUE_FETCH_DATA(ppData);
			
			/* Change queue state */
			if (me->entryCnt == 0)
			{
				QUEUE_TRANS(QUEUE_STATE_EMPTY)
			}
			else
			{
				QUEUE_TRANS(QUEUE_STATE_NOT_EMPTY)
			}
			result = QUEUE_STATUS_SUCCESS;
			break ;
		}
		default:	/* QUEUE_STATE_EMPTY */
		{
			UTILS_ASSERT((me->entryCnt == 0), "error! queue get: E\n", 20)
			QUEUE_TRANS(QUEUE_STATE_EMPTY);
			if (optionBlock) /* Block and wait till there is space for input */
			{
				while (me->entryCnt >= me->size){}	
				QUEUE_FETCH_DATA(ppData)
				result = QUEUE_STATUS_SUCCESS;
			}
			else /* simple ignore data */
			{
				result = QUEUE_STATUS_IGNORED;
			}
			break ;
		}
	}
	QUEUE_MUTEX_UNLOCK(me);
	return (result);
}










/* Active object = thread + queue + statemachine */
#define AO_TRUE (1)

typedef enum enStatusActiveObj
{
	AO_STATUS_FAIL = -1,
	AO_STATUS_SUCCESS,
	AO_STATUS_IGNORED
}   t_enStatusActiveObj;

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


/* Event class */

#endif /* UTILS_H */
