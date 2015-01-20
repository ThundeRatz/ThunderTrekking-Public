#ifndef __PID_H__
#define __PID_H__
void pid_init(double new_p, double new_i, double new_d);
double pid_update(double erro);
#endif
