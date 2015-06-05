#include <stdio.h>
#include <inttypes.h>

#include "pixy_cam.h"
#include "thread_sonar.h"
#include "thread_spawn.h"
#include "thread_motors.h"

#define MS			1000000
#define MAX_Vel     50
#define CONVERTE    58000

enum {
	PARADO, ANDANDO
};

int main() {
//	set_max_speed(50);

	const struct timespec block_wait_time = {.tv_sec = 0, .tv_nsec = 100 * MS};
	int status = PARADO;
	int corretor = 0;
	uint64_t sonar_min;
	pixy_cam_init();
	thread_spawn(sonar_thread, NULL);
	thread_spawn(motors_thread, NULL);
//	for (;;) {
//		printf("motor(100, 100)\n");
//		motor(100, 100);
//		if (nanosleep(&block_wait_time, NULL))
//                      perror("nanosleep");
//	}
	for (;;) {
		pixy_block_t object;
		if (nanosleep(&block_wait_time, NULL))
			perror("nanosleep");

		pixy_cam_get(&object);
		printf("Objeto = x: %4hd y: %4hd w: %4hu h: %4hu a: %4hu\n", object.x, object.y,
			object.width, object.height, object.angle);
		printf("Distancia = Left: %" PRIu64 " Right: %" PRIu64 "\n", sonar_l/CONVERTE, sonar_r/CONVERTE);

		sonar_min = sonar_l/CONVERTE < sonar_r/CONVERTE ? sonar_l/CONVERTE : sonar_r/CONVERTE;
		printf("Menor Distancia: %" PRIu64 "\n", sonar_min);
		if (status == PARADO && sonar_min > 15)
			status = ANDANDO;

		else if (status == ANDANDO && sonar_min < 40)
			status = PARADO;

		if (status == ANDANDO) {
			if (object.x < 0) {
				corretor = MAX_Vel + object.x;
				motor(corretor, MAX_Vel);
				printf("motor(%d, %d)\n", corretor, MAX_Vel);
			} else if (object.x > 0) {
				corretor = MAX_Vel - object.x;
				motor(MAX_Vel, corretor);
				printf("motor(%d, %d)\n", MAX_Vel, corretor);
			}

			if (object.height == 0 && object.width == 0) {
				motor(0, 0);
				printf("motor(0, 0)\n");
			}
		} else {
			motor(0, 0);
			printf("motor(0, 0)\n");
		}
	}

	return 0;
}