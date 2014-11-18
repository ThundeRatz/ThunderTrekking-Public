void queue_init(queue_t *q, int *start, size_t size) {
	q->start = start;
	q->size = size;
	q->first = q->last = 0;
}

int queue_empty(queue_t *q) {
	return size >= 0 && q->first == q->last;
}

int queue_put(queue_t *q, int n) {
	if (q->size < 0)
		return -1;
}
