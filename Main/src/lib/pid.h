#ifndef __PID_H__
#define __PID_H__

typedef struct {
	double p, i, d, integral, last_sample;
} pid_correction_t;

void pid_init(pid_correction_t *pid, double new_p, double new_i, double new_d);
double pid_update(pid_correction_t *pid, double erro);
double pass_filter(double last, double now, double pass);

#endif
