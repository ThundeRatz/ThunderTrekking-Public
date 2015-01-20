#include <math.h>

static double integral, p, i, d, ultima_amostra = 0;

void pid_init(double new_p, double new_i, double new_d) {
	p = new_p;
	i = new_i;
	d = new_d;
	integral = 0;
}

double pid_update(double erro) {
	int amostra_antiga = ultima_amostra;
	if (erro > M_PI / 4)
		return -255;
	if (erro < -M_PI / 4)
		return 255;
	integral += erro;
	ultima_amostra = erro;
	return - erro * p - integral * i + (amostra_antiga - erro) * d;
}
