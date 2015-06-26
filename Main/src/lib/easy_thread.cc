#include <pthread.h>

#include "easy_thread.h"

#define status_try(cmd, errmsg)	do {int __errno = (cmd); if (__errno) {status_perror(errmsg, __errno); exit(-1);}} while (0)

void easy_thread(EASY_THREAD(new_thread), void *args) {
	pthread_t thread;
	pthread_attr_t attr;
    int error_number;

    if ((error_number = pthread_attr_init(&attr))) {
        easy_perror("pthread_attr_init", error_number);
        exit(-1);
    }
    if ((error_number = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))) {
        easy_perror("pthread_attr_setdetachstate", error_number);
        exit(-1);
    }
    if ((error_number = pthread_create(&thread, &attr, new_thread, args))) {
        easy_perror("pthread_create", error_number);
        exit(-1);
    }
}
