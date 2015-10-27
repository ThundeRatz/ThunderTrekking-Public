#include <eigen3/Eigen/Dense>
#include <iostream>
#include <cstdlib>
#include "BNO055.hh"
#include "sleep.hh"

int main() {
	FILE *file;
	Trekking::BNO055 bno055;
	Eigen::Vector2d acceleration;
	Eigen::Rotation2D<double> heading;
	struct bno055_sic_matrix_t *p;
	u8 v_mag_calib_u8;
	u8 v_accel_calib_u8;
	u8 v_gyro_calib_u8;
	u8 v_sys_calib_u8;
	
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
