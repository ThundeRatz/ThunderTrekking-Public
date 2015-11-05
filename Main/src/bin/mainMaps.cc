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

#include <eigen3/Eigen/Dense>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>

#include "ThreadMotors.hh"
#include "ThreadSpawn.hh"
#include "LeddarEK.hh"
#include "BNO055.hh"
#include "Bumper.hh"
#include "sleep.hh"
#include "Leds.hh"
#include "GPS.hh"

#include "compass.h"

#define len(array)     ((&array)[1] - array)

#define VMAX           255
#define ERRO_MAX       M_PI/20

#define BUMPERR  49 //codigo
#define BUMPERL  50 //codigo
#define RESET    51 //codigo

using namespace std;
using namespace Trekking;

struct Evento {
	GPS pos;
	bool tem_cone;
	void executa();
	int find_cone();
};

static Evento eventos[] = {
	{.pos = GPS(-0.3705664791, -0.7852462611), .tem_cone = false}, // 0 0
	{.pos = GPS(-0.3705599906, -0.7852493748), .tem_cone = true},  // -0.0213236069 0.0386643109
	{.pos = GPS(-0.3705620335, -0.7852462032), .tem_cone = true},  // -0.0058898665 0.0312121650
	{.pos = GPS(-0.3705658885, -0.7852487568), .tem_cone = true},  // -0.0200598360 0.0026165574
};

static Leds ledr("LedRed");
static Leds ledg("LedGreen");
static Leds ledb("LedBlue");

static Bumper bumper;
static BNO055* bno;
static LeddarEK leddar;
static Eigen::Rotation2D<double> heading;
static GPSMonitor gps(eventos[0].pos);

static int cont = 0;
static double ant = 0.;
static int ev_atual = 1;
static int bumper_code = 0;
static int reinicio = 0;

int main() {
	BNO055 bno055_instance;
	bno = &bno055_instance;
	thread_spawn(motors_thread);

	for(;;) {
		while(!bumper.pressed());
		ev_atual = 1;

		ledr = 1;
		sleep_ms(50);
		ledb = 1;
		sleep_ms(50);
		ledg = 1;
		sleep_ms(50);

		ledr = 0; ledb = 0; ledg = 0;

		for (int i = 0; i < len(eventos); i++)
			eventos[i].pos.to_2d(eventos[i].pos.point, eventos[0].pos);

		for (; ev_atual < len(eventos); )
			eventos[ev_atual].executa();

		motor(0, 0);

		for (int i = 0; i < 101; i++) {
			ledr = i % 2;
			sleep_ms(50);
			ledb = i % 2;
			sleep_ms(50);
			ledg = i % 2;
			sleep_ms(50);
		}
	}
	cout << "Terminado" << endl;

	motor(0, 0);

	return 0;
}

void Evento::executa() {
	int motor_l = 0, motor_r = 0, c = 0;
	double correcao;

	cout << fixed << setprecision(8);
	cout << "Executando evento\n";

	sleep_ms(1000);

	for (;;) {
		sleep_ms(50);
		c++;

		// leddar.update();
		bno->heading(heading);
		gps.blocking_update();

		// if ((bumper_code = bumper.pressed()) == RESET) {
		if (bumper.pressed())
			reinicio++;

		if (reinicio >= 20) {
			cout << "Reiniciando...\n";
			sleep_ms(10);
			ledr = 1;
			sleep_ms(1000);
			ledr = 0;
			reinicio = 0;
			ev_atual = 1;
			return;
		}

		// if (ant == leddar.measure.mDistance) cont++;
		// else cont = 0;
		//
		// if (cont >= 50) {
		// 	leddar.restart();
		// 	cont = 0;
		// 	ant = 0.;
		// }
		//
		// ant = leddar.measure.mDistance;

		correcao = compass_diff(gps.azimuth_to(pos), heading.angle());
		cout << gps.point << " -> " << pos.point << endl
			<< "Azimuth: " << gps.azimuth_to(pos) << endl
			<< "Direcao Atual: " << heading.angle() << endl
			<< "Diff: " << correcao << endl
			<< "Menor Distancia EK: " << leddar.measure.mSegment << "|"
			<< leddar.measure.mDistance << endl << endl;

		if (correcao > ERRO_MAX) {
                cout << "Girando para a direita\n";
                motor_l = VMAX;
                motor_r = -(VMAX - abs((correcao * 75)));//-VMAX;
		} else if (correcao < -ERRO_MAX) {
                cout << "Girando para a esquerda\n";
                motor_l = -(VMAX - abs((correcao * 75)));//-VMAX;
                motor_r = VMAX;
        } else {
                cout << "Seguindo reto\n";
                motor_l = VMAX;
                motor_r = VMAX;
        }
		motor(motor_l, motor_r);

		if (this->tem_cone && gps.distance_to(this->pos) < 4./1000.) {
			cout << "Cone proximo\n";
			int f = find_cone();

			//if (f == -1) return;
			if (f == 1) {
				motor(VMAX, VMAX);
				sleep_ms(1500);
				motor(-VMAX, -VMAX);
				sleep_ms(500);
				return;
			}
		}

		// if (leddar.nMeasures > 0 && leddar.measure.mDistance < 4 && leddar.measure.mDistance > 0 && c >= 100) {
		// 	cout << "Cone proximo\n";
		// 	ledb = 1;
		// 	sleep_ms(150);
		// 	ledb = 0;
		// 	if (find_cone()) {
		// 		motor(VMAX, VMAX);
		// 		sleep_ms(1500);
		// 		motor(-VMAX, -VMAX);
		// 		sleep_ms(1500);
		// 		return;
		// 	}
		// 	ledr = 0; ledg = 0; ledb = 0;
		// }
	}
}

int Evento::find_cone() {
	unsigned int led_wait_time = 500;
	int corretor;

	ledb = 1;
	sleep_ms(led_wait_time);
	ledb = 0;

	for (;;) {
		sleep_ms(50);

		leddar.update();
		// bumper.pressed();

		if (ant == leddar.measure.mDistance) cont++;
		else cont = 0;

		if (cont >= 50) {
			ledg = 1;
			leddar.restart();
			ledg = 0;
			cont = 0;
			ant = 0.;
		}

		ant = leddar.measure.mDistance;

		if  (leddar.nMeasures == 0 || leddar.measure.mDistance > 8) {
			ledr = 1; ledb = 1;
			cout << "Procurando cone...\n";
			motor(230, -230);
			sleep_ms(50);
			ledr = 0; ledb = 0;
		}

		cout << "Menor Distancia EK: " << leddar.measure.mSegment << "|"
			<< leddar.measure.mDistance << endl << endl;

		// if (bumper_code == BUMPERL || bumper_code == BUMPERR) {
		if (bumper.pressed()) {
			ledr = 1; ledg = 1; ledb = 1;
			cout << "Found Cone!" << endl;
			motor(0, 0);
			sleep_ms(2000);
			ledr = 0; ledg = 0; ledb = 0;
			reinicio = 0;
			ev_atual++;
			return 1;
		}
		// } else if (bumper_code == RESET){
		// 	cout << "Reiniciando...\n";
		// 	sleep_ms(10);
		// 	ledr = 1;
		// 	sleep_ms(1000);
		// 	ledr = 0;
		// 	ev_atual = 1;
		// 	return -1;
		// }

		if (leddar.measure.mDistance <= 8 && leddar.measure.mSegment < 6) {
			corretor = VMAX - 100;
			motor(corretor, VMAX);
		} else if (leddar.measure.mDistance <= 8 && leddar.measure.mSegment > 8) {
			corretor = VMAX - 100;
			motor(VMAX, corretor);
		} else if (leddar.measure.mDistance <= 8 && (leddar.measure.mSegment >= 6 && leddar.measure.mSegment <= 8)) {
			motor(VMAX, VMAX);
		}
	}
	return 0;
}
