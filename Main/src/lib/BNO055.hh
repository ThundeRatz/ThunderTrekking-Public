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
		void heading(Eigen::Rotation2D<double> new_heading);

	private:
		static Trekking::I2C bno055_i2c;

		static s8 read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt);
		static s8 write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt);
		static void delay_ms(u32 ms);
	};
}
