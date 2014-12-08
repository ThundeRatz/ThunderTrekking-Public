#include <stdio.h>
#include <unistd.h>
#include "file_lock.h"
#include "joystick.h"
#include "mod_i2c.h"
#include "motors.h"
#include "compass.h"

int main() {
	double orientacao_inicial = compass_orientation(), diff;
	printf("Calibração da bússola\n");
	
	if (file_lock("/tmp/trekking") == -1)
		return -1;
	
	int js;
	struct js_event jsev;
	
	mod_i2c_create();
	
	while ((js = joystick_open("/dev/input/js0")) == -1)
		sleep(1);
	
	joystick_dump(js);
	
	while (!joystick_read(js, &jsev)) {
		if (jsev.type & JS_EVENT_BUTTON) {
			if (!(jsev.type & JS_EVENT_INIT)) {
				switch (jsev.number) {
					case 16:
					printf("Iniciando programa principal...\n");
					char *newargv[] = {"/home/pi/build/trekking", NULL};
					if (execve(newargv[0], newargv, NULL) == -1)
						perror("execve");
					break;
					
					case 11:
					diff = compass_diff(orientacao_inicial, compass_orientation());
					if (diff > 0)
						motor(250, 250 - (int) (5 * diff));
					else
						motor(250 - (int) (5 * diff), 250);
					break;
				}
			}
		}
	}
	
	motor(0, 0);
	return 0;
}
