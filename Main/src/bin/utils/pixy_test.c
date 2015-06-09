#include <stdio.h>

#include "pixy_cam.h"

#define MS			1000000

int main() {
	const struct timespec block_wait_time = {.tv_sec = 0, .tv_nsec = 125 * MS};

	pixy_cam_init();

	for (;;) {
		pixy_block_t object;
		if (nanosleep(&block_wait_time, NULL))
			perror("nanosleep");

		pixy_cam_get(&object);
		printf("t: %4hu sig: %4hu x: %4hd y: %4hd w: %4hu h: %4hu a: %4hu\n",
			object.type, object.signature, object.x, object.y, object.width,
			object.height, object.angle);
	}

	return 0;
}
