#pragma once

#include <libevdev-1.0/libevdev/libevdev-uinput.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

namespace Trekking {
	class Bumper {
		private:
			struct libevdev *dev;
			int fd;
		public:
			Bumper();
			~Bumper();
			int pressed();
	};
}
