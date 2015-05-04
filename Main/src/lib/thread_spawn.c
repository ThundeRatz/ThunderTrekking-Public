#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define status_perror(__msg, __errno)	do {if ((__errno)) {char __errmsg[50]; fprintf(stderr, "%s: %s\n", __msg, strerror_r(__errno, __errmsg, sizeof(__errmsg)));}} while (0)
#define status_try(cmd, errmsg)	do {int __errno = (cmd); if (__errno) {status_perror(errmsg, __errno); exit(-1);}} while (0)

void thread_spawn(void *new_thread(void *attr), void *args) {
	pthread_t thread;
	pthread_attr_t attr;

	status_try(pthread_attr_init(&attr), "pthread_attr_init");
	status_try(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED), "pthread_attr_setdetachstate");
	status_try(pthread_create(&thread, &attr, new_thread, args), "pthread_create");
}
