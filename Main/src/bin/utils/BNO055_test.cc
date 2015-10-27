#include <eigen3/Eigen/Dense>
#include <iostream>
#include <cstdlib>
#include "BNO055.hh"
#include "sleep.hh"

int main() {
	Trekking::BNO055 bno055;
<<<<<<< HEAD
	
	Eigen::Vector2d acceleration;
	Eigen::Rotation2D<double> heading;
	
	bno055.calibrate();
	
=======

	bno055.save_file();

	/*
>>>>>>> f850f60032445a1bf0149a0a7b320dbc360d81ac
	for (;;) {
		bno055.linear_acceleration(acceleration);
		bno055.heading(heading);
		std::cout << "Accel: " << acceleration[0] << ' ' << acceleration[1] << '\n';
		std::cout << "Heading: " << heading.angle() << '\n';
		Trekking::sleep_ms(100);
	}
	return 0;
}
