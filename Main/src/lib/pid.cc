#include <math.h>

#include "pid.h"

void pid_init(pid_correction_t *pid, double new_p, double new_i, double new_d) {
	pid->p = new_p;
	pid->i = new_i;
	pid->d = new_d;
	pid->integral = 0;
	// vai dar um fator derivativo errado na primeira amostra
	pid->last_sample = 0;
}

double pid_update(pid_correction_t *pid, double erro) {
	int amostra_antiga = pid->last_sample;
	pid->integral += erro;
	pid->last_sample = erro;
	return - erro * pid->p - pid->integral * pid->i + (amostra_antiga - erro) * pid->d;
}

double pass_filter(double last, double now, double pass) {
	return now * pass + last * (1 - pass);
}
