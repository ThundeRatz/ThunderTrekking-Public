#warning reg 8 bits?

#define i2c_queue_empty(q)		(q->start == q->end)

typedef struct {
	uint8_t dev;
	uint8_t reg;
	uint8_t data;
} i2c_data_t;

typedef struct {
	i2c_data_t *queue;
	int size, start, end;
} i2c_queue_t;
