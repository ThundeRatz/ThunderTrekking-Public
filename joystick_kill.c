#include <stdio.h>
#include "joystick.h"

int main () {
	int js;
	struct js_event jsev;
	
	if ((js = joystick_open("/dev/input/js0")) == -1) {
		perror("joystick_open");
		return -1;
	}
	
	while (!joystick_read(js, &jsev)) {
		if (jsev.type & JS_EVENT_BUTTON) {
			if (!(jsev.type & JS_EVENT_INIT) && jsev.value == 1) {
				return 0;
			}
		}
	}
	
	perror("joystick_read");
	return 0;
}
