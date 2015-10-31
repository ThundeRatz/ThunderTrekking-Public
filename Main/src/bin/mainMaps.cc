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

using namespace std;
using namespace Trekking;

struct Evento {
	GPS pos;
	bool tem_cone;
	double angle;
	void executa();
	bool find_cone();
};

static Evento eventos[] = {
	{.pos = GPS(-0.3705665059, -0.7852447993), .tem_cone = false, .angle = 0}, // 0 0
	{.pos = GPS(-0.3705604371, -0.7852483900), .tem_cone = true, .angle = 0},  // -0.0213236069 0.0386643109
	{.pos = GPS(-0.3705616068, -0.7852457911), .tem_cone = true, .angle = 0},  // -0.0058898665 0.0312121650
	{.pos = GPS(-0.3705660952, -0.7852481772), .tem_cone = true, .angle = 0},  // -0.0200598360 0.0026165574
};

static Leds ledr("LedRed");
static Leds ledg("LedGreen");
static Leds ledb("LedBlue");

static Bumper bumper;
static BNO055 bno;
static LeddarEK leddar;
static Eigen::Rotation2D<double> heading;

static int cont = 0;
static double ant = 0.;

int main() {
	thread_spawn(motors_thread);

	for (int i = 0; i < len(eventos); i++)
		eventos[i].pos.to_2d(eventos[i].pos.point, eventos[0].pos);

	for (int i = 1; i < len(eventos); i++)
		eventos[i].angle = eventos[i - 1].pos.azimuth_to(eventos[i].pos);

	for (int i = 1; i < len(eventos); i++)
		eventos[i].executa();

	for (int i = 0; i < 20; i++) {
		ledr = i % 2;
		sleep_ms(50);
		ledb = i % 2;
		sleep_ms(50);
		ledg = i % 2;
		sleep_ms(50);
	}


	cout << "Terminado" << endl;

	motor(0, 0);

	return 0;
}

void Evento::executa() {
	int motor_l = 0, motor_r = 0;
	double correcao;

	cout << fixed << setprecision(8);
	cout << "Executando evento\n";

	for (;;) {
		leddar.update();
		bno.heading(heading);
		bumper.pressed();

		if (ant == leddar.measure.mDistance) cont++;
		else cont = 0;

		if (cont >= 20) {
			leddar.restart();
			cont = 0;
			ant = 0.;
		}

		ant = leddar.measure.mDistance;

		correcao = compass_diff(this->angle, heading.angle());
		cout << "Azimuth: " << this->angle << endl
			<< "Direcao Atual: " << heading.angle() << endl
			<< "Diff: " << correcao << endl;

		if (correcao > ERRO_MAX) {
                cout << "Girando para a direita\n";
                motor_l = VMAX ;
                motor_r = -(VMAX - (correcao * 100));//-VMAX;
		} else if (correcao < -ERRO_MAX) {
                cout << "Girando para a esquerda\n";
                motor_l = -(VMAX - (correcao * 100));//-VMAX;
                motor_r = VMAX;
        } else {
                cout << "Seguindo reto\n";
                motor_l = VMAX;
                motor_r = VMAX;
        }
		motor(motor_l, motor_r);

		if (leddar.measure.mDistance < 10) {
			cout << "Cone proximo\n";
			lebb = 1;
			sleep_ms(50);
			lebb = 0;
			if (find_cone()) {
				unsigned int re_time = 1500;
				motor(VMAX, VMAX);
				sleep_ms(re_time);
				motor(-VMAX, -VMAX);
				sleep_ms(re_time);
				return;
			}
			ledr = 0; ledg = 0; ledb = 0;
		}
	}
}

bool Evento::find_cone() {
	unsigned int block_wait_time = 100;
	unsigned int led_wait_time = 500;
	int corretor;

	sleep_ms(led_wait_time);

	for (;;) {
		sleep_ms(50);

		leddar.update();

		if (ant == leddar.measure.mDistance) cont++;
		else cont = 0;

		if (cont >= 20) {
			leddar.restart();
			cont = 0;
			ant = 0.;
		}

		ant = leddar.measure.mDistance;

		cout << "Menor Distancia EK: " << leddar.measure.mSegment << "|"
			<< leddar.measure.mDistance << endl << endl;

		if (bumper.pressed()) {
			ledr = 1; ledg = 1; ledb = 1;
			cout << "Found Cone!" << endl;
			motor(0, 0);
			sleep_ms(1500);
			ledr = 0; ledg = 0; ledb = 0;
			return true;
		}

		if (leddar.measure.mSegment < 6) {
			corretor = VMAX - 50;
			motor(corretor, VMAX);
		} else if (leddar.measure.mSegment > 8) {
			corretor = VMAX - 50;
			motor(VMAX, corretor);
		} else {
			motor(VMAX, VMAX);
		}
	}
	return false;
}
