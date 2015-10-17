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
#include "GPS.hh"

using namespace Trekking;

static WINDOW* gps_s;
static WINDOW* pixy_s;
static WINDOW* bno_s;
static WINDOW* bumper_s;

static pixy_block_t block;
static GPSMonitor gps(GPS(0., 0.));
static BNO055 bno055;
static Bumper bumper;

void GPSScreen() {
	gps.update();

	wclear(gps_s);
	box(gps_s, 0 , 0);
	mvwprintw(gps_s, 1, 3, "GPS");

	mvwprintw(gps_s, 3, 1, "Latitude: %hu", gps.latitude);
	mvwprintw(gps_s, 4, 1, "Longitude: %hu", gps.longitude);

	wrefresh(gps_s);
}

void PixyScreen() {
	pixy_cam_get(&block);

	wclear(pixy_s);
	box(pixy_s, 0 , 0);
	mvwprintw(pixy_s, 1, 3, "Pixy Cam");

	mvwprintw(pixy_s, 3, 1, "Object Type: %hu", block.type);
	mvwprintw(pixy_s, 4, 1, "Object Signature: %hu", block.signature);
	mvwprintw(pixy_s, 5, 1, "Object X: %hd", block.x);
	mvwprintw(pixy_s, 6, 1, "Object Y: %hd", block.y);
	mvwprintw(pixy_s, 7, 1, "Object Width: %hu", block.width);
	mvwprintw(pixy_s, 8, 1, "Object Height: %hu", block.height);
	mvwprintw(pixy_s, 9, 1, "Object Angle: %hu", block.angle);

	wrefresh(pixy_s);
}

void BNOScreen() {
	Eigen::Vector2d acceleration;
	Eigen::Rotation2D<double> heading;

	bno055.linear_acceleration(acceleration);
	bno055.heading(heading);

	wclear(bno_s);
	box(bno_s, 0 , 0);
	mvwprintw(bno_s, 1, 3, "Sensor de 9 eixos");

	mvwprintw(bno_s, 5, 1, "Acceleration: %f %f", acceleration[0], acceleration[1]);
	mvwprintw(bno_s, 9, 1, "Heading: %f", heading.angle());

	wrefresh(bno_s);
}

void BumperScreen() {
	wclear(bumper_s);
	box(bumper_s, 0, 0);
	mvwprintw(bumper_s, 1, 3, "Bumper");

	if (bumper.pressed())
		mvwprintw(bno_s, 5, 1, "Pressed");
	else
		mvwprintw(bno_s, 5, 1, "Released");

	wrefresh(bumper_s);
}

int main() {
	pixy_cam_init();

	initscr();

	gps_s = newwin(LINES/2 - 6, COLS/2 - 6, 2, 2);
	box(gps_s, 0 , 0);

	pixy_s = newwin(LINES/2 - 6, COLS/2 - 6, 2, COLS/2 + 1);
	box(pixy_s, 0 , 0);

	bno_s = newwin(LINES/2 - 6, COLS/2 - 6, LINES/2 + 1, 2);
	box(bno_s, 0 , 0);

	bumper_s = newwin(LINES/2 - 6, COLS/2 - 6, LINES/2 + 1, COLS/2 + 1);
	box(bumper_s, 0 , 0);

	wrefresh(gps_s);
	wrefresh(pixy_s);
	wrefresh(bno_s);
	wrefresh(bumper_s);

	for (;;) {
		GPSScreen();
		PixyScreen();
		BNOScreen();
		BumperScreen();
		sleep_ms(200);
	}

	endwin();
}
