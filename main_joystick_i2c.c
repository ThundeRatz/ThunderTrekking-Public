#include <stdio.h>
#include "joystick.h"
#include "motors.h"

int main() {
	int js = joystick_open("/dev/input/js0");
	struct js_event jsev;
	float x = 0., y = 0., tan, speed, absx, absy;
	
	if (js == -1)
		return -1;
	joystick_dump(js);
	
	if (motors_init() == -1)
		return -1;
	
	while (!joystick_read(js, &jsev)) {
		//printf("%u %d %u %u\n", jsev.time, jsev.value, jsev.type, jsev.number);
		if (jsev.type & JS_EVENT_AXIS) {
			if (jsev.number == 2) {
				x = ((float) jsev.value) / 32768;
			} else if (jsev.number == 3) {
				y = ((float) jsev.value) / 32768;
			}
			
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
	}
	
	motor(0, 0);
	return 0;
}
