#define COMPASS_P	5

#define JOYSTICK_NONBLOCK
#define ERRO_MAX M_PI/10 // 20º
#define VELOCIDADE_MAX 50
#define FATOR 2/3

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include <pixy.h>

#include "gps_coord.h"
#include "thread_spawn.h"
#include "thread_hmc5883l.h"
#include "thread_sonar.h"
#include "thread_gps.h"
#include "thread_motors.h"
#include "pid.h"
#include "compass.h"
#include "file_lock.h"
#include "joystick.h"
#include "pixy_cam.h"
#include "leds.h"

#define MAX_Vel     50
#define CONVERTE    58000

#define len(array)	((&array)[1] - array)
#define MS	1000000

static void executa_evento(int evento_atual);
static int joystick, quadrado = 0;
static pixy_block_t pixy;
static const struct timespec cycle = {.tv_sec = 0, .tv_nsec = 10 * MS};

typedef struct {
	gps_coord_t pos;
	double margem_gps;
	int tem_cone, desvio;
} evento;

evento eventos[] = {
	{.pos = {.latitude = -23.64673319, .longitude = -46.57228128}},
	{.pos = {.latitude = -23.64682453, .longitude = -46.57256780}, .margem_gps = 4. / 1000., .tem_cone = 1, .desvio = 0},
	// Competição
	/*
	{.pos = {.latitude = -23.64698718, .longitude = -46.57280886}},
	{.pos = {.latitude = -23.64735580, .longitude = -46.57260658}, .margem_gps = 10. / 1000., .tem_cone = 1, .desvio = 0},
	{.pos = {.latitude = -23.64723972, .longitude = -46.57278148}, .margem_gps = 10. / 1000., .tem_cone = 1, .desvio = 0},
	{.pos = {.latitude = -23.64702190, .longitude = -46.57262284}, .margem_gps = 10. / 1000., .tem_cone = 1, .desvio = 0},
	*/
};

void espera_trigger();

int evento_atual;

int color_follow() {
//	led_set(1, LEDS2000MS, LEDS75PC, 0);
//	set_max_speed(50);

	const struct timespec block_wait_time = {.tv_sec = 0, .tv_nsec = 100 * MS};
	int corretor = 0;
	uint64_t sonar_min;
	
	pixy_led_set_RGB(0, 255, 0);
	
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

		sonar_min = sonar_l / CONVERTE;//sonar_l/CONVERTE < sonar_r/CONVERTE ? sonar_l/CONVERTE : sonar_r/CONVERTE;
		printf("Menor Distancia: %" PRIu64 "\n", sonar_min);

		if (sonar_min < 40 && sonar_min > 1) {
			const struct timespec chegou = {.tv_sec = 1, .tv_nsec = 100 * MS};
			led_set(1, LEDS2000MS, LEDS75PC, 5);
			pixy_led_set_RGB(255, 255, 255);
			motor(0, 0);
			nanosleep(&chegou, NULL);
			return 1;
		}

		if (gps_distance(gps_get(), &eventos[evento_atual].pos) > 8. / 1000.) {
			pixy_led_set_RGB(0, 0, 255);
			return 0;
		}

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
			motor(30, 30);
			printf("motor(0, 0)\n");
		}
	}

	return 0;
}

int main() {

	thread_spawn(gps_thread, NULL);
	thread_spawn(motors_thread, NULL);
	thread_spawn(hmc5883l_thread, NULL);
	thread_spawn(sonar_thread, NULL);

	pixy_cam_init();
	pixy_led_set_max_current(200000);
	gps_set(&eventos[0].pos, 1.);
	leds_init();
	led_set(0, LEDS2000MS, LEDS75PC, 0);
	pixy_led_set_RGB(0, 0, 0);
	for (evento_atual = 1; evento_atual < len(eventos); evento_atual++)
		executa_evento(evento_atual);
	printf("Terminado\n");
	return 0;
}

static void executa_evento(int evento_atual) {
	pixy_led_set_RGB(255, 0, 0);
	struct js_event joystick_event;
	int motor_l = 0, motor_r = 0, debug_helper = 0, debug_enabled = 1;
	//double direcao = azimuth(&eventos[evento_atual - 1].pos, &eventos[evento_atual].pos);
	//pid_correction_t pid_direcao;

	printf("Evento %d\n", evento_atual);
	//pid_init(&pid_direcao, 5, 0, 2);

	for (;;) {
//		led_set(0, LEDS2000MS, LEDS75PC, 0);
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
                motor_r = -VELOCIDADE_MAX;
		} else if (correcao < -ERRO_MAX){
                printf("Girando para a esquerda\n");
                motor_l = -VELOCIDADE_MAX;
                motor_r = VELOCIDADE_MAX;
        } else {
                printf("Seguindo reto\n");
                motor_l = VELOCIDADE_MAX;
                motor_r = VELOCIDADE_MAX;
        }
		motor(motor_l, motor_r);

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

		if (gps_distance(gps_get(), &eventos[evento_atual].pos) < eventos[evento_atual].margem_gps) {
			if (eventos[evento_atual].tem_cone) {
				printf("Sonares próximos - margem gps\n");
				if (color_follow())
					return;
			}
		}
		printf("Sonares: %llu, %llu\n", sonar_l / 58000, sonar_r / 58000);
//		if ((sonar_l < 50 * 58000 || sonar_r < 50 * 58000) && (sonar_r > 58000 && sonar_l > 58000)) {
//			motor(0,0);
//			return 0;
//		}
		printf("Distance: %f\n", gps_distance(gps_get(), &eventos[evento_atual].pos));

		printf("Motor %d %d\n", motor_l, motor_r);
		nanosleep(&cycle, NULL);
	}
}
