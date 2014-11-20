#include <stdio.h>
#include <unistd.h>
#include "mod_i2c.h"
#include "compass.h"

int main() {
	double inicial = compass_orientation(), atual;
	//struct compass c;
	mod_i2c_create();
	sleep(3);
	for (;;) {
		//compass_read(&c);
		//printf("%hd\t%hd\t%hd\n", c.x, c.y, c.z);
		atual = compass_orientation();
		printf("%g - diff %g\n", atual, compass_diff(inicial, atual));
	}
	
	return 0;
}
