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
	
	int js;
	struct js_event jsev;
	
	mod_i2c_create();
	if (compass_udp_init())
		return -1;
	compass_udp_recv(compass);
	orientacao_inicial = compass[0];
	
	while ((js = joystick_open("/dev/input/js0")) == -1)
		sleep(1);
	
	joystick_dump(js);
	for (;;)
		switch (joystick_read(js, &jsev)) {
			case 0:
			compass_udp_recv(compass);
			diff = compass_diff(orientacao_inicial, compass[0]);
			printf("%f - diff %g\n", compass[0], diff);
			if (diff > 0)
				motor(250, 250 - (int) (100. * diff));
			else
				motor(250 - (int) (100. * diff), 250);
			break;
			
			case 1:
			if (jsev.type & JS_EVENT_BUTTON) {
				if (!(jsev.type & JS_EVENT_INIT)) {
					switch (jsev.number) {
						case 16:
						/*
						printf("Iniciando programa principal...\n");
						char *newargv[] = {"/home/pi/build/trekking", NULL};
						if (execve(newargv[0], newargv, NULL) == -1)
							perror("execve");
						break;
						*/
						goto exit;
						
						case 11:
						compass_udp_recv(compass);
						diff = compass_diff(orientacao_inicial, compass[0]);
						printf("%f - diff %g\n", compass[0], diff);
						if (diff > 0)
							motor(250, 250 - (int) (50 * diff));
						else
							motor(250 - (int) (50 * diff), 250);
						break;
					}
				}
			}
			break;
			
			case -1:
			perror("joystick_read");
			goto exit;
		}
	
	exit:
	motor(0, 0);
	return 0;
}
