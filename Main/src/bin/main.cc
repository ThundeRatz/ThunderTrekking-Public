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

#include "errno_string.hh"
#include "ThreadMotors.hh"
#include "ThreadSpawn.hh"
#include "BNO055.hh"
#include "Bumper.hh"
#include "sleep.hh"
#include "Pixy.hh"
#include "Leds.hh"
#include "GPS.hh"
#include "PID.hh"

#include "compass.h"

#define len(array)     ((&array)[1] - array)

#define VELOCIDADE_MAX 50
#define ERRO_MAX       M_PI/10
#define MS	           1000000

using namespace std;
using namespace Trekking;

struct Evento {
	GPS pos;
	double margemGPS, margemObjetivo;
	bool tem_cone;
	int desvio;
	void executa();
	bool find_cone();
};

static Evento eventos[] = {
	{.pos = GPS(-0.4127174167, -0.8128488894), .margemGPS = 0, .margemObjetivo = 0, .tem_cone = false, .desvio = 0},
	{.pos = GPS(-0.4127238648, -0.8128459268), .margemGPS = 8./1000., .margemObjetivo = 8./1000., .tem_cone = true, .desvio = 0},
	{.pos = GPS(-0.4127223579, -0.8128485238), .margemGPS = 8./1000., .margemObjetivo = 8./1000., .tem_cone = true, .desvio = 0},
	{.pos = GPS(-0.4127182328, -0.8128458454), .margemGPS = 8./1000., .margemObjetivo = 8./1000., .tem_cone = true, .desvio = 0},
};

static GPS pos_atual(eventos[0].pos);
static GPSMonitor gps_monitor;
static BNO055 bno055;
static Eigen::Rotation2D<double> heading;

int main() {
	thread_spawn(motors_thread);

	for (int i = 0; i < len(eventos); i++)
		eventos[i].pos.to_2d(eventos[i].pos.point, eventos[0].pos);
	pos_atual = eventos[0].pos;

	for (int i = 1; i < len(eventos); i++)
		eventos[i].executa();

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
		if (gps_monitor.update()) {
			pos_atual.latitude = gps_monitor.latitude;
			pos_atual.longitude = gps_monitor.longitude;
			bno055.heading(heading);
			correcao = compass_diff(pos_atual.azimuth_to(this->pos), heading.angle());
			cout << pos_atual.point << " -> " << this->pos.point << endl
				<< "Azimuth: " << pos_atual.azimuth_to(this->pos) << endl
				<< "Direcao Atual: " << heading.angle() << endl
				<< "Diff: " << correcao << endl;

			if (correcao > ERRO_MAX) {
		                cout << "Girando para a direita\n";
		                motor_l = VELOCIDADE_MAX;
		                motor_r = 0;//-VELOCIDADE_MAX;
			} else if (correcao < -ERRO_MAX) {
		                cout << "Girando para a esquerda\n";
		                motor_l = 0;//-VELOCIDADE_MAX;
		                motor_r = VELOCIDADE_MAX;
		        } else {
		                cout << "Seguindo reto\n";
		                motor_l = VELOCIDADE_MAX + 10;
		                motor_r = VELOCIDADE_MAX + 10;
		        }
			motor(motor_l, motor_r);

			if (this->tem_cone && pos_atual.distance_to(this->pos) < this->margemObjetivo) {
				cout << "Cone proximo\n";
				if (find_cone()) {
					unsigned int re_time = 1000;
					motor(-50, -50);
					sleep_ms(re_time);
					return;
				}
				ledr = 0; ledg = 0; ledb = 0;
			}
		} else {
			unsigned int sleep_time = 1000;
			cerr << "blocking_update error" << endl;
			sleep_ms(sleep_time);
		}
	}
}

bool Evento::find_cone() {
	unsigned int block_wait_time = 100;
	unsigned int led_wait_time = 500;
	int corretor;

	sleep_ms(led_wait_time);

	for (;;) {
		if (pos_atual.update()) {
			if (pos_atual.distance_to(this->pos) > this->margemObjetivo + 1./1000.)
				return false;
			sleep_ms(block_wait_time);

			cone.update();
			cout << "Objeto = x: " << cone.block.x << " y: " << cone.block.y
				<< " w: " << cone.block.width << " h: " << cone.block.height
				<< " a: " << cone.block.angle << endl;

			if (bumper.pressed()) {
				unsigned int chegou = 1000;
				ledr = 1; ledg = 1; ledb = 1;
				motor(0, 0);
				sleep_ms(chegou);
				return true;
			}

			if (cone.block.x < 0) {
				corretor = VELOCIDADE_MAX + cone.block.x/2;
				motor(corretor, VELOCIDADE_MAX);
			} else if (cone.block.x > 0) {
				corretor = VELOCIDADE_MAX - cone.block.x/2;
				motor(VELOCIDADE_MAX, corretor);
			}

			if (cone.block.height == 0 && cone.block.width == 0)
				motor(50, 50);
		} else {
			unsigned int sleep_time = 1000;
			cerr << "blocking_update error" << endl;
			sleep_ms(sleep_time);
		}
	}
	return false;
}
