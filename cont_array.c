#include <stdlib.h>
#include "cont_array.h"
#define INITIAL_ALLOC	1

int cont_array_put(cont_array_t *a, void *v) {
	if (a->start == NULL) {
		a->start = malloc(INITIAL_ALLOC * sizeof(void *));
		if (a->start == NULL)
			return -1;
		a->max_itens = INITIAL_ALLOC;
		a->n_itens = 0;
	}
	
	if (a->max_itens <= a->n_itens) {
		a->start = realloc(a->start, 2 * a->max_itens);
		if (a->start == NULL)
			return -1;
		a->max_itens *= 2;
	}
	
	a->start[a->n_itens] = v;
	a->n_itens++;
	return 0;
}

void cont_array_free(cont_array_t *a) {
	free(a->start);
}

