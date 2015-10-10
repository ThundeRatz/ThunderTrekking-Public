#include <eigen3/Eigen/Dense>
#include <iostream>

#include "BNO055.hh"

int main() {
	Trekking::BNO055 bno055;
	Eigen::Vector2d acceleration;
	Eigen::Rotation2D<double> heading;
	for (;;) {
		bno055.linear_acceleration(acceleration);
		bno055.heading(heading);
		std::cout << acceleration;
		std::cout << heading.angle() << '\n';
	}
	return 0;
}
