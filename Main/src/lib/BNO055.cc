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

#include <ctime>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "BNO055.hh"

using namespace Eigen;
using namespace std;

namespace Trekking {
	Trekking::I2C BNO055::bno055_i2c(BNO055_I2C_BUS, BNO055_I2C_BUS_NAME);

	BNO055::BNO055() {
		ifstream file("Calibration.txt");
		static struct bno055_t bno055;
		bno055.dev_addr = BNO055_I2C_ADDR2;
		bno055.bus_write = write;
		bno055.bus_read = read;
		bno055.delay_msec = delay_ms;

		file >> mag >> accel >> gyro >> sys;

		if (bno055_init(&bno055))
			throw runtime_error("BNO055 initialization failed");
		if (bno055_set_power_mode(POWER_MODE_NORMAL))
			throw runtime_error("BNO055 power mode failed");
		if (bno055_set_operation_mode(OPERATION_MODE_NDOF))
			throw runtime_error("BNO055 operation mode failed");
		if (bno055_set_accel_slow_no_motion_durn(0x3f))
			throw runtime_error("BNO055 accel no motion sleep failed");

		if (bno055_set_remap_x_sign(REMAP_AXIS_POSITIVE))
			throw runtime_error("Error on set_remap_x_sign");
		if (bno055_set_remap_y_sign(REMAP_AXIS_POSITIVE))
			throw runtime_error("Error on set_remap_y_sign");
		if (bno055_set_remap_z_sign(REMAP_AXIS_POSITIVE))
			throw runtime_error("Error on set_remap_z_sign");
	}

	void BNO055::linear_acceleration(Vector2d& acceleration_return) {
		bno055_linear_accel_t acceleration;
		if (bno055_read_linear_accel_xyz(&acceleration)) {
			cerr << "bno055_read_linear_accel_xyz failed\n";
			acceleration_return << 0, 0;
		}
		acceleration_return << acceleration.x, acceleration.y;
	}

	void BNO055::heading(Rotation2D<double>& new_heading) {
		double alpha;
		if (bno055_convert_double_euler_h_rad(&alpha))
			cerr << "bno055_convert_double_euler_h_rad failed\n";
		new_heading.angle() = alpha;
	}

	s8 BNO055::read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
		bno055_i2c.acquire(dev_addr);
		for (u8 i = 0; i < cnt; i++)
			*(reg_data + i) = bno055_i2c[reg_addr + i];
		bno055_i2c.release();
		return 0;
	}

	s8 BNO055::write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
		bno055_i2c.acquire(dev_addr);
		for (u8 i = 0; i < cnt; i++)
			bno055_i2c[reg_addr + i] = *(reg_data + i);
		bno055_i2c.release();
		return 0;
	}

	void BNO055::delay_ms(u32 ms) {
		struct timespec sleep_time = {
			.tv_sec = ms / 1000,
			.tv_nsec = (ms % 1000) * 1000000
		};
		nanosleep(&sleep_time, NULL);
	}
	void BNO055::save_file(){
		ofstream file("Calibration.txt");
		file << mag << " " << accel << " " << gyro << " " << sys << endl;
	}
}
