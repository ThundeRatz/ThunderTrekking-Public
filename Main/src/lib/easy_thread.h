#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define EASY_THREAD(thread_name) void __attribute__((noreturn)) *thread_name(__attribute__((unused)) void *_ignored)

typedef pthread_mutex_t easy_mutex_t;

static inline void easy_perror(char *function, int error_number) {
    char message[50];
    fprintf(stderr, "%s: %s\n", function, strerror_r(error_number, message, sizeof(message)));
}

static inline void easy_mutex_lock(easy_mutex_t *m) {
    int return_value = pthread_mutex_lock(m);
    if (return_value) {
        easy_perror("easy_mutex_lock", return_value);
        exit(-1);
    }
}

static inline void easy_mutex_unlock(easy_mutex_t *m) {
    int return_value = pthread_mutex_unlock(m);
    if (return_value) {
        easy_perror("easy_mutex_unlock", return_value);
        exit(-1);
    }
}
