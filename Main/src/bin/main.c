#define COMPASS_P	5

#define JOYSTICK_NONBLOCK
#define ERRO_MAX M_PI/36 // 20º
#define VELOCIDADE_MAX 120

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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

#define len(array)	((&array)[1] - array)
#define MS	1000000

static void executa_evento(int evento_atual);
static int joystick, quadrado = 0;
static pixy_block_t pixy;
static const struct timespec cycle = {.tv_sec = 1, .tv_nsec = 10 * MS};

typedef struct {
	gps_coord_t pos;
	int margem_gps, tem_cone, desvio;
} evento;

evento eventos[] = {
	{.pos = {.latitude = 37.5473595, .longitude = -122.302314833333}},
	{.pos = {.latitude = 37.546799333333, .longitude = -122.302291833333}, .margem_gps = 10 / 1000, .tem_cone = 1, .desvio = 0},
	{.pos = {.latitude = 37.548270833333, .longitude = -122.302276666667}, .margem_gps = 10 / 1000, .tem_cone = 1, .desvio = 0},
	{.pos = {.latitude = 37.546314166667, .longitude = -122.302970833333}, .margem_gps = 10 / 1000, .tem_cone = 1, .desvio = 0},
	{.pos = {.latitude = 37.546550833333, .longitude = -122.3014755}, .margem_gps = 10 / 1000, .tem_cone = 1, .desvio = 0},
};

void espera_trigger();

int main() {
	int evento_atual;

	thread_spawn(gps_thread, NULL);
	thread_spawn(motors_thread, NULL);
	thread_spawn(hmc5883l_thread, NULL);
	thread_spawn(sonar_thread, NULL);

	gps_set(&eventos[0].pos, 1.);

	for (evento_atual = 1; evento_atual < len(eventos); evento_atual++)
		executa_evento(evento_atual);
	printf("Terminado\n");
	return 0;
}

static void executa_evento(int evento_atual) {
	struct js_event joystick_event;
	int motor_l = 0, motor_r = 0, debug_helper = 0, debug_enabled = 1;
	//double direcao = azimuth(&eventos[evento_atual - 1].pos, &eventos[evento_atual].pos);
	//pid_correction_t pid_direcao;

	printf("Evento %d\n", evento_atual);
	//pid_init(&pid_direcao, 5, 0, 2);

	for (;;) {
		double correcao;
		//direcao += pid_update(&pid_direcao, compass_diff(azimuth(gps_get(), &eventos[evento_atual].pos), direcao));

		//correcao = compass_diff(direcao_atual, direcao);
		//printf("%.6lf %.6lf -> %.6lf %.6lf - dist %lf Alvo: %lf, atual: %lf\n", gps_get()->latitude, gps_get()->longitude, eventos[evento_atual].pos.latitude, eventos[evento_atual].pos.longitude, gps_distance(gps_get(), &eventos[evento_atual].pos), azimuth(gps_get(), &eventos[evento_atual].pos), direcao_atual);
		correcao = compass_diff(azimuth(gps_get(), &eventos[evento_atual].pos), direcao_atual);
		printf("Diff %lf\n", correcao);
        
        
		if (correcao > ERRO_MAX) {
                printf("Girando para a direita");
                motor_l = VELOCIDADE_MAX;
                motor_r = -VELOCIDADE_MAX;
		} else if (correcao < -ERRO_MAX){
                printf("Girando para a esquerda");
                motor_l = -VELOCIDADE_MAX;
                motor_r = VELOCIDADE_MAX;
        } else{
                printf("Seguindo reto");
                motor_l = VELOCIDADE_MAX;
                motor_r = VELOCIDADE_MAX;
        }
        
        // tentar fazer zigue zague

		if (gps_distance(gps_get(), &eventos[evento_atual].pos) < eventos[evento_atual].margem_gps) {
			if (eventos[evento_atual].tem_cone) {
				printf("Sonares próximos - margem gps\n");
				if (sonar_r < 10 * 1000 * 58 || sonar_l < 10 * 1000 * 58) {
					const struct timespec sleep_hit = {.tv_sec = 0, .tv_nsec = 500 * MS};
					motor(150, 150);
					printf("Esperando atingir cone\n");
					nanosleep(&sleep_hit, NULL);
					return;
				}
			}
		}

		printf("Motor %d %d\n", motor_l, motor_r);
		nanosleep(&cycle, NULL);
	}
}
