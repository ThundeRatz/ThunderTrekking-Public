/***********************************************************************
 * Hooks são estáticos (código mais simples) já que não há a necessidade
 * de serem dinâmicos.
 * Hooks instalados nunca são chamados em paralelo (há um mutex em hook_trigger,
 * não é necessário usar um nos hooks).
***********************************************************************/

#define _POSIX_C_SOURCE 199309L
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "cont_array.h"
#include "status.h"
#include "hook.h"

static cont_array_t hooks[HOOK_MAX];
long hooks_last_call[HOOK_MAX];
static pthread_mutex_t hook_installing = PTHREAD_MUTEX_INITIALIZER;

void hook_init() {
	struct timespec res;
	memset(hooks, 0, sizeof(hooks));
	memset(hooks_last_call, 0, sizeof(hooks_last_call));
	clock_getres(CLOCK_MONOTONIC_COARSE, &res);
	printf("hook_init: precisão do timer CLOCK_MONOTONIC_COARSE = %ld.%.9ld\n", res.tv_sec, res.tv_nsec);
}

void hook_trigger(int hook) {
	size_t i;
	
	if (hook >= HOOK_MAX) {
		fprintf(stderr, "hook_install: hook inválido\n");
		abort();
	}
	
	status_abort(pthread_mutex_lock(&hook_installing), "pthread_mutex_lock");
	for (i = 0; i < hooks[hook].n_itens; i++) {
		void (*handler)(long) = hooks[hook].start[i];
		struct timespec tp;
		clock_gettime(CLOCK_MONOTONIC_COARSE, &tp);
		if (hooks_last_call[hook])
			handler(tp.tv_sec * 1000 + tp.tv_nsec / 1000000 - hooks_last_call[hook]);
		hooks_last_call[hook] = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
	}
	status_abort(pthread_mutex_unlock(&hook_installing), "pthread_mutex_lock");
}

void hook_install(int hook, void (*handler)(long)) {
	if (hook >= HOOK_MAX) {
		fprintf(stderr, "hook_install: hook inválido\n");
		abort();
	}
	
	status_abort(pthread_mutex_lock(&hook_installing), "pthread_mutex_lock");
	if (cont_array_put(&hooks[hook], handler) == -1) {
		fprintf(stderr, "hook_install: alocação de memória falhou\n");
		abort();
	}
	status_abort(pthread_mutex_unlock(&hook_installing), "pthread_mutex_lock");
}

void hook_free() {
	int i;
	for (i = 0; i < HOOK_MAX; i++)
		cont_array_free(&hooks[i]);
}
