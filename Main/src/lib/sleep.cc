#include <iostream>

#include "errno_string.hh"
#include "sleep.hh"

void Trekking::sleep_ms(long ms) {
	struct timespec sleep_time = {
		.tv_sec = ms / 1000,
		.tv_nsec = (ms % 1000) * 1000000
	};
	if(nanosleep(&sleep_time, NULL))
		std::cerr << "sleep_ms: " << errno_string() << std::endl;
}

void Trekking::sleep_us(long us) {
	struct timespec sleep_time = {
		.tv_sec = us / 1000000,
		.tv_nsec = (us % 1000000) * 1000
	};
	if(nanosleep(&sleep_time, NULL))
		std::cerr << "sleep_us: " << errno_string() << std::endl;
}

void Trekking::sleep_ns(long ns) {
	struct timespec sleep_time = {
		.tv_sec = ns / 1000000000,
		.tv_nsec = (ns % 1000000000)
	};
	if(nanosleep(&sleep_time, NULL))
		std::cerr << "sleep_ns: " << errno_string() << std::endl;
}
