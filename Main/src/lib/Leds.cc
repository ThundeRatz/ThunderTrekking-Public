#include "Leds.hh"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>

#include "errno_string.hh"

namespace Trekking {
	Leds::Leds(const std::string& name) : path("/sys/class/leds/" + name + "/") {
		brightness_fd = open((path + "brightness").c_str(), O_WRONLY);
		if (brightness_fd == -1)
			throw std::runtime_error("Leds open: " + ((std::string) errno_string()));
	}

	Leds::~Leds() {
		close(brightness_fd);
	}

	void Leds::operator=(int brightness) {
		std::string data = std::to_string(brightness) + "\n";
		ssize_t status = write(brightness_fd, data.c_str(), data.size());
		if (status == -1)
			std::cerr << "Leds write: " << errno_string() << '\n';
		else if (status != data.size())
			std::cerr << "Leds write: unexpected size\n";
	}

	void Leds::set_trigger(const std::string& trigger) {
		set_property("trigger", trigger);
	}

	void Leds::set_property(const std::string& name, const std::string& value) {
		int fd = open((path + name).c_str(), O_WRONLY);
		if (fd != -1)
			std::cerr << "Leds set_property open: " << errno_string() << '\n';
		else {
			std::string data = value + "\n";
			ssize_t status = write(fd, data.c_str(), data.size());
			if (status == -1)
				std::cerr << "Leds set_property: " << errno_string() << '\n';
			else if (status != data.size())
				std::cerr << "Leds set_property: unexpected size\n";
			close(fd);
		}
	}
}
