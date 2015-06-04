#include <stdio.h>

#include "pixy_cam.h"
#include "thread_sonar.h"
#include "thread_spawn.h"

#define MS			1000000
#define MAX_Vel     100
#define CONVERTE	58000

enum {
	PARADO, ANDANDO
}

int main() {
	const struct timespec block_wait_time = {.tv_sec = 0, .tv_nsec = 10 * MS};
	int status = PARADO, corretor;

	pixy_cam_init();
	thread_spawn(thread_sonar, NULL);

	for (;;) {
		pixy_block_t object;
		if (nanosleep(&block_wait_time, NULL))
			perror("nanosleep");

		pixy_cam_get(&object);
		printf("x: %4hd y: %4hd w: %4hu h: %4hu a: %4hu\n", object.x, object.y,
			object.width, object.height, object.angle);

		if (status == PARADO && sonar_l/CONVERTE>130)
			status = ANDANDO;

		if (status == ANDANDO && sonar_)

		if (largest_block.x<=0){
			corretor = MAX_Vel + largest_block.x;
			motor(corretor,MAX_Vel);
		}
		if (largest_block.x>0){
			corretor = MAX_Vel - largest_block.x;
			motor(MAX_Vel,corretor);
		}
	}

	return 0;
}
