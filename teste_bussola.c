/* Teste gambiarra para ver se a bússola funciona por UDP
 * Usa controle P para manter a direção do trekking */

//#define MAX_SPEED	250
//#define P			130

#define MAX_SPEED	150
#define P			100

#define JOYSTICK_NONBLOCK

#include <stdio.h>
#include <unistd.h>
#include "file_lock.h"
#include "joystick.h"
#include "mod_i2c.h"
#include "motors.h"
#include "compass.h"
#include "compass_udp.h"

int main() {
	double diff;
	float compass[3], orientacao_inicial;
	
	
	printf("Calibração da bússola\n");
	
	if (file_lock("/tmp/trekking") == -1)
		return -1;
	
	mod_i2c_create();
	if (compass_udp_init())
		return -1;
	compass_udp_recv(compass);
	orientacao_inicial = compass[0];
	
	for (;;) {
		compass_udp_recv(compass);
		diff = compass_diff(orientacao_inicial, compass[0]);
		printf("%f %f - diff %g\n", orientacao_inicial, compass[0], diff);
		if (diff > 2 || diff < -2)
			motor(0, 0);
		else if (diff > 0)
			motor(MAX_SPEED, MAX_SPEED - (int) (P * diff));
		else
			motor(MAX_SPEED + (int) (P * diff), MAX_SPEED);
	}
	
	motor(0, 0);
	return 0;
}
