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
	{.pos = GPS(-0.3705664273, -0.7852473030), .tem_cone = false, .angle = 0}, // 0 0
	{.pos = GPS(-0.3705602814, -0.7852492754), .tem_cone = true, .angle = 0},  // -0.0213236069 0.0386643109
	{.pos = GPS(-0.3705620804, -0.7852460117), .tem_cone = true, .angle = 0},  // -0.0058898665 0.0312121650
	{.pos = GPS(-0.3705660099, -0.7852496494), .tem_cone = true, .angle = 0},  // -0.0200598360 0.0026165574
};

static Leds ledr("LedRed");
static Leds ledg("LedGreen");
static Leds ledb("LedBlue");

static Bumper bumper;
static BNO055* bno;
static LeddarEK leddar;
static Eigen::Rotation2D<double> heading;

static int cont = 0;
static double ant = 0.;
static int ev_atual = 1;
static int reinicio = 0;

int main() {
	BNO055 bno055_instance;
	bno = &bno055_instance;
	thread_spawn(motors_thread);

	while(!bumper.pressed());

	ledr = 1;
	sleep_ms(50);
	ledb = 1;
	sleep_ms(50);
	ledg = 1;
	sleep_ms(50);

	ledr = 0; ledb = 0; ledg = 0;

	for (int i = 0; i < len(eventos); i++)
		eventos[i].pos.to_2d(eventos[i].pos.point, eventos[0].pos);

	for (int i = 1; i < len(eventos); i++)
		eventos[i].angle = eventos[i - 1].pos.azimuth_to(eventos[i].pos);

	for (; ev_atual < len(eventos);)
		eventos[ev_atual].executa();

	motor(0, 0);

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
	int motor_l = 0, motor_r = 0, c = 0;
	double correcao;

	cout << fixed << setprecision(8);
	cout << "Executando evento\n";

	sleep_ms(1000);

	for (;;) {
		sleep_ms(50);
		c++;

		leddar.update();
		bno->heading(heading);
		if (bumper.pressed()) {
			reinicio++;
		}

		if (reinicio >= 10) {
			cout << "Reiniciando...\n";
			sleep_ms(10);
			ledr = 1;
			sleep_ms(1000);
			ledr = 0;
			reinicio = 0;
			ev_atual = 1;
			return;
		}

		if (ant == leddar.measure.mDistance) cont++;
		else cont = 0;

		if (cont >= 50) {
			leddar.restart();
			cont = 0;
			ant = 0.;
		}

		ant = leddar.measure.mDistance;

		correcao = compass_diff(this->angle, heading.angle());
		cout << "Azimuth: " << this->angle << endl
			<< "Direcao Atual: " << heading.angle() << endl
			<< "Diff: " << correcao << endl
			<< "Menor Distancia EK: " << leddar.measure.mSegment << "|"
			<< leddar.measure.mDistance << endl << endl;

		if (correcao > ERRO_MAX) {
                cout << "Girando para a direita\n";
                motor_l = VMAX ;
                motor_r = -(VMAX - abs((correcao * 50)));//-VMAX;
		} else if (correcao < -ERRO_MAX) {
                cout << "Girando para a esquerda\n";
                motor_l = -(VMAX - abs((correcao * 50)));//-VMAX;
                motor_r = VMAX;
        } else {
                cout << "Seguindo reto\n";
                motor_l = VMAX;
                motor_r = VMAX;
        }
		motor(motor_l, motor_r);

		if (leddar.nMeasures > 0 && leddar.measure.mDistance < 4 && leddar.measure.mDistance > 0 && c >= 100) {
			cout << "Cone proximo\n";
			ledb = 1;
			sleep_ms(150);
			ledb = 0;
			if (find_cone()) {
				motor(VMAX, VMAX);
				sleep_ms(1500);
				motor(-VMAX, -VMAX);
				sleep_ms(1500);
				return;
			}
			ledr = 0; ledg = 0; ledb = 0;
		}
	}
}

bool Evento::find_cone() {
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

		if (leddar.measure.mDistance > 5) {
			cout << "Alarme falso! Retornando\n";
			return false;
		}

		cout << "Menor Distancia EK: " << leddar.measure.mSegment << "|"
			<< leddar.measure.mDistance << endl << endl;

		if (bumper.pressed()) {
			ledr = 1; ledg = 1; ledb = 1;
			cout << "Found Cone!" << endl;
			motor(0, 0);
			sleep_ms(2000);
			ledr = 0; ledg = 0; ledb = 0;
			ev_atual++;
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
