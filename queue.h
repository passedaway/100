#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue_s{
	struct queue_s *prev;
	struct queue_s *next;
}queue_t;

typedef struct ele_s{
	queue_t qb;
	char *data;
}ele_t;

void q_enq(queue_t *ql, queue_t *item);
void q_deq(queue_t *item);
int q_count(queue_t *ql);
int q_enq_byorder(queue_t *ql, queue_t *item);

#endif

