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

// https://www.kernel.org/doc/Documentation/i2c/dev-interface
// https://www.kernel.org/doc/Documentation/i2c/smbus-protocol
// https://www.kernel.org/doc/Documentation/i2c/functionality

#include <iostream>
#include <fstream>

#include <linux/i2c.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "I2C.hh"

using namespace std;

namespace Trekking {
	I2CRegister::I2CRegister(I2C &i2c_bus, int index) : i2c(i2c_bus), i2c_register(index) {}

	int I2CRegister::operator=(int value) {
		int32_t status = i2c_smbus_write_byte_data(i2c.fd, i2c_register, value);
		if (status < 0) {
			throw runtime_error("I2CRegister write failed - " + to_string(status));
		}
		return value;
	}

	I2CRegister::operator int () const {
		int status = i2c_smbus_read_byte_data(i2c.fd, i2c_register);
		if (status < 0) {
			perror("i2c_smbus_read_byte_data");
			throw runtime_error("I2CRegister read failed");
		}
		return status;
	}

	I2C::I2C(int dev_n, const std::string& name) {
		fstream sysfs_name_file;
		string sysfs_name;
		sysfs_name_file.open("/sys/class/i2c-dev/i2c-" + to_string(dev_n) + "/name", fstream::in);
		std::getline(sysfs_name_file, sysfs_name);
		if (sysfs_name != name) {
			cerr << "i2c_open: Falha na checagem do nome do adaptador\nEsperado "
				<< name << ", encontrou " << sysfs_name << endl;
			throw runtime_error("I2C sysfs check");
		}
		fd = open(("/dev/i2c-" + to_string(dev_n)).c_str(), O_RDWR);
		if (fd < 0) {
			perror("open");
			throw runtime_error("I2C open");
		}
#ifdef DEBUG
		dump_info();
#endif
	}

	I2C::~I2C() {
		close(fd);
	}

	void I2C::dump_info() {
		long funcs;
		if (ioctl(fd, I2C_FUNCS, &funcs) == -1) {
			perror("ioctl");
			close(fd);
			throw runtime_error("I2C ioctl");
		}
#define I2C_CHECK_FEATURE(feature)		cout << #feature << ((funcs & feature) ? "YES" : "NO") << endl
		cout << "I2C features:" << endl;
		I2C_CHECK_FEATURE(I2C_FUNC_10BIT_ADDR);	// suporta endereços de 10 bits
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_PEC);	// packet error checking
		I2C_CHECK_FEATURE(I2C_FUNC_I2C);		// combined read/write
		I2C_CHECK_FEATURE(I2C_FUNC_PROTOCOL_MANGLING);
		I2C_CHECK_FEATURE(I2C_FUNC_NOSTART);
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_BLOCK_PROC_CALL);
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_QUICK);
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_READ_BYTE);	// ou
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_WRITE_BYTE);	// I2C_FUNC_SMBUS_BYTE
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_READ_BYTE_DATA);
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_WRITE_BYTE_DATA);
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_READ_WORD_DATA);
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_WRITE_WORD_DATA);
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_PROC_CALL);
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_READ_BLOCK_DATA);
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_WRITE_BLOCK_DATA);
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_READ_I2C_BLOCK);
		I2C_CHECK_FEATURE(I2C_FUNC_SMBUS_WRITE_I2C_BLOCK);
	}

	void I2C::acquire(int device) {
		mutex.lock();
		ioctl(fd, I2C_SLAVE, device);
	}

	void I2C::release() {
		mutex.unlock();
	}

	I2CRegister I2C::operator[](int index) {
		return I2CRegister(*this, index);
	}
}
