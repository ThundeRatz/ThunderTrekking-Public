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

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <exception>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "GPIO.hh"

using namespace std;

namespace Trekking {
	const std::string GPIO::IN = "in";
	const std::string GPIO::OUT_LOW = "low";
	const std::string GPIO::OUT_HIGH = "high";
	const std::string GPIO::EDGE_NONE = "none";
	const std::string GPIO::EDGE_RISE = "rise";
	const std::string GPIO::EDGE_FALL = "fall";
	const std::string GPIO::EDGE_BOTH = "both";

	GPIO::GPIO(int gpio) {
		this->gpio = gpio;
		if (exported()) {
			cout << "GPIO " << gpio << " already exported" << endl;
			if (fcntl(fd_value, F_GETFL) != -1)
				// Já aberto
				return;
		} else
			export_gpio();
		// Outra opção: se direction for chamado, inicializar fd_value lá se
		// &direction == &GPIO::GPIO_IN (mas não daria pra ler o valor de pinos
		// de saída)
		fd_value = open(("/sys/class/gpio/gpio" + to_string(gpio) + "/value").c_str(), O_RDWR);
		if (fd_value == -1) {
			perror("open");
			throw runtime_error("open failed");
		}
		poll_targets.events = POLLPRI;
		poll_targets.fd = fd_value;
	}

	GPIO::~GPIO() {
		close(fd_value);
	}

	void GPIO::export_gpio() {
		write_to_file("/sys/class/gpio/export", to_string(gpio));
		if (exported())
			return;
		throw runtime_error("GPIO export failed");
	}

	int GPIO::poll(int timeout) {
		int poll_status = ::poll(&poll_targets, 1, timeout);
		if (poll_status == -1) {
			perror("poll");
			return 0;
		}
		return poll_status;
	}

	void GPIO::poll() {
		poll(-1);
	}

	void GPIO::direction(const string &direction) {
		write_to_file("/sys/class/gpio/gpio" + to_string(gpio) + "/direction", direction);
	}

	void GPIO::active_low() {
		write_to_file("/sys/class/gpio/gpio" + to_string(gpio) + "/active_low", "1");
	}

	void GPIO::edge(const string &edge_type) {
		write_to_file("/sys/class/gpio/gpio" + to_string(gpio) + "/edge", edge_type);
	}

	void GPIO::operator=(bool value) {
		if (write(fd_value, value ? "1" : "0", 1) == -1) {
			perror("GPIO write");
			throw runtime_error("GPIO write failed");
		}
	}

	GPIO::operator bool() const {
		char buffer;
		if (read(fd_value, &buffer, 1) == -1) {
			perror("GPIO read");
			throw runtime_error("GPIO read failed");
		}
		if (lseek(fd_value, SEEK_SET, 0) == -1) {
			perror("GPIO lseek");
			throw runtime_error("GPIO lseek failed");
		}
		return buffer == '1';
	}

	void GPIO::write_to_file(const string& name, const string& value) {
		fstream file;
		file.open(name, fstream::out);
		file << value;
	}

	bool GPIO::exported() {
		struct stat st;
		if (stat(("/sys/class/gpio/gpio" + to_string(gpio)).c_str(), &st) == -1) {
			if (errno == ENOENT)
				return 0;
			else {
				perror("stat");
				throw runtime_error("stat error");
			}
		}
		return st.st_mode & S_IFDIR;
	}
}
