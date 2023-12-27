
#ifndef QUEUE_H
#define QUEUE_H
# include "utils.h"

#define QUEUE_SIZE (10)

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
	void             *data[QUEUE_SIZE];
}   t_Queue;


/* MACROS */
#define QUEUE_TRANS(_target)(((t_Queue *)me)->state = (_target))
#define QUEUE_PUT_DATA(xData)\
	(me->data)[me->idxPost] = (xData);\
	me->idxPost = (me->idxPost + 1) % (me->size);\
	(me->entryCnt)++;\



#define QUEUE_FETCH_DATA(ppData)\
	*(ppData) = (me->data)[me->idxGet];\
	me->idxGet = (me->idxGet + 1) % (me->size);\
	(me->entryCnt)--;\
	


#define QUEUE_MUTEX_LOCK(pQueue)\
	while (((t_Queue *)(pQueue))->mtx){}\
	((t_Queue *)(pQueue))->mtx = 1;\


	
#define QUEUE_MUTEX_UNLOCK(pQueue)\
	((t_Queue *)(pQueue))->mtx = 0;\



/* FUNCTION */
void              QUEUE__Init(t_Queue * const me, bool optionBlock);
t_enStatusQueue   QUEUE__Put(t_Queue * const me, void *pData);
t_enStatusQueue   QUEUE__Get(t_Queue * const me, void **ppData);
void              QUEUE__Clear(t_Queue * const me);
int               QUEUE__GetLevel(t_Queue * const me);


#endif /* QUEUE_H */