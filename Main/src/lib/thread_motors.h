#ifndef __THREAD_MOTORS_H__
#define __THREAD_MOTORS_H__

void __attribute__((noreturn)) *motors_thread(__attribute__((unused)) void *ignored);
void motor(int left, int right);
void set_max_speed(int new_max_speed);
void reset_max_speed();
int get_max_speed();

#endif
