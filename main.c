#include <stdio.h>
#include <unistd.h>
#include "joystick.h"
#include "motors.h"
#include "leds.h"
#include "file_lock.h"
#include "mod_i2c.h"
#include "compass.h"
#include "init.h"

void update(float x, float y) {
	float tan, speed, absx, absy;
	if (x == 0 && y == 0)
		motor(0, 0);
	else {
		absx = x > 0 ? x : -x;
		absy = y > 0 ? y : -y;
		tan = (absy - absx) / (absy + absx);
#ifdef DEBUG
		printf("x = %f. y =  %f, tan = %f\n", x, y, tan);
#endif
		if (absy > absx)
			speed = absy;
		else
			speed = absx;
		
		if (x >= 0. && y >= 0.)
			motor((int) -(speed * 255), (int) -(tan * speed * 255));
		else if (x < 0. && y >= 0.)
			motor((int) -(tan * speed * 255), (int) -(speed * 255));
		else if (x < 0. && y < 0.)
			motor((int) (speed * 255), (int) (tan * speed * 255));
		else if (x >= 0. && y < 0.)
			motor((int) (tan * speed * 255), (int) (speed * 255));
	}
	
}


int main() {
	int on = 0;
	struct js_event jsev;
	float x = 0., y = 0.;
	
	init();
	joystick_dump(js);
	
#warning Ler todos os eventos em buffer do joystick de uma vez
	leds_color(0, 0, 0);
	leds_mode(MANUAL);
	//leds_color(255, 255, 255);
	//leds_mode(PULSE3);
	//leds_timestep(1);
	
	while (!joystick_read(js, &jsev)) {
		//printf("%u %d %u %u\n", jsev.time, jsev.value, jsev.type, jsev.number);
		if (jsev.type & JS_EVENT_AXIS) {
			if (jsev.number == 2) {
				x = ((float) jsev.value) / 32768;
				if (on)
					update(x, y);
			} else if (jsev.number == 3) {
				y = ((float) jsev.value) / 32768;
				if (on)
					update(x, y);
			}
		} else if (jsev.type & JS_EVENT_BUTTON) {
			if (!(jsev.type & JS_EVENT_INIT)) {
				switch (jsev.number) {
					case 16:
					if (jsev.value == 0) {
						on = !on;
						if (!on)
							motor(0, 0);
					}
					break;
					
					case 3:
					
					break;
					
					case 11:
					printf("Iniciando calibração da bússola...\n");
					char *newargv[] = {"/home/pi/build/calibra_bussola", NULL};
					if (execve(newargv[0], newargv, NULL) == -1)
						perror("execve");
					break;
				}
			}
		}
	}
	
	motor(0, 0);
	return 0;
}
