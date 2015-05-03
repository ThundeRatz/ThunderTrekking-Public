#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define status_perror(__msg, __errno)	do {if ((__errno)) {char __errmsg[50]; fprintf(stderr, "%s: %s\n", __msg, strerror_r(__errno, __errmsg, sizeof(__errmsg)));}} while (0)
#define status_try(cmd, errmsg)	do {int __errno = (cmd); if (__errno) {status_perror(errmsg, __errno); exit(-1);}} while (0)
