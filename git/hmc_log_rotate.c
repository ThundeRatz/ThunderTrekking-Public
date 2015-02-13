#define JOYSTICK_NONBLOCK

#define MAX_SPEED	250

#include <stdio.h>
#include <unistd.h>
#include "file_lock.h"
#include "joystick.h"
#include "motors.h"
#include "compass.h"
#include "hmc5883l_udp.h"
#include "init.h"
#include "gps.h"
#include "gps_udp.h"

int main() {
	double diff, gps[2];
	float orientacao_inicial;
	int16_t compass[3];
	
	printf("Calibração da bússola\n");
	
	//init();
	
	//if (gps_udp_init() || gps_udp_recv(gps))
		//return -1;
	if (file_lock("/tmp/trekking") == -1)
		exit(-1);
	//hook_init();
	motor_init();
	if (hmc5883l_udp_init())
		return -1;
	
	//joystick_dump(js);
	motor(255, -255);
	for (;;) {
		hmc5883l_udp_recv(compass);
		printf("%hd %hd %hd\n", compass[0], compass[1], compass[2]);
	}
	
	return 0;
}
