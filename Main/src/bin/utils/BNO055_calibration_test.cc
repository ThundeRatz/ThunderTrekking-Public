#include <eigen3/Eigen/Dense>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>

#include "BNO055.hh"
#include "sleep.hh"

using namespace std;

int main() {
	cout << fixed;
	Trekking::BNO055 bno055;

	u8 mag;
	u8 accel;
	u8 gyro;
	u8 sys;

	bno055_get_accel_calib_stat(&accel);
	bno055_get_gyro_calib_stat(&gyro);
	bno055_get_mag_calib_stat(&mag);
	bno055_get_sys_calib_stat(&sys);

	while(mag != 3 || accel != 3 || gyro != 3) {

		bno055_get_accel_calib_stat(&accel);
		bno055_get_gyro_calib_stat(&gyro);
		bno055_get_mag_calib_stat(&mag);
		bno055_get_sys_calib_stat(&sys);

		cout << "Mag: " << unsigned(mag)
				 << "\nAccel: " << unsigned(accel)
				 << "\nGyro: " << unsigned(gyro)
				 << "\nAll: " << unsigned(sys) << endl << endl;
		Trekking::sleep_ms(100);
	}

	ofstream file("Calibration.txt");
	struct bno055_accel_offset_t accel_offset;
	struct bno055_gyro_offset_t gyro_offset;
	struct bno055_mag_offset_t mag_offset;
	bno055_read_accel_offset(&accel_offset);
	bno055_read_gyro_offset(&gyro_offset);
	bno055_read_mag_offset(&mag_offset);

	file << accel_offset.x << " " << accel_offset.y << " "
			 << accel_offset.z << " " << accel_offset.r << " "
			 << gyro_offset.x << " " << gyro_offset.y << " "
			 << gyro_offset.z << " "
			 << mag_offset.x << " " << mag_offset.y << " "
			 << mag_offset.z << " " << mag_offset.r << endl;
	file.close();
	//Test
	cout << accel_offset.x << " " << accel_offset.y << " "
			 << accel_offset.z << " " << accel_offset.r << " "
			 << gyro_offset.x << " " << gyro_offset.y << " "
			 << gyro_offset.z << " "
			 << mag_offset.x << " " << mag_offset.y << " "
			 << mag_offset.z << " " << mag_offset.r << endl;

 bno055.load_file();

	return 0;
}
