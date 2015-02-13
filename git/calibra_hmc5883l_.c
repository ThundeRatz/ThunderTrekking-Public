#define JOYSTICK_NONBLOCK

//#define MAX_SPEED	250
//#define P			130

#define MAX_SPEED	260//250
#define P			180//50
#define I			0.01//0.5
#define D			10

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "file_lock.h"
#include "joystick.h"
#include "motors.h"
#include "compass.h"
#include "hmc5883l_udp.h"
#include "init.h"
#include "gps.h"
#include "gps_udp.h"
#include "pid.h"

static int running = 1;

static void sig_handler(int signo) {
	switch (signo) {
		case SIGINT:
		running = 0;
	}
}

int main() {
	double diff;
	gps_coord_t gps_inicial, gps_final;
	int16_t compass[3];
	double atual, orientacao_inicial;
	struct sigaction sinal;
	
	printf("Calibração da bússola\n");
	
	memset((char *) &sinal, 0, sizeof(sinal));
	sinal.sa_handler = sig_handler;
	if (sigaction(SIGINT, &sinal, NULL) == -1)  {
		perror("sigaction");
		return -1;
	}
	
	init();
	
	if (gps_udp_init() || gps_udp_recv(&gps_inicial))
		return -1;
	
	printf("GPS inicial: %g %g\n", gps_inicial.longitude, gps_inicial.latitude);
	if (hmc5883l_udp_init())
		return -1;
	hmc5883l_udp_recv(compass);
	orientacao_inicial = compass_orientation(compass[0], compass[2]);
	atual = orientacao_inicial;
	joystick_dump(js);
	pid_init(P, I, D);
	while (running) {
		hmc5883l_udp_recv(compass);
		atual = compass_orientation(compass[0], compass[2]);//pass_filter(atual, compass_orientation(compass[0], compass[2]), 1.);
		diff = compass_diff(orientacao_inicial, atual);
		printf("%f - diff %g", atual, diff);
		diff = pid_update(diff);
		printf(" - pid %g\n", diff);
		if (diff > 0)
			motor(MAX_SPEED - (int) (diff), MAX_SPEED);
		else
			motor(MAX_SPEED, MAX_SPEED + (int) (diff));
	}
	
	motor(0, 0);
	gps_udp_recv(&gps_final);
	gps_udp_recv(&gps_final);
	printf("Bússola: %g, real: %g, diff %g\n", orientacao_inicial, azimuth(&gps_inicial, &gps_final), orientacao_inicial - azimuth(&gps_inicial, &gps_final));
	
	return 0;
}
