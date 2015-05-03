#include <stdio.h>
#include "motors.h"
#include "hc_sr04_udp.h"

#define MAX_SPEED	100

static double integral, p, i, d, ultima_amostra = 0;

void pid_init(double new_p, double new_i, double new_d) {
	p = new_p;
	i = new_i;
	d = new_d;
	integral = 0;
	ultima_amostra = 0;
}

double pid_update(double erro) {
	int amostra_antiga = ultima_amostra;
	integral += erro;
	if (integral > 500)
		integral = 500;
	if (integral < -500)
		integral = -500;
	ultima_amostra = erro;
	return erro * p + integral * i - (amostra_antiga - erro) * d;
}


int main() {
	int target, filtered = 0;
	hc_sr04_udp_packet_t sonar;
	motor_init();
	hc_sr04_udp_init();
	pid_init(.5, 0.01, 0.0005);
	for (;;) {
		scanf("%d", &target);
		printf("%d\n", target);
		filtered = 0.7 * filtered + 0.3* target;
		target = (int) pid_update((double) target);
		if (target > MAX_SPEED)
			target = MAX_SPEED;
		if (target < -MAX_SPEED)
			target = -MAX_SPEED;
		printf("PID - %d\n", target);
		if (target > 0)
			motor(MAX_SPEED, MAX_SPEED - target);
		else
			motor(MAX_SPEED + target, MAX_SPEED);
		if (!hc_sr04_recv(&sonar)) {
			printf("Sonar %llu\n", (long long unsigned) sonar.distance / 56 / 1000);
			if (sonar.distance / 56 / 1000 < 60) {
				motor(0, 0);
				return 0;
			}
		}
	}
}
