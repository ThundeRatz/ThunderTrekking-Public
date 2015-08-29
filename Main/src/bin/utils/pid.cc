#define COMPASS_P	5

#define JOYSTICK_NONBLOCK
#define ERRO_MAX M_PI/10 // 20º
//#define VELOCIDADE_MAX 50
#define FATOR 2/3

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <inttypes.h>

#include "ThreadProximity.hh"
#include "ThreadHmc5883l.hh"
#include "ThreadMotors.hh"
#include "ThreadSpawn.hh"
#include "ThreadSonar.hh"
#include "ThreadGPS.hh"
#include "PID.hh"

#include "file_lock.h"
#include "joystick.h"
#include "compass.h"
#include "leds.h"
#include "pid.h"


#define MAX_Vel        50
#define VELOCIDADE_MAX 40
#define CONVERTE    58000


#define len(array)	((&array)[1] - array)
#define MS	1000000

using namespace Trekking;


int main() {

	double objetivo;

	PID pid(10., 0., 0.);


	thread_spawn(motors_thread);


	thread_spawn(hmc5883l_thread);

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
