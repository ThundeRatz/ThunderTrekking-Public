/***********************************************************************
 * simple_queue.c
 * Interface de pilha em array. Não retorna erro se pilha estiver cheia.
***********************************************************************/

#ifdef DEBUG
#include <stdio.h>	
#include "colors.h"
#endif

#define ADVANCE(var)		if (++(q->var) == q->size) q->var = 0
#define ADVANCE_VAR(var, limit)	if (++(var) == (limit)) var = 0

void simple_queue_init(simple_queue_t *q, int *start, size_t size) {
	q->start = start;
	q->size = size;
	q->first = q->last = 0;
}

int simple_queue_empty(simple_queue_t *q) {
	return size >= 0 && q->first == q->last;
}

void simple_queue_put(simple_queue_t *q, int n) {
#ifdef DEBUG
	if (q->size < 0) {
		fprintf(stderr, CRED "queue_put: lista cheia\n" CRESET);
		return;
	}
#endif
	q->start[q->last] = n;
	ADVANCE(last);
	if (q->start == q->last)
		q->size = -q->size;
}

int simple_queue_get(simple_queue_t *q) {
	int ret;
	
	if (queue_empty(q))
		return -1;
	
	ret = q->start[q->first]
	if (q->size < 0)
		q->size = -q->size;
	ADVANCE(first);
	return ret;
}

int simple_queue_search(simple_queue_t *q, int n) {
	int ret, size;
	
	ret = q->first;
	if (size < 0) {
		if (q->start[q->first] == n)
			return q->first;
		
		size = -q->size;
		ADVANCE_VAR(ret, size);
	} else
		size = q->size;
	while (ret != q->last) {
		if (q->start[ret] == n)
			return ret;
		ADVANCE_VAR(ret, size);
	}
	return -1;
}
