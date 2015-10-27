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

#pragma once

#include <eigen3/Eigen/Dense>
#include <string>

#include "bno055.h"
#include "I2C.hh"

namespace Trekking {
	class BNO055 {
	public:
		BNO055();
		void linear_acceleration(Eigen::Vector2d& acceleration_return);
		void heading(Eigen::Rotation2D<double>& new_heading);
		void get_calibration();
		void save_file();

	private:
		static Trekking::I2C bno055_i2c;
		static s8 read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt);
		static s8 write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt);
		static void delay_ms(u32 ms);
		u8 mag;
		u8 accel;
		u8 gyro;
		u8 sys;
	};
}
