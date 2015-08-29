#include <iostream>
#include <cstring>

#include "GPS.hh"
#include "ThreadSpawn.hh"
#include "ThreadGPS.hh"

using namespace std;

int main() {
	char error[32];
	thread_spawn(gps_thread);
	const struct timespec sleep_time = {.tv_sec = 1, .tv_nsec = 0};
	for (;;) {
		cout << gps_get().latitude << " " << gps_get().longitude << endl;
		if (nanosleep(&sleep_time, NULL))
			cerr << "nanosleep: " << strerror_r(errno, error, sizeof error) << endl;
	}
}
