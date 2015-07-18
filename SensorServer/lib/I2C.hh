#pragma once

#include <string>
#include <mutex>

namespace Trekking {
	class I2C;

	class I2CRegister {
	public:
		I2CRegister(I2C& i2c_bus, int index);
		int operator=(int value);
		operator int () const;
	private:
		I2C &i2c;
		int i2c_register;
	};

	class I2C {
		friend class I2CRegister;
	public:
		I2C(int dev_n, const std::string& dev_name);
		~I2C();
		I2CRegister const operator[](int index) const;
		I2CRegister operator[](int index);
		void adquire(int device);
		void release();
		void dump_info();
	private:
		std::mutex mutex;
		int fd;
	};
}
