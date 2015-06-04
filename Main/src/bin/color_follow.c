#include <stdio.h>

#include "pixy_cam.h"
#include "thread_sonar.h"
#include "thread_spawn.h"

#define MS			1000000

enum {
	PARADO, ANDANDO
}

int main() {
	const struct timespec block_wait_time = {.tv_sec = 0, .tv_nsec = 10 * MS};
	int status = PARADO;

	pixy_cam_init();
	thread_spawn(thread_sonar, NULL);

	for (;;) {
		pixy_block_t object;
		if (nanosleep(&block_wait_time, NULL))
			perror("nanosleep");

		pixy_cam_get(&object);
		printf("x: %4hd y: %4hd w: %4hu h: %4hu a: %4hu\n", object.x, object.y,
			object.width, object.height, object.angle);

		if (sonar_l)
		motor();
		if ()
	}

	return 0;
}
