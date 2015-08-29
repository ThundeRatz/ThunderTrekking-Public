#include <ctime>
#include <iostream>
#include <stdexcept>

#include "BNO055.hh"

using namespace Eigen;
using namespace std;

namespace Trekking {
	BNO055::BNO055() {
		struct bno055_t bno055 = {
			.bus_write = write,
			.bus_read = read,
			.delay_msec = delay_ms,
			.dev_addr = BNO055_I2C_ADDR1
		};

		if (bno055_init(&bno055))
			throw runtime_error("BNO055 initialization failed");
		if (bno055_set_power_mode(POWER_MODE_NORMAL))
			throw runtime_error("BNO055 power mode failed");
		if (bno055_set_operation_mode(OPERATION_MODE_NDOF))
			throw runtime_error("BNO055 operation mode failed");
		if (bno055_set_accel_slow_no_motion_durn(0x3f))
#warning verificar aqui
			throw runtime_error("BNO055 accel no motion sleep failed");
	}

	void BNO055::linear_acceleration(Vector2d& acceleration_return) {
		bno055_linear_accel_t acceleration;
		if (bno055_read_linear_accel_xyz(&acceleration)) {
			cerr << "bno055_read_linear_accel_xyz failed\n";
			acceleration_return << 0, 0;
		}
		acceleration_return << acceleration.x, acceleration.y;
	}

	void BNO055::heading(Rotation2D<double> new_heading) {
		double alpha;
		if (bno055_convert_double_euler_h_rad(&alpha))
			cerr << "bno055_convert_double_euler_h_rad failed\n";
		new_heading.angle() = alpha;
	}

	s8 BNO055::read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
		i2c.acquire(dev_addr);
		for (u8 i = 0; i < cnt; i++)
			*(reg_data + i) = i2c[reg_addr + i];
		i2c.release();
	}

	s8 BNO055::write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
		i2c.acquire(dev_addr);
		for (u8 i = 0; i < cnt; i++)
			i2c[reg_addr + i] = *(reg_data + i);
		i2c.release();
	}

	void BNO055::delay_ms(u32 ms) {
		struct timespec sleep_time = {
			.tv_sec = ms / 1000,
			.tv_nsec = (ms % 1000) * 1000000
		};
		nanosleep(&sleep_time, NULL);
	}
}
