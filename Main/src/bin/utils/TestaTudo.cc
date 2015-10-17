#include <eigen3/Eigen/Dense>
#include <stdexcept>
#include <cmath>
#include <ctime>

#include <ncurses.h>

#include "ThreadPixy.hh"
#include "LedsI2C.hh"
#include "BNO055.hh"
#include "Bumper.hh"
#include "sleep.hh"
#include "Leds.hh"
#include "GPS.hh"

using namespace Trekking;

static WINDOW* gps_s;
static WINDOW* pixy_s;
static WINDOW* bno_s;
static WINDOW* bumper_s;

void GPSScreen(GPSMonitor& gps) {
	wclear(gps_s);
	if(!gps.update())
		mvwprintw(gps_s, LINES/2 - 6 - 5, 1, "blocking_update error");

	box(gps_s, 0 , 0);
	mvwprintw(gps_s, 1, (COLS/2 - 6)/2 - 1, "GPS");

	mvwprintw(gps_s, 3, 1, "Latitude: %hu", gps.latitude);
	mvwprintw(gps_s, 4, 1, "Longitude: %hu", gps.longitude);

	wrefresh(gps_s);
}

void PixyScreen(pixy_block_t& block) {
	pixy_cam_get(&block);

	wclear(pixy_s);
	box(pixy_s, 0 , 0);
	mvwprintw(pixy_s, 1, (COLS/2 - 6)/2 - 4, "Pixy Cam");

	mvwprintw(pixy_s, 3, 1, "Object Type: %hu", block.type);
	mvwprintw(pixy_s, 4, 1, "Object Signature: %hu", block.signature);
	mvwprintw(pixy_s, 5, 1, "Object X: %hd", block.x);
	mvwprintw(pixy_s, 6, 1, "Object Y: %hd", block.y);
	mvwprintw(pixy_s, 7, 1, "Object Width: %hu", block.width);
	mvwprintw(pixy_s, 8, 1, "Object Height: %hu", block.height);
	mvwprintw(pixy_s, 9, 1, "Object Angle: %hu", block.angle);

	wrefresh(pixy_s);
}

void BNOScreen(BNO055& bno055) {
	Eigen::Vector2d acceleration;
	Eigen::Rotation2D<double> heading;

	bno055.linear_acceleration(acceleration);
	bno055.heading(heading);

	wclear(bno_s);
	box(bno_s, 0 , 0);
	mvwprintw(bno_s, 1, (COLS/2 - 6)/2 - 3, "9 eixos");

	mvwprintw(bno_s, 5, 1, "Acceleration: %.0f %.0f", acceleration[0], acceleration[1]);
	mvwprintw(bno_s, 6, 1, "Heading: %f", heading.angle());

	wrefresh(bno_s);
}

void BumperScreen(Bumper& bumper) {
	wclear(bumper_s);
	box(bumper_s, 0, 0);
	mvwprintw(bumper_s, 1, (COLS/2 - 6)/2 - 3, "Bumper");

	if (bumper.pressed())
		mvwprintw(bumper_s, 5, 1, "Pressed");
	else
		mvwprintw(bumper_s, 5, 1, "Released");

	wrefresh(bumper_s);
}

int main() {
	pixy_block_t block;
	GPSMonitor gps(GPS(0., 0.));
	BNO055 bno055;
	Bumper bumper;
	char c;

	Leds ledr("LedRed");
	Leds ledg("LedGreen");
	Leds ledb("LedBlue");

	bool ledrOn = false;
	bool ledgOn = false;
	bool ledbOn = false;

	pixy_cam_init();

	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);

	gps_s = newwin(LINES/2 - 6, COLS/2 - 6, 2, 2);
	box(gps_s, 0 , 0);

	pixy_s = newwin(LINES/2 - 6, COLS/2 - 6, 2, COLS/2 + 1);
	box(pixy_s, 0 , 0);

	bno_s = newwin(LINES/2 - 6, COLS/2 - 6, LINES/2, 2);
	box(bno_s, 0 , 0);

	bumper_s = newwin(LINES/2 - 6, COLS/2 - 6, LINES/2, COLS/2 + 1);
	box(bumper_s, 0 , 0);

	mvprintw(LINES - 1, 3, "LEDs: r - toggle red, g - toggle green, b - toggle blue | q - quit");

	refresh();
	wrefresh(gps_s);
	wrefresh(pixy_s);
	wrefresh(bno_s);
	wrefresh(bumper_s);

	timeout(90);
	for (;;) {
		c = getch();
		switch (c) {
			case 'r': ledr = (ledrOn ? 0 : 1); ledrOn = !ledrOn; break;
			case 'g': ledg = (ledgOn ? 0 : 1); ledgOn = !ledgOn; break;
			case 'b': ledb = (ledbOn ? 0 : 1); ledbOn = !ledbOn; break;
			case 'q': endwin(); return 0; break;
			default: break;
		}
		GPSScreen(gps);
		PixyScreen(block);
		BNOScreen(bno055);
		BumperScreen(bumper);
		sleep_ms(10);
	}

	endwin();
}
