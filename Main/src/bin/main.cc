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

#include "camera.hh"
#include "errno_string.hh"
#include "LedsI2C.hh"
#include "GPIOButton.hh"
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

#define VELOCIDADE_MAX 40
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
	{.pos = GPS(-0.3705665059, -0.7852447993), .margemGPS = 0, .margemObjetivo = 0, .tem_cone = false, .desvio = 0},
	{.pos = GPS(-0.3705602194, -0.7852497849), .margemGPS = 12./1000., .margemObjetivo = 12./1000., .tem_cone = true, .desvio = 0},
	{.pos = GPS(-0.3705618575, -0.7852466957), .margemGPS = 12./1000., .margemObjetivo = 12./1000., .tem_cone = true, .desvio = 0},
	{.pos = GPS(-0.3705658102, -0.785249658), .margemGPS = 12./1000., .margemObjetivo = 12./1000., .tem_cone = true, .desvio = 0},
};

static GPSMonitor gps_monitor(eventos[0].pos);
static BNO055 *bno055;
static Eigen::Rotation2D<double> heading;
static int evento;

int main() {
	GPIOButton reset(164);
	BNO055 bno055_instance;
	bno055 = &bno055_instance;
	thread_spawn(motors_thread);

	for (int i = 0; i < len(eventos); i++)
		eventos[i].pos.to_2d(eventos[i].pos.point, eventos[0].pos);

	while (!gps_monitor.update()) {
		cout << "gps_monitor.update() inicial\n";
		sleep_ms(2000);
	}

	for (;;) {
		while (reset) {
			LedsI2C led;
			sleep_ms(200);
		}
		for (evento = 1; evento < len(eventos); evento++)
			eventos[evento].executa();

		cout << "Terminado" << endl;

		motor(0, 0);
		sleep_ms(5000);
	}

	return 0;
}

void Evento::executa() {
	int motor_l = 0, motor_r = 0;
	double correcao;
	GPIOButton reset(164);
	GPS pos_atual = eventos[evento - 1].pos;
	pos_atual.to_2d(pos_atual.point, eventos[0].pos);

	cout << fixed << setprecision(8);
	cout << "Executando evento\n";

	while (!reset) {
		if (gps_monitor.update()) {
			pos_atual.latitude = gps_monitor.latitude;
			pos_atual.longitude = gps_monitor.longitude;
			pos_atual.to_2d(pos_atual.point, eventos[0].pos);
		}
		bno055->heading(heading);
		correcao = compass_diff(0.40594833 - 0.014 + pos_atual.azimuth_to(this->pos), heading.angle());
		cout << pos_atual.point << " -> " << this->pos.point << endl
			<< "Azimuth: " << pos_atual.azimuth_to(this->pos) << endl
			<< "Direcao Atual: " << heading.angle() << endl
			<< "Diff: " << correcao << endl
			<< "Dist: " << pos_atual.distance_to(this->pos) << endl;

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
                motor_l = VELOCIDADE_MAX + 10 + 8 * correcao;
                motor_r = VELOCIDADE_MAX + 10 - 8 * correcao;
	    }
		motor(motor_l, motor_r);

		if (this->tem_cone && pos_atual.distance_to(this->pos) < this->margemObjetivo) {
			cout << "Cone proximo\n";
			motor(0, 0);
			if (find_cone()) {
				motor(-100, -100);
				sleep_ms(800);
				return;
			}
		}
	}
}

bool Evento::find_cone() {
	Trekking::GPIOButton bumper_1(165), bumper_2(166);
	Camera camera;
	LedsI2C led;

	for (;;) {
		if (gps_monitor.update() && (gps_monitor.distance_to(this->pos) > this->margemObjetivo + 1./1000.))
			return false;

		if (bumper_1 || bumper_2) {
			led.red((evento == 1) * 255);
			led.green((evento == 2) * 255);
			led.blue((evento == 3) * 255);
			led.setMode(MANUAL);
			return true;
		}

		camera.showBiggest();
		camera.followContour(camera.getBiggest());
	}
}
