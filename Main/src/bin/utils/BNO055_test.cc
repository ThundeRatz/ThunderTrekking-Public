#include <eigen3/Eigen/Dense>
#include <iostream>
#include <cstdlib>
#include "BNO055.hh"
#include "sleep.hh"

int main() {
	FILE *file;
	Trekking::BNO055 bno055;

	bno055.save_file();

	/*
	for (;;) {
		bno055.linear_acceleration(acceleration);
		bno055.heading(heading);
		std::cout << "Accel: " << acceleration[0] << ' ' << acceleration[1] << '\n';
		std::cout << "Heading: " << heading.angle() << '\n';
		Trekking::sleep_ms(100);
	}*/
	return 0;
}
