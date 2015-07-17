#include <stdio.h>
#include <inttypes.h>

#include "pixy_cam.h"
#include "thread_sonar.h"
#include "thread_spawn.h"
#include "thread_motors.h"
#include "thread_proximity.h"
#include "leds.h"

#define MS			1000000
#define MAX_Vel     50
#define CONVERTE    58000

enum {
	PARADO, ANDANDO
};

int main() {
//	set_max_speed(50);
	//thread_spawn(gps_thread, NULL);
	thread_spawn(motors_thread, NULL);
	//thread_spawn(hmc5883l_thread, NULL);
	//thread_spawn(sonar_thread, NULL);
	thread_spawn(proximity_thread, NULL);
	leds_init();
	pixy_cam_init();
	led_set(0, LEDS2000MS, LEDS75PC, 0);
	const struct timespec block_wait_time = {.tv_sec = 0, .tv_nsec = 100 * MS};
	const struct timespec led_wait_time = {.tv_sec = 1, .tv_nsec = 500 * MS};
	int corretor = 0, n = 0;
	uint64_t sonar_max;

	if (nanosleep(&led_wait_time, NULL))
		perror("nanosleep");

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
		//printf("Distancia = Left: %" PRIu64 " Right: %" PRIu64 "\n", sonar_l/CONVERTE, sonar_r/CONVERTE);
		//sonar_max = sonar_l/CONVERTE > sonar_r/CONVERTE ? sonar_l/CONVERTE : sonar_r/CONVERTE;
		//printf("Maior Distancia: %" PRIu64 "\n", sonar_max);

//		if (sonar_max < 20 && sonar_max > 1) {
//			const struct timespec chegou = {.tv_sec = 1, .tv_nsec = 100 * MS};
//			led_set(1 << evento_atual, LEDS2000MS, LEDS75PC, 5);
//			motor(0, 0);
//			nanosleep(&chegou, NULL);
//			return 1;
//		}
		if (sensor_contato < 4	) {
			const struct timespec chegou = {.tv_sec = 1, .tv_nsec = 100 * MS};
			led_set((1 << 0), LEDS2000MS, LEDS75PC, 5);
			motor(0, 0);
			nanosleep(&chegou, NULL);
			return 1;
		}

		if ((object.x - (PIXY_MAX_X - PIXY_MIN_X + 1) / 2) < 0) {
			corretor = MAX_Vel + object.x/2;
			motor(corretor, MAX_Vel);
			printf("motor(%d, %d)\n", corretor, MAX_Vel);
		} else if ((object.x - (PIXY_MAX_X - PIXY_MIN_X + 1) / 2) > 0) {
			corretor = MAX_Vel - object.x/2;
			motor(MAX_Vel, corretor);
			printf("motor(%d, %d)\n", MAX_Vel, corretor);
		}

		if (object.height == 0 && object.width == 0) {
			//const struct timespec search_time = {.tv_sec = 0, .tv_nsec = 500 * MS};
			motor(40, -40);
			n++;
			//nanosleep(&search_time, NULL);
			printf("Procurado Cone...\n");
			//if (n >= 30) {
//#warning Lembrar de colocar sonares
				//while (gps_distance(gps_get(), &eventos[evento_atual].pos) < 6. / 1000.)
					//motor(0, 0);
				//return 0;
			//}
		}
	}
	return 0;
}
