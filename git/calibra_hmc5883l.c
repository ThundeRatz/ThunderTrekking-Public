#define JOYSTICK_NONBLOCK

//#define MAX_SPEED	250
//#define P			130

#define MAX_SPEED	255//250
#define P			100//180//50
#define I			0.2
#define D			2//10

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
#include "hc_sr04_udp.h"

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
	uint64_t sonar0 = 500 * 58 * 1000, sonar1 = 500 * 58 * 1000;
	hc_sr04_udp_packet_t amostra_sonar;
	
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
	if (hc_sr04_udp_init())
		return -1;
	
	printf("GPS inicial: %.15g %.15g\n", gps_inicial.latitude, gps_inicial.longitude);
	if (hmc5883l_udp_init())
		return -1;
	
	hmc5883l_udp_recv(compass);
	//orientacao_inicial = compass_orientation(compass[0], compass[2]);
	//printf("Azimuth: %g\n", orientacao_inicial);
	
	gps_final.latitude = -23.5552046;
	gps_final.longitude = -46.7303391;
	orientacao_inicial = azimuth(&gps_inicial, &gps_final);
	printf("Azimuth: %g - Distância: %g\n", orientacao_inicial, gps_distance(&gps_inicial, &gps_final));
	//return 0;
	joystick_dump(js);
	pid_init(P, I, D);
	while (running) {
		//uint64_t distance;d
		/*if (gps_udp_has_data()) {
			gps_udp_recv(&gps_inicial);
			orientacao_inicial = azimuth(&gps_inicial, &gps_final);
			printf("GPS: %.15g %.15g (orientação nova %g)\n", gps_inicial.latitude, gps_inicial.longitude, orientacao_inicial);
		}*/
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
		/*if (diff > 0)
			motor(250, 150);
		else
			motor(150, 250);*/
		/*if (!hc_sr04_recv(&amostra_sonar)) {
			if (amostra_sonar.id == 0) {
				sonar0 = pass_filter(amostra_sonar.distance, sonar0, 0.8);
				printf("Sonar 0: %llu\n", (long long unsigned) sonar0 / 58 / 1000);
				if (sonar0 < 180 * 58 * 1000) {
					motor(0, 0);
					return 0;
				}
			} else if (amostra_sonar.id == 1) {
				sonar1 = pass_filter(amostra_sonar.distance, sonar0, 0.8);
				printf("Sonar 1: %llu\n", (long long unsigned) sonar1 / 58 / 1000);
				if (sonar1 < 180 * 58 * 1000) {
					motor(0, 0);
					return 0;
				}
			}
		}*/
	}
	
	motor(0, 0);
	//gps_udp_recv(&gps_final);
	//gps_udp_recv(&gps_final);
	//printf("Bússola: %g, real: %g, diff %g\n", orientacao_inicial, azimuth(&gps_inicial, &gps_final), orientacao_inicial - azimuth(&gps_inicial, &gps_final));
	
	return 0;
}
