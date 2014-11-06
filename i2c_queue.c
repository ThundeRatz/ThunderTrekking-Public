#include <string.h>
// Para alocação dinâmica. Pode ser estática também
void i2c_queue_init(i2c_queue_t *q, int n) {
	q->queue = malloc(n * sizeof(i2c_data_t));
	q->size = n;
	q->start = q->end = 0;
}

i2c_data_t* i2c_queue_get(i2c_queue_t *q) {
	i2c_data_t *get = q->queue[q->start];
	if (++(q->start) == q->size)
		q->start = 0;
	return get;
}

int i2c_queue_put(i2c_queue_t *q, uint8_t dev, uint8_t reg, uint8_t data) {
	q->queue[q->end].dev = dev;
	q->queue[q->end].reg = reg;
	q->queue[q->end].data = data;
	
	if (++(q->end) == q->size)
		q->end == 0;
	
	if (q->end == q->start)
		// Pilha está cheia agora, cliente deve chamar i2c_queue_wait para esperar
		// liberar uma posição
		return 1;
	
	return 0;
}
