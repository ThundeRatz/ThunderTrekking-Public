#ifndef __MOTORS_H__
#define __MOTORS_H__
int motor_right(int value);
int motor_left(int value);
int motor(int right, int left);
void motor_wait();
void motor_init();
#endif
