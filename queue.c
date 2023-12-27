#include "queue.h"


void    QUEUE__Init(t_Queue * const me, bool optionBlock)
{
	/*
	me->entryCnt = 0;
	me->size = 0;
	me->idxPost = 0;
	me->idxGet = 0;
	me->optionBlock = 0;
	*/
	//UTILS_MEMSET(me,0,sizeof(t_Queue))
    bzero(me, sizeof(t_Queue));
	me->state = QUEUE_STATE_EMPTY;
	me->optionBlock = optionBlock;
    me->size = QUEUE_SIZE;
}



void    QUEUE__Clear(t_Queue * const me)
{
	QUEUE_MUTEX_LOCK(me)
	UTILS_MEMSET(me->data,0,me->size)
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
			QUEUE_TRANS(QUEUE_STATE_NOT_EMPTY);
			result = QUEUE_STATUS_SUCCESS;
			break ;
		}
		case   QUEUE_STATE_NOT_EMPTY:
		case   QUEUE_STATE_NOT_FULL:	
		{
			UTILS_ASSERT((me->entryCnt < me->size), "error! queue post: NF\n")
			/* Put data in Queue */
			QUEUE_PUT_DATA(pData)
			
			/* Change queue state */
			if (me->entryCnt >= me->size)
			{
				QUEUE_TRANS(QUEUE_STATE_FULL);
			}
			else
			{
				QUEUE_TRANS(QUEUE_STATE_NOT_FULL);
			}
			result = QUEUE_STATUS_SUCCESS;
			break ;
		}
		default:	/* QUEUE_STATE_FULL */
		{
			UTILS_ASSERT((me->entryCnt >= me->size), "error! queue post: FULL\n")
			QUEUE_TRANS(QUEUE_STATE_FULL);
			if (me->optionBlock) /* Block and wait till there is space for input */
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
			QUEUE_TRANS(QUEUE_STATE_NOT_FULL);
			result = QUEUE_STATUS_SUCCESS;
			break ;
		}
		case   QUEUE_STATE_NOT_EMPTY:
		case   QUEUE_STATE_NOT_FULL:	
		{
			UTILS_ASSERT((me->entryCnt > 0), "error! queue get: NF\n")
			/* Fetch data in Queue */
			QUEUE_FETCH_DATA(ppData);
			
			/* Change queue state */
			if (me->entryCnt == 0)
			{
				QUEUE_TRANS(QUEUE_STATE_EMPTY);
			}
			else
			{
				QUEUE_TRANS(QUEUE_STATE_NOT_EMPTY);
			}
			result = QUEUE_STATUS_SUCCESS;
			break ;
		}
		default:	/* QUEUE_STATE_EMPTY */
		{
			UTILS_ASSERT((me->entryCnt == 0), "error! queue get: E\n")
			QUEUE_TRANS(QUEUE_STATE_EMPTY);
			if (me->optionBlock) /* Block and wait till there is space for input */
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



int    QUEUE__GetLevel(t_Queue * const me)
{
    return (me->entryCnt);
}