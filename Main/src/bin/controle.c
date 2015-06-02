#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#include "joystick.h"
#include "thread_spawn.h"
#include "thread_motors.h"
#include "file_lock.h"
#include "compass.h"

#define MS	1000000

static int js;

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
	const struct timespec sleep_joystick = {.tv_sec = 0, .tv_nsec = 500 * MS};
	int on = 0;
	struct js_event jsev;
	float x = 0., y = 0.;
	
	if (file_lock("/tmp/trekking") == -1)
		exit(-1);
	while ((js = joystick_open("/dev/input/js0")) == -1) {
		printf("Joystick não conectado\n");
		if (nanosleep(&sleep_joystick, NULL) == -1)
			perror("nanosleep");
	}
	
	thread_spawn(motors_thread, NULL);
	joystick_dump(js);
	
#warning Ler todos os eventos em buffer do joystick de uma vez
	//leds_color(0, 0, 0);
	//leds_mode(MANUAL);
	//leds_color(255, 255, 255);
	//leds_mode(PULSE3);
	//leds_timestep(1);
	
	while (!joystick_read(js, &jsev)) {
		//printf("%u %d %u %u\n", jsev.time, jsev.value, jsev.type, jsev.number);
		if (jsev.type & JS_EVENT_AXIS) {
			if (jsev.number == 2) {
				x = -((float) jsev.value) / 32768;
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
					
					case 0:
					if (jsev.value) {
						if (get_max_speed() < 255)
							set_max_speed(255);
						else
							reset_max_speed();
					}
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
