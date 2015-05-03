#define JOYSTICK_NONBLOCK

//#define MAX_SPEED	250
//#define P			130

#define MAX_SPEED	150
#define P			100

#include <stdio.h>
#include <unistd.h>
#include "file_lock.h"
#include "joystick.h"
#include "motors.h"
#include "compass.h"
#include "compass_udp.h"
#include "init.h"
#include "gps.h"
#include "gps_udp.h"

int main() {
	double diff, gps[2];
	float compass[3], orientacao_inicial;
	struct js_event jsev;
	
	printf("Calibração da bússola\n");
	
	init();
	
	if (gps_udp_recv(gps))
		return -1;
	
	if (compass_udp_init())
		return -1;
	compass_udp_recv(compass);
	orientacao_inicial = compass[0];
	
	joystick_dump(js);
	for (;;) {
		compass_udp_recv(compass);
		diff = compass_diff(orientacao_inicial, compass[0]);
		printf("%f - diff %g\n", compass[0], diff);
		if (diff > 0)
			motor(MAX_SPEED, MAX_SPEED - (int) (P * diff));
		else
			motor(MAX_SPEED + (int) (P * diff), MAX_SPEED);
		}
	
	exit:
	motor(0, 0);
	motor_wait();
	return 0;
}
