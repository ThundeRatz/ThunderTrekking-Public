#include "Bumper.hh"

namespace Trekking {
	Bumper::Bumper() {
		int rc;
		dev = NULL;
		fd = open("/dev/input/by-path/platform-soc:keypad-event",
				  O_RDONLY|O_NONBLOCK);
		rc = libevdev_new_from_fd(fd, &dev);
			if (rc < 0) {
				 fprintf(stderr, "Failed to init libevdev (%s)\n", strerror(rc));
				 exit(1);
			 }
	}

	Bumper::~Bumper() {
		close(fd);
	}

	int Bumper::pressed() {
		struct input_event ev;
		if(!libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev))
			return ev.code;
		return 0;
	}
}
