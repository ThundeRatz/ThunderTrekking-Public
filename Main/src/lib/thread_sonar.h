#ifndef __THREAD_SONAR_H__
#define __THREAD_SONAR_H__

#include <stdint.h>

extern uint64_t sonar_l, sonar_r;
void __attribute__((noreturn)) *sonar_thread(__attribute__((unused)) void *ignored);

#endif
