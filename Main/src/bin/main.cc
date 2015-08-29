#define COMPASS_P	5

#define JOYSTICK_NONBLOCK
#define ERRO_MAX M_PI/10 // 20º
//#define VELOCIDADE_MAX 50
#define FATOR 2/3

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "ThreadProximity.hh"
#include "ThreadHmc5883l.hh"
#include "ThreadMotors.hh"
#include "ThreadSpawn.hh"
#include "ThreadSonar.hh"
#include "ThreadPixy.hh"
#include "ThreadGPS.hh"

#include "file_lock.h"
#include "joystick.h"
#include "compass.h"
#include "leds.h"
#include "pid.h"

#define MAX_Vel        50
#define VELOCIDADE_MAX 50
#define CONVERTE    58000

#define len(array)	((&array)[1] - array)
#define MS	1000000

static void executa_evento(int evento_atual);
static int joystick, quadrado = 0;
static pixy_block_t pixy;
static const struct timespec cycle = {.tv_sec = 0, .tv_nsec = 10 * MS};

typedef struct {
	GPS pos;
	double margem_gps, margem_objetivo;
	int tem_cone, desvio;
} evento;

evento eventos[] = {
//	{.pos = {.latitude = -23.64691151, .longitude = -46.57286750}, .margem_gps = 4. / 1000., .tem_cone = 1, .desvio = 0},
//	{.pos = {.latitude = -23.64673319, .longitude = -46.57228128}},

	// Competição

	{.pos = {.latitude = -23.64696611, .longitude = -46.57281075}},
	{.pos = {.latitude = -23.64733556, .longitude = -46.57264100}, .margem_gps = 8. / 1000., .margem_objetivo = 8. / 1000., .tem_cone = 1, .desvio = 0},
	{.pos = {.latitude = -23.64724922, .longitude = -46.57278980}, .margem_gps = 8. / 1000., .margem_objetivo = 8. / 1000., .tem_cone = 1, .desvio = 0},
	{.pos = {.latitude = -23.64701287, .longitude = -46.57263634}, .margem_gps = 8. / 1000., .margem_objetivo = 8. / 1000., .tem_cone = 1, .desvio = 0},

};

void espera_trigger();

int evento_atual;
static pixy_block_t object;

int color_follow() {
	led_set(7, LEDS2000MS, LEDS75PC, 5);
//	set_max_speed(50);
	const struct timespec block_wait_time = {.tv_sec = 0, .tv_nsec = 100 * MS};
	const struct timespec led_wait_time = {.tv_sec = 1, .tv_nsec = 500 * MS};
	int corretor = 0, n = 0;
	uint64_t sonar_max;

	if (nanosleep(&led_wait_time, NULL))
		perror("nanosleep");

	for (;;) {
		if (gps_distance(gps_get(), &eventos[evento_atual].pos) > eventos[evento_atual].margem_objetivo + 1. /  1000.) {
			return 0;
		}
		if (nanosleep(&block_wait_time, NULL))
			perror("nanosleep");

		pixy_cam_get(&object);
		printf("Objeto = x: %4hd y: %4hd w: %4hu h: %4hu a: %4hu\n", object.x, object.y,
			object.width, object.height, object.angle);

		if (sensor_contato < 4.) {
			const struct timespec chegou = {.tv_sec = 1, .tv_nsec = 100 * MS};
			led_set((1 << (evento_atual - 1)), LEDS2000MS, LEDS75PC, 5);
			motor(0, 0);
			nanosleep(&chegou, NULL);
			return 1;
		}

		if (object.x < 0) {
			corretor = MAX_Vel + object.x/2;
			motor(corretor, MAX_Vel);
			printf("motor(%d, %d)\n", corretor, MAX_Vel);
		} else if (object.x > 0) {
			corretor = MAX_Vel - object.x/2;
			motor(MAX_Vel, corretor);
			printf("motor(%d, %d)\n", MAX_Vel, corretor);
		}

		if (object.height == 0 && object.width == 0)// {
			//const struct timespec search_time = {.tv_sec = 0, .tv_nsec = 500 * MS};
			motor(50, 50);
		//	n++;
			//nanosleep(&search_time, NULL);
		//	printf("Procurado Cone...\n");
		//	if (n >= 30) {
//#warning Lembrar de colocar sonares
//				while (gps_distance(gps_get(), &eventos[evento_atual].pos) < 10. / 1000.) {
//					motor(60, 60);
//					if (sensor_sontato > 4)
//						return 1;
//				n = 0;
//				return 0;
//			}
//		}
	}
}

int main()
{
	thread_spawn(gps_thread);
	thread_spawn(motors_thread);
	thread_spawn(hmc5883l_thread);
#warning SONAR AQUI
	thread_spawn(sonar_thread);
	thread_spawn(proximity_thread);

	pixy_cam_init();
	gps_set(&eventos[0].pos, 1.);
	leds_init();
	led_set(0, LEDS2000MS, LEDS75PC, 0);
	for (evento_atual = 1; evento_atual < len(eventos); evento_atual++)
		executa_evento(evento_atual);
	printf("Terminado\n");
	motor(0, 0);
	return 0;
}

static void executa_evento(int evento_atual) {
	struct js_event joystick_event;
	int motor_l = 0, motor_r = 0, debug_helper = 0, debug_enabled = 1;
	//double direcao = azi4muth(&eventos[evento_atual - 1].pos, &eventos[evento_atual].pos);
	//pid_correction_t pid_direcao;

	printf("Evento %d\n", evento_atual);
	//pid_init(&pid_direcao, 5, 0, 2);

	for (;;) {
		//led_set(1, LEDS2000MS, LEDS75PC, 0);
		double correcao, correcao_anterior;
		//direcao += pid_update(&pid_direcao, compass_diff(azimuth(gps_get(), &eventos[evento_atual].pos), direcao));

		//correcao = compass_diff(direcao_atual, direcao);
		//printf("%.6lf %.6lf -> %.6lf %.6lf - dist %lf Alvo: %lf, atual: %lf\n", gps_get()->latitude, gps_get()->longitude, eventos[evento_atual].pos.latitude, eventos[evento_atual].pos.longitude, gps_distance(gps_get(), &eventos[evento_atual].pos), azimuth(gps_get(), &eventos[evento_atual].pos), direcao_atual);
		correcao = compass_diff(azimuth(gps_get(), &eventos[evento_atual].pos), direcao_atual);
		printf("%f %f -> %f %f\n", gps_get()->latitude, gps_get()->longitude,
		    eventos[evento_atual].pos.latitude, eventos[evento_atual].pos.longitude);
		printf("Azimuth: %f\n", azimuth(gps_get(), &eventos[evento_atual].pos));
		printf("Direção atual: %f\n", direcao_atual);
		printf("Diff %lf\n", correcao);

		if (correcao > ERRO_MAX) {
                printf("Girando para a direita\n");
                motor_l = VELOCIDADE_MAX;
                motor_r = 0;//-VELOCIDADE_MAX;
		} else if (correcao < -ERRO_MAX) {
                printf("Girando para a esquerda\n");
                motor_l = 0;//-VELOCIDADE_MAX;
                motor_r = VELOCIDADE_MAX;
        } else {
                printf("Seguindo reto\n");
                motor_l = VELOCIDADE_MAX + 10;
                motor_r = VELOCIDADE_MAX + 10;
        }
		motor(motor_l, motor_r);


// #define ERRO_MAX_ZIGZAG			(M_PI / 10)
// #define VELOCIDADE_MAX_ZIGZAG	50
// 		if (correcao > ERRO_MAX) {
// 			printf("Girando para a direita\n");
// 			motor(VELOCIDADE_MAX_ZIGZAG, VELOCIDADE_MAX_ZIGZAG - 10);
// 		} else if (correcao < -ERRO_MAX) {
// 			printf("Girando para a esquerda\n");
// 			motor(VELOCIDADE_MAX_ZIGZAG - 10, VELOCIDADE_MAX_ZIGZAG);
// 		}

//#define P	20.
//		if (correcao > 0)
//			motor(VELOCIDADE_MAX, VELOCIDADE_MAX - correcao * P);
//		else
//			motor(VELOCIDADE_MAX + correcao * P, VELOCIDADE_MAX);

        // faz zigue zague
        /*
        correcao_anterior = correcao;
        if (correcao > ERRO_MAX) {
            printf("Girando para a direita");
            motor_l = VELOCIDADE_MAX;
            motor_r = -VELOCIDADE_MAX;
        } else if (correcao < -ERRO_MAX){
            printf("Girando para a esquerda");
            motor_l = -VELOCIDADE_MAX;
            motor_r = VELOCIDADE_MAX;
        } else if (correcao < correcao_anterior){
            motor_l = FATOR*VELOCIDADE_MAX;
            motor_r = VELOCIDADE_MAX;
        } else if (correcao > correcao_anterior){
            motor_l = VELOCIDADE_MAX;
            motor_r = FATOR*VELOCIDADE_MAX;
        }
        */

		if (gps_distance(gps_get(), &eventos[evento_atual].pos) < eventos[evento_atual].margem_objetivo) {
			if (eventos[evento_atual].tem_cone) {
				printf("Cone proximo\n");
				if (color_follow()) {
					//led_set(0, 0, 0, 0);
					const struct timespec re_time = {.tv_sec = 1, .tv_nsec = 0 * MS};
					motor(-50,-50);
					if (nanosleep(&re_time, NULL))
						perror("nanosleep");
					return;
				}
				led_set(0, 0, 0, 0);
			}
		}
//#warning mudar pra minimo
//		uint64_t sonar_max = sonar_l/CONVERTE > sonar_r/CONVERTE ? sonar_l/CONVERTE : sonar_r/CONVERTE;
//		printf("Maior Distancia: %" PRIu64 "\n", sonar_max);
//		printf("Sonares: %llu, %llu\n", sonar_l / 58000, sonar_r / 58000);
		printf("Sensor Contato: %f\n", sensor_contato);
#warning SONAR AQUI
		if (sonar_l == 141414141414 && sonar_r == 141414141414) {
			evento_atual = 0;
			gps_set(&eventos[0].pos, 1.);
			led_set(0, LEDS2000MS, LEDS75PC, 0);
		}
//		if ((sonar_l < 50 * 58000 || sonar_r < 50 * 58000) && (sonar_r > 58000 && sonar_l > 58000)) {
//			motor(0,0);
//			return 0;
//		}
		printf("Distance: %f\n", gps_distance(gps_get(), &eventos[evento_atual].pos));

		printf("Motor %d %d\n", motor_l, motor_r);
//		pixy_cam_get(&object);
//
//		if (gps_distance(gps_get(), &eventos[0].pos) < 3. / 1000. && object.signature == 7 && object.height * object.width >= 7000) {
//			const struct timespec restart_time = {.tv_sec = 1, .tv_nsec = 0 * MS};
//			led_set(3, LEDS2000MS, LEDS75PC, 5);
//			if(nanosleep(&restart_time, NULL))
//				perror("restart time");
//			gps_set(&eventos[0].pos, 1.);
//			evento_atual = 0;
//			return;
//		}
		nanosleep(&cycle, NULL);
	}
}
