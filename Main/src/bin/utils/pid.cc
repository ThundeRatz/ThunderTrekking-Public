#define COMPASS_P	5

#define JOYSTICK_NONBLOCK
#define ERRO_MAX M_PI/10 // 20º
//#define VELOCIDADE_MAX 50
#define FATOR 2/3

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>

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
#include "thread_proximity.h"
#include "leds.h"
#include "PID.hh"


#define MAX_Vel        50
#define VELOCIDADE_MAX 40
#define CONVERTE    58000


#define len(array)	((&array)[1] - array)
#define MS	1000000

using namespace Trekking;


int main() {

	double objetivo;

	PID pid(10., 0., 0.);


	thread_spawn(motors_thread, NULL);


	thread_spawn(hmc5883l_thread, NULL);

	scanf("%lf", &objetivo);


	for (;;) {
		double correcao;
		double saida_PID;
		correcao = compass_diff(objetivo, direcao_atual);
		//printf("Direção atual: %lf\n", direcao_atual);
		if((saida_PID = pid.update(correcao)) > 0)
		{
			motor(VELOCIDADE_MAX - saida_PID, VELOCIDADE_MAX);
		}
		else
		{
			motor(VELOCIDADE_MAX, VELOCIDADE_MAX + saida_PID);
		}
		printf("Diff %lf\tPID - %lf\n", correcao, pid.update(correcao));
	}
	return 0;
}
