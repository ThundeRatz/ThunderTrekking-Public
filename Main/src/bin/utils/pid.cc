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
#include "PID.hh"

#include "compass.h"
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
