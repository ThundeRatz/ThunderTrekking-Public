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
		void acquire(int device);
		void release();
		void dump_info();
	private:
		/// @todo há um único mutex para todos os barramentos, vale a pena repensar
		/// isso se for usar vários.
		std::mutex mutex;
		int fd;
	};
}
