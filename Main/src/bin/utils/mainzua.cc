#include <iostream>

#include "LeddarEK.hh"
#include "Bumper.hh"
#include "ThreadMotors.hh"
#include "ThreadSpawn.hh"
#include "sleep.hh"
#include "Leds.hh"

#define VMAX 245

using namespace std;
using namespace Trekking;

int main() {
	thread_spawn(motors_thread);

	Bumper bumper;
	LeddarEK leddar;

	Leds ledr("LedRed");
	Leds ledg("LedGreen");
	Leds ledb("LedBlue");

	int corretor, cont = 0;
	double ant = 0.;

	// for (;;) {
		// Cone 1;
		leddar.update();
		while (leddar.nMeasures == 0) {
			leddar.update();

			if (ant == leddar.measure.mDistance) cont++;
			else cont = 0;

			if (cont >= 15) {
				leddar.restart();
				cont = 0;
				ant = 0.;
			}

			ant = leddar.measure.mDistance;

			motor(VMAX, VMAX);
			sleep_ms(100);
		}

		for (;;) {
			sleep_ms(50);
			leddar.update();

			if (ant == leddar.measure.mDistance) cont++;
			else cont = 0;

			if (cont >= 15) {
				leddar.restart();
				cont = 0;
				ant = 0.;
			}

			ant = leddar.measure.mDistance;

			if (bumper.pressed()) {
				ledr = 1; ledg = 1; ledb = 1;
				cout << "Found Cone!" << endl;
				motor(0, 0);
				sleep_ms(2500);
				ledr = 0; ledg = 0; ledb = 0;
				bumper.pressed();
				bumper.pressed();
				break;
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

		motor(-VMAX, -VMAX);
		sleep_ms(1000);

		motor(VMAX, -VMAX);
		sleep_ms(1500);

		leddar.update();
		while (leddar.nMeasures == 0 || leddar.measure.mDistance > 20) {
			leddar.update();

			if (ant == leddar.measure.mDistance) cont++;
			else cont = 0;

			if (cont >= 15) {
				leddar.restart();
				cont = 0;
				ant = 0.;
			}

			ant = leddar.measure.mDistance;

			motor(VMAX, -VMAX);
			sleep_ms(100);
		}

		for (;;) {
			sleep_ms(50);
			leddar.update();

			if (ant == leddar.measure.mDistance) cont++;
			else cont = 0;

			if (cont >= 15) {
				leddar.restart();
				cont = 0;
				ant = 0.;
			}

			if (bumper.pressed()) {
				ledr = 1; ledg = 1; ledb = 1;
				cout << "Found Cone!" << endl;
				motor(0, 0);
				sleep_ms(2500);
				ledr = 0; ledg = 0; ledb = 0;
				bumper.pressed();
				bumper.pressed();
				break;
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

		motor(-VMAX, -VMAX);
		sleep_ms(1000);

		motor(VMAX, -VMAX);
		sleep_ms(1500);
	// }
}
