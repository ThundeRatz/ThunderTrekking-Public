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
#include <stdexcept>
#include <cmath>
#include <ctime>

#include <ncurses.h>

#include "ThreadMotors.hh"
#include "ThreadSpawn.hh"
#include "LeddarEK.hh"
#include "LedsI2C.hh"
#include "BNO055.hh"
#include "Bumper.hh"
#include "sleep.hh"
#include "Leds.hh"
#include "Pixy.hh"
#include "GPS.hh"

using namespace Trekking;

static WINDOW* gps_s;
static WINDOW* bno_s;
static WINDOW* pixy_s;
static WINDOW* leds_s;
static WINDOW* bumper_s;
static WINDOW* leddar_s;

static int left = 0, right = 0;

void GPSScreen(GPSMonitor& gps) {
	wclear(gps_s);
	if(!gps.blocking_update())
		mvwprintw(gps_s, LINES/2 - 6 - 5, 1, "blocking_update error");

	box(gps_s, 0 , 0);
	mvwprintw(gps_s, 0, (COLS/3 - 8)/2 - 2, " GPS ");

	mvwprintw(gps_s, 3, 1, "Latitude: %f", (double)gps.latitude);
	mvwprintw(gps_s, 4, 1, "Longitude: %f", (double)gps.longitude);
	mvwprintw(gps_s, 5, 1, "X: %f", (double)gps.point[0]);
	mvwprintw(gps_s, 6, 1, "Y: %f", (double)gps.point[1]);

	wrefresh(gps_s);
}

void PixyScreen(PixyCam& block) {
	block.update();

	wclear(pixy_s);
	box(pixy_s, 0 , 0);
	mvwprintw(pixy_s, 0, (COLS/3 - 8)/2 - 5, " Pixy Cam ");

	mvwprintw(pixy_s, 3, 1, "Object Type: %hu", block.block.type);
	mvwprintw(pixy_s, 4, 1, "Object Signature: %hu", block.block.signature);
	mvwprintw(pixy_s, 5, 1, "Object X: %hd", block.block.x);
	mvwprintw(pixy_s, 6, 1, "Object Y: %hd", block.block.y);
	mvwprintw(pixy_s, 7, 1, "Object Width: %hu", block.block.width);
	mvwprintw(pixy_s, 8, 1, "Object Height: %hu", block.block.height);
	mvwprintw(pixy_s, 9, 1, "Object Angle: %hu", block.block.angle);

	wrefresh(pixy_s);
}

void BNOScreen(BNO055& bno055) {
	Eigen::Vector2d acceleration;
	Eigen::Rotation2D<double> heading;

	bno055.linear_acceleration(acceleration);
	bno055.heading(heading);

	wclear(bno_s);
	box(bno_s, 0 , 0);
	mvwprintw(bno_s, 0, (COLS/3 - 8)/2 - 4, " 9 eixos ");

	mvwprintw(bno_s, 5, 1, "Acceleration: %.0f %.0f", acceleration[0], acceleration[1]);
	mvwprintw(bno_s, 6, 1, "Heading: %f", heading.angle());

	wrefresh(bno_s);
}

void BumperScreen(Bumper& bumper) {
	wclear(bumper_s);
	box(bumper_s, 0, 0);
	mvwprintw(bumper_s, 0, (COLS/3 - 8)/2 - 4, " Bumper ");

	if (bumper.pressed())
		mvwprintw(bumper_s, 5, 1, "Pressed");
	else
		mvwprintw(bumper_s, 5, 1, "Released");

	wrefresh(bumper_s);
}

void LeddarScreen(LeddarEK& leddar) {
	leddar.update();

	wclear(leddar_s);
	box(leddar_s, 0, 0);
	mvwprintw(leddar_s, 0, (COLS/3 - 8)/2 - 4, " Leddar ");

	mvwprintw(leddar_s, 5, 1, "Segment: %d", leddar.measure.mSegment);
	mvwprintw(leddar_s, 6, 1, "Distance: %f", leddar.measure.mDistance);

	wrefresh(leddar_s);
}

void LedsScreen() {
	wclear(leds_s);
	box(leds_s, 0, 0);
	mvwprintw(leds_s, 0, (COLS/3 - 8)/2 - 3, " LEDs ");

	mvwprintw(leds_s, 5, 1, "r - red");
	mvwprintw(leds_s, 6, 1, "v - green");
	mvwprintw(leds_s, 7, 1, "b - blue");

	mvwprintw(leds_s, 9, 1, "Setas - motor");
	mvwprintw(leds_s, 10, 1, "motor(%d, %d)", left, right);


	wrefresh(leds_s);
}

int main() {
	thread_spawn(motors_thread);

	GPSMonitor gps(GPS(-0.411087, -0.815589));
	LeddarEK leddar;
	PixyCam block;
	BNO055 bno055;
	Bumper bumper;
	char c;


	Leds ledr("LedRed");
	Leds ledg("LedGreen");
	Leds ledb("LedBlue");

	bool ledrOn = false;
	bool ledgOn = false;
	bool ledbOn = false;

	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);

	gps_s = newwin(LINES/2 - 6, COLS/3 - 8, 2, 2);
	box(gps_s, 0 , 0);

	pixy_s = newwin(LINES/2 - 6, COLS/3 - 8, 2, 2 + COLS/3 + 2);
	box(pixy_s, 0 , 0);

	bno_s = newwin(LINES/2 - 6, COLS/3 - 8, 2, 2 + 2 * (COLS/3 + 2));
	box(bno_s, 0 , 0);

	bumper_s = newwin(LINES/2 - 6, COLS/3 - 8, LINES/2 + 1, 2);
	box(bumper_s, 0 , 0);

	leddar_s = newwin(LINES/2 - 6, COLS/3 - 8, LINES/2  + 1, 2 + COLS/3 + 2);
	box(bumper_s, 0 , 0);

	leds_s = newwin(LINES/2 - 6, COLS/3 - 8, LINES/2 + 1, 2 + 2 * (COLS/3 + 2));
	box(bumper_s, 0 , 0);

	mvprintw(LINES - 1, 3, "q - quit");

	refresh();
	wrefresh(gps_s);
	wrefresh(bno_s);
	wrefresh(pixy_s);
	wrefresh(leds_s);
	wrefresh(bumper_s);
	wrefresh(leddar_s);

	LedsScreen();
	timeout(90);
	for (;;) {
		c = getch();
		switch (c) {
			case 'r': ledr = (ledrOn ? 0 : 1); ledrOn = !ledrOn; break;
			case 'v': ledg = (ledgOn ? 0 : 1); ledgOn = !ledgOn; break;
			case 'b': ledb = (ledbOn ? 0 : 1); ledbOn = !ledbOn; break;
			case KEY_UP:
				if (left < 60) left++;
				if (right < 60) right++;
			break;

			case KEY_DOWN:
				if (left > -60) left--;
				if (right > -60) right--;
				break;

			case KEY_LEFT:
				if (right > -60) right--;
				if (left < 60) left ++;
				break;

			case KEY_RIGHT:
				if (left > -60) left--;
				if (right < 60) right++;
				break;

			case ' ': left = right = 0; break;
			case 'q': endwin(); return 0; break;
			default: break;
		}
		GPSScreen(gps);
		PixyScreen(block);
		BNOScreen(bno055);
		BumperScreen(bumper);
		LeddarScreen(leddar);
		motor(left, right);
		sleep_ms(10);
	}

	endwin();
}
