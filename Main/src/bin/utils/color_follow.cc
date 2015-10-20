/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 ThundeRatz

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include <cstdio>

#include <inttypes.h>

#include "ThreadPixy.hh"
#include "ThreadSpawn.hh"
#include "ThreadMotors.hh"
#include "ThreadProximity.hh"

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
	thread_spawn(motors_thread);
	//thread_spawn(hmc5883l_thread, NULL);
	//thread_spawn(sonar_thread, NULL);
	thread_spawn(proximity_thread);
	//leds_init();
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

		if (((int16_t) object.x) < 0) {
			corretor = MAX_Vel + ((int16_t) object.x)/2;
			motor(corretor, MAX_Vel);
			printf("1 - motor(%d, %d)\n", corretor, MAX_Vel);
		} else {
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
