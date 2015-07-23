#include <cstdio>
#include <exception>
#include <stdexcept>
#include <cstring>

#include <sys/ioctl.h>
#include <poll.h>

#include "Button.hh"

#define KDSKBMODE		0x00004b45
#define K_RAW			0
#define K_XLATE			1

// Caso precise de raw keycodes:
// if (ioctl(0, KDSKBMODE, K_RAW) == -1) {
// 	perror("ioctl(0, KDSKBMODE, K_RAW)");
// 	throw std::runtime_error("ioctl(0, KDSKBMODE, K_RAW)");
// }
// ...
// if (ioctl(0, KDSKBMODE, K_XLATE) == -1)
// 	perror("ioctl(0, KDSKBMODE, K_XLATE)");

namespace Trekking {
	Button::LowLevelButtonSetup::LowLevelButtonSetup() {
        struct termios new_terminal;
        if (tcgetattr(0, &default_terminal))
                perror("tcsetattr");
		memcpy(&new_terminal, &default_terminal, sizeof(default_terminal));
        new_terminal.c_lflag &= ~ICANON;
        new_terminal.c_lflag &= ~ECHO;
        new_terminal.c_cc[VMIN] = 1;
        new_terminal.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &new_terminal))
            perror("tcsetattr");
	}

	Button::LowLevelButtonSetup::~LowLevelButtonSetup() {
		if (tcsetattr(0, TCSAFLUSH, &default_terminal))
				perror ("tcsetattr");
	}

	Button::Button() {
		mutex_button.lock();
	}

	Button::~Button() {
		free();
	}

	void Button::free() {
		mutex_button.unlock();
	}

	char Button::update() {
		return blocking_update(0);
	}

	char Button::blocking_update() {
		char input = 0;
		if (read(0, &input, 1) == -1)
			perror ("read");
		return input;

	}

	char Button::blocking_update(int timeout) {
		struct pollfd poll_stdin = {.fd = 0, .events = POLLIN | POLLPRI, .revents = 0};

		switch (poll(&poll_stdin, 1, timeout)) {
			case -1:
			perror("Button poll");
			throw std::runtime_error("Button poll");

			case 0:
			return 0;

			default:
			return blocking_update();
		}
	}
	std::mutex Button::mutex_button;
	Button::LowLevelButtonSetup Button::button_initializer{};
}
