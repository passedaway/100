#include "queue.h"

int q_enq_byorder(queue_t *ql, queue_t *item)
{
	int ret = 0;
	queue_t *qe = ql->next;

	if( ql->next == ql )
	{
		q_enq(ql, item);
		return 1;
	}

	for(; qe != ql; qe = qe->next)
	{
		ret = strncmp(((ele_t*)item)->data, ((ele_t*)qe)->data, 11);
//		printf("\r %s  %s", ((ele_t*)item)->data, ((ele_t*)qe)->data);
		if( ret <= 0 )
			break;
	}

	if ( ret != 0 )
		q_enq(qe, item);

	return ret;
}

void q_enq(queue_t *ql, queue_t *item)
{
	ql->prev->next = item;
	item->next = ql;
	item->prev = ql->prev;
	ql->prev = item;
}

void q_deq(queue_t *item)
{
	item->prev->next = item->next;
	item->next->prev = item->prev;

}

	
int q_count(queue_t *ql)
{
	int count = 0;
	queue_t *qe = ql;

	while(qe->next != ql){
		qe = qe->next;
		count++;
	}
	return count;
}


