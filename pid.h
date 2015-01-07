#ifndef __PID_H__
#define __PID_H__
void pid_init(int new_p, int new_i, int new_d);
int pid_update(int erro);
#endif
