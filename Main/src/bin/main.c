#define COMPASS_P	5

#define DEBUG_DEADZONE		1500

#define JOYSTICK_NONBLOCK

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
	
	if (file_lock("/tmp/trekking") == -1) {
		printf("file_lock falhou\n");
		exit(-1);
	}
	
	thread_spawn(gps_thread, NULL);
	thread_spawn(motors_thread, NULL);
	thread_spawn(hmc5883l_thread, NULL);
	thread_spawn(sonar_thread, NULL);
	
	pixy_cam_init();
	
	while ((joystick = joystick_open_nb("/dev/input/js0")) == -1) {
		const struct timespec sleep_joystick = {.tv_sec = 0, .tv_nsec = 500 * MS};
		printf("Joystick não conectado\n");
		if (nanosleep(&sleep_joystick, NULL) == -1)
			perror("nanosleep");
	}
	
	joystick_dump(joystick);
	
	printf("Esperado botão 14\n");
	espera_trigger();
	
	gps_set(&eventos[0].pos);
	
	for (evento_atual = 1; evento_atual < len(eventos); evento_atual++)
		executa_evento(evento_atual);
	int motor_l = 0, motor_r = 0;
	int debug_helper = 0;
	for (;;) {
		struct js_event joystick_event;
		int joystick_has_packets = 1;
		while (joystick_has_packets)
			switch (joystick_read_nb(joystick, &joystick_event)) {
				case -1:
				printf("joystick_read_nb - erro de leitura, parando\n");
				motor_l = motor_r = 0;
				debug_helper = 0;
				joystick_has_packets = 0;
				break;
				
				case 1:
				if (joystick_event.type  & JS_EVENT_BUTTON) {
					if (joystick_event.number == 14 && !joystick_event.value) {
						printf("Pausado\n");
						motor(0, 0);
						espera_trigger();
						printf("Retomando\n");
					}
					if (joystick_event.number == 9 && !joystick_event.value) {
						printf("Terminado\n");
						motor(0, 0);
						exit(0);
					}
				}
				
				if (joystick_event.type  & JS_EVENT_AXIS) {
					if (joystick_event.number == 18) {
						if (joystick_event.value > DEBUG_DEADZONE)
							debug_helper = (joystick_event.value - DEBUG_DEADZONE) / 80;
						else if (joystick_event.value < -DEBUG_DEADZONE)
							debug_helper = (joystick_event.value + DEBUG_DEADZONE) / 80;
						else
							debug_helper = 0;
					}
				}
				break;
				
				case 0:
				joystick_has_packets = 0;
				break;
			}
		
		if (debug_helper) {
			if (debug_helper > 0) {
				motor_l = 255;
				motor_r = 255 - debug_helper;
			} else {
				motor_r = 255;
				motor_l = 255 + debug_helper;
			}
			printf("Motor %d %d (DBG)\n", motor_l, motor_r);
		} else {
			motor_l = 180; motor_r = 180;
			printf("Motor %d %d\n", motor_l, motor_r);
		}
		
		motor(motor_l, motor_r);
		nanosleep(&cycle, NULL);
	}
	printf("Terminado\n");
	return 0;
}

void espera_trigger() {
	for (;;) {
		struct js_event joystick_event;
		const struct timespec joystick_sleep_time = {.tv_sec = 0, .tv_nsec = 10 * MS};
		while (joystick_read_nb(joystick, &joystick_event) == 1) {
			if ((joystick_event.type  & JS_EVENT_BUTTON) && !(joystick_event.type & JS_EVENT_INIT) &&
				joystick_event.number == 14 && joystick_event.type)
				return;
			if ((joystick_event.type  & JS_EVENT_BUTTON) && !(joystick_event.type & JS_EVENT_INIT) &&
				joystick_event.number == 9 && joystick_event.type) {
				printf("Terminado\n");
				exit(0);
			}
		}
		if (nanosleep(&joystick_sleep_time, NULL) == -1)
			perror("nanosleep");
	}
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
		if (correcao > 0) {
			/*if (correcao > M_PI / 2) {
				printf("Erro grande, girando direita\n");
				motor_l = 150;
				motor_r = -100;
			} else {*/
				printf("Compass PID: dir %lf\n", 255 - COMPASS_P * correcao);
				motor_l = 200;
				motor_r = 200 - COMPASS_P * correcao;
			//}
		} else {
			/*if (correcao < -M_PI / 2) {
				printf("Erro grande, girando esquerda\n");
				motor_l = -100;
				motor_r = 150;
			} else {*/
				printf("Compass PID: esq %lf\n", 255 + COMPASS_P * correcao);
				motor_l = 200 + COMPASS_P * correcao;
				motor_r = 200;
			//}
		}
		
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
		} else {
			/*if (eventos[evento_atual].desvio == 0) { // esquerda
				if (sonar_l < 50 * 1000 * 58 || sonar_r < 30 * 1000 * 58) {
					printf("Sonares próximos - desvio esquerda\n");
					motor_l = -180;
					motor_r = 200;
				}
			} else {
				if (sonar_r < 50 * 1000 * 58 || sonar_l < 30 * 1000 * 58) {
					printf("Sonares próximos - desvio direita\n");
					motor_l = 200;
					motor_r = -180;
				}
			}*/
		}
		
		if (gps_distance(gps_get(), &eventos[evento_atual].pos) < eventos[evento_atual].margem_gps) {
			if (!eventos[evento_atual].tem_cone) {
				printf("Checkpoint!\n");
				return;
			}
			pixy_cam_get(&pixy);
			if (pixy.height * pixy.width > 100) {
				int delta = pixy.x - 180;
				printf("PIXY delta %d\n", delta);
				if (delta > 0) {
					motor_l = 200;
					motor_r = 200 - delta;
				} else {
					motor_l = 200 + delta;
					motor_r = 200;
				}
			}
		}
		
		int joystick_has_packets = 1;
		while (joystick_has_packets)
			switch (joystick_read_nb(joystick, &joystick_event)) {
				case -1:
				printf("joystick_read_nb - erro de leitura, parando\n");
				motor_l = motor_r = 0;
				debug_helper = 0;
				joystick_has_packets = 0;
				break;
				
				case 1:
				if (joystick_event.type  & JS_EVENT_BUTTON) {
					if (joystick_event.number == 14 && !joystick_event.value) {
						printf("Pausado\n");
						motor(0, 0);
						espera_trigger();
						printf("Retomando\n");
					}
					if (joystick_event.number == 9 && !joystick_event.value) {
						printf("Terminado\n");
						motor(0, 0);
						exit(0);
					}
					if (joystick_event.number == 0 && !joystick_event.value) {
						printf("debug switch\n");
						debug_enabled = !debug_enabled;
					}
				}
				
				if (joystick_event.type  & JS_EVENT_AXIS) {
					if (joystick_event.number == 19) {
						if (joystick_event.value > -30000)
							quadrado = 1;
						else
							quadrado = 0;
					}
				}
				break;
				
				case 0:
				joystick_has_packets = 0;
				break;
			}
		
		printf("Motor %d %d\n", motor_l, motor_r);
		
		if (motor_l > motor_r)
			motor(245, 100);
		else
			motor(150, 245);
		nanosleep(&cycle, NULL);
	}
}
