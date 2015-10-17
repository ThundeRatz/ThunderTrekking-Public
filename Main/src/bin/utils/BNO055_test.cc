#include <eigen3/Eigen/Dense>
#include <iostream>

#include "BNO055.hh"
#include "sleep.hh"

int main() {
	Trekking::BNO055 bno055;
	Eigen::Vector2d acceleration;
	Eigen::Rotation2D<double> heading;
	for (;;) {
		bno055.linear_acceleration(acceleration);
		bno055.heading(heading);
		std::cout << "Accel: " << acceleration[0] << ' ' << acceleration[1] << '\n';
		std::cout << "Heading: " << heading.angle() << '\n';
		Trekking::sleep_ms(500);
	}
	return 0;
}
