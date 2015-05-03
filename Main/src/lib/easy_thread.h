#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define EASY_THREAD(thread_name) void __attribute__((noreturn)) *thread_name(__attribute__((unused)) void *_ignored)

typedef pthread_mutex_t easy_mutex_t;

static easy_perror(char *function, int error_number) {
    char message[50];
    fprintf(stderr, "%s: %s\n", function, strerror_r(error_number, message, sizeof(message)));
}

void easy_mutex_lock(easy_mutex_t *m) {
    int return_value = pthread_mutex_lock(m);
    if (return_value) {
        easy_perror("easy_mutex_lock", m);
        exit(-1);
    }
}

void easy_mutex_unlock(easy_mutex_t *m) {
    int return_value = pthread_mutex_unlock(m);
    if (return_value) {
        easy_perror("easy_mutex_unlock", m);
        exit(-1);
    }
}

void easy_thread(EASY_THREAD(new_thread), void *args) {
	pthread_t thread;
	pthread_attr_t attr;

	status_try(pthread_attr_init(&attr), "pthread_attr_init");
	status_try(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED), "pthread_attr_setdetachstate");
	status_try(pthread_create(&thread, &attr, new_thread, args), "pthread_create");
}
