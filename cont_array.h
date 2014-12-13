#ifndef __CONT_ARRAY_H__
#define __CONT_ARRAY_H__

#include <unistd.h>
typedef struct {
	void **start;
	size_t n_itens, max_itens;
} cont_array_t;

int cont_array_put(cont_array_t *a, void *v);
void cont_array_free(cont_array_t *a);

#endif

