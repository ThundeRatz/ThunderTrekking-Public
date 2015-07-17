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
	GPIO::GPIO(int gpio) {
		this->gpio = gpio;
		if (exported()) {
			cout << "GPIO " << gpio << "already exported" << endl;
			if (fcntl(fd_value, F_GETFL) != -1)
				// Já aberto
				return;
		} else
			export_gpio();
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

	void GPIO::direction(Direction direction) {
		switch (direction) {
			case GPIO_IN:
			write_to_file("/sys/class/gpio/gpio" + to_string(gpio) + "/direction", "in");
			break;

			case GPIO_OUT_LOW:
			write_to_file("/sys/class/gpio/gpio" + to_string(gpio) + "/direction", "low");
			break;

			case GPIO_OUT_HIGH:
			write_to_file("/sys/class/gpio/gpio" + to_string(gpio) + "/direction", "high");
			break;

			default:
			throw runtime_error("GPIO direction: invalid argument");
			break;
		}
	}

	void GPIO::active_low() {
		write_to_file("/sys/class/gpio/gpio" + to_string(gpio) + "/active_low", "1");
	}

	void GPIO::edge(Edge edge_type) {
		switch (edge_type) {
			case GPIO_EDGE_NONE:
			write_to_file("/sys/class/gpio/gpio" + to_string(gpio) + "/edge", "none");
			break;

			case GPIO_EDGE_RISE:
			write_to_file("/sys/class/gpio/gpio" + to_string(gpio) + "/edge", "rise");
			break;

			case GPIO_EDGE_FALL:
			write_to_file("/sys/class/gpio/gpio" + to_string(gpio) + "/edge", "fall");
			break;

			case GPIO_EDGE_BOTH:
			write_to_file("/sys/class/gpio/gpio" + to_string(gpio) + "/edge", "both");
			break;

			default:
			throw runtime_error("GPIO edge: invalid argument");
			break;
		}
	}

	void GPIO::operator=(int value) {
		if (write(fd_value, value ? "1" : "0", 1) == -1) {
			perror("GPIO write");
			throw runtime_error("GPIO write failed");
		}
	}

	int GPIO::get() {
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

	void GPIO::write_to_file(string name, string value) {
		fstream file;
		file.open(name, fstream::out);
		file << value;
		file.close();
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
