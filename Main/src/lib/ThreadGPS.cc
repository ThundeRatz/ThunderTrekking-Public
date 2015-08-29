#include <iostream>
#include <iomanip>
#include <cstring>
#include <mutex>

#include "GPSMonitor.hh"

using namespace std;
using namespace Trekking;

static GPS gps;
mutex gps_mutex;

void __attribute__((noreturn)) gps_thread() {
	cout << fixed << setprecision(9);
	char error[32];
	GPSMonitor gpsm;
	for (;;) {
		if (gpsm.update()) {
			if (gpsm.gpsd_data != NULL) {
				if (gpsm.gpsd_data->set & LATLON_SET) {
					try {
						gps_mutex.lock();
						gps.latitude = gpsm.gpsd_data->fix.latitude;
						gps.longitude = gpsm.gpsd_data->fix.longitude;
						gps_mutex.unlock();
					} catch (system_error& e) {
						cerr << "GPS Mutex: " << e.what() << endl;
					}
				}
			}
		} else {
			const struct timespec sleep_time = {.tv_sec = 1, .tv_nsec = 0};
            //cerr << "blocking_update error" << endl;
            if (nanosleep(&sleep_time, NULL))
                cerr << "nanosleep: " << strerror_r(errno, error, sizeof error) << endl;
		}
	}
}

GPS& gps_get() {
	return gps;
}

void gps_set(GPS& nova, double pass) {
	try {
		gps_mutex.lock();
		if (gps.latitude == 0.) {
			gps.latitude = nova.latitude;
			gps.longitude = nova.longitude;
		} else {
			gps.latitude = gps.latitude * (1 - pass) + pass * nova.latitude;
			gps.longitude = gps.longitude * (1 - pass) + pass * nova.longitude;
		}
		gps_mutex.unlock();
	} catch (system_error& e) {
		cerr << "GPS Mutex: " << e.what() << endl;
	}
}
