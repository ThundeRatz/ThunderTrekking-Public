#include <stdio.h>
#include "joystick.h"
#include "motors.h"
#include "leds.h"
#include "file_lock.h"

void update(float x, float y) {
	float tan, speed, absx, absy;
	if (x == 0 && y == 0)
		motor(0, 0);
	else {
		absx = x > 0 ? x : -x;
		absy = y > 0 ? y : -y;
		tan = (absy - absx) / (absy + absx);
		
		printf("%f %f %f\n", x, y, tan);
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
	
	if (file_lock("/tmp/trekking") == -1)
		return -1;
	
	int js = joystick_open("/dev/input/js0"), on = 0;
	struct js_event jsev;
	float x = 0., y = 0.;
	
	if (js == -1)
		return -1;
	joystick_dump(js);
	
	if (motors_init() == -1)
		return -1;
#warning Ler todos os eventos em buffer de uma vez
	
	setup_leds();
	
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
			if (!(jsev.type & JS_EVENT_INIT) && jsev.number == 16 && jsev.value == 0) {
				on = !on;
				if (!on)
					motor(0, 0);
			}
		}
	}
	
	motor(0, 0);
	return 0;
}
