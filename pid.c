#include <math.h>

static int integral, p, i, d, ultima_amostra = 0;

void pid_init(int new_p, int new_i, int new_d) {
	p = new_p;
	i = new_i;
	d = new_d;
	integral = 0;
}

int pid_update(int erro) {
	int amostra_antiga = ultima_amostra;
	if (erro > M_PI / 8)
		return -255;
	if (erro < -M_PI / 8)
		return 255;
	integral += erro;
	ultima_amostra = erro;
	return - erro * p - integral * i + (amostra_antiga - erro) * d;
}
