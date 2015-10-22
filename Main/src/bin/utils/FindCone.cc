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

#include "ThreadMotors.hh"
#include "LeddarEK.hh"
#include "Pixy.hh"
#include "Leds.hh"

#define VELOCIDADE_MAX     50

using namespace Trekking;
using namespace std;

int main() {
	LeddarEK leddar;
	Pixy pixy

	Leds ledr("LedRed");
	Leds ledg("LedGreen");
	Leds ledb("LedBlue");

	unsigned int block_wait_time = 100;
	unsigned int led_wait_time = 500;
	int corretor;

	thread_spawn(motors_thread);

	sleep_ms(led_wait_time);

	for (;;) {
		sleep_ms(block_wait_time);

		leddar.update();
		pixy.update();

		cout << "Objeto = x: " << pixy.block.x << " y: " << pixy.block.y
			<< " w: " << pixy.block.width << " h: " << pixy.block.height
			<< " a: " << pixy.block.angle << endl;

		if (bumper.pressed()) {
			unsigned int chegou = 1000;
			ledr = 1; ledg = 1; ledb = 1;
			motor(0, 0);
			sleep_ms(chegou);
			return 0;
		}

		if (leddar.measure.mDistance < 3) {
			if (pixy.block.x < 0) {
				corretor = VELOCIDADE_MAX + pixy.block.x/2;
				motor(corretor, VELOCIDADE_MAX);
			} else if (pixy.block.x > 0) {
				corretor = VELOCIDADE_MAX - pixy.block.x/2;
				motor(VELOCIDADE_MAX, corretor);
			}
		} else if (leddar.measure.mDistance > 10) {
			motor(-50, 50);
		} else {
			if (leddar.measure.mSegment < 6) {
				corretor = VELOCIDADE_MAX - 10;
				motor(corretor, VELOCIDADE_MAX);
			} else if (leddar.measure.mSegment > 8) {
				corretor = VELOCIDADE_MAX - 10;
				motor(VELOCIDADE_MAX, corretor);
			} else {
				motor(VELOCIDADE_MAX, VELOCIDADE_MAX);
			}
		}

	}

	return -1;
}
