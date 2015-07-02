#if GPSD_API_MAJOR_VERSION != 6
#error GPSd API 6 required
#endif

#include <iostream>
#include <libgpsmm.h>

#include "GPSMonitor.hh"

namespace Trekking {
	GPSMonitor::GPSMonitor() : gpsd_client("localhost", DEFAULT_GPSD_PORT) {
		if (!gpsd_client.is_open()) {
			std::cout << "gpsmm() initialization failed" << std::endl;
			exit(0);
		}
		gpsd_client.clear_fix();
		gpsd_client.stream(WATCH_ENABLE | WATCH_JSON);
	}

	int GPSMonitor::blocking_update() {
		gpsd_data = gpsd_client.read();
		if (gpsd_data == NULL)
			return -1;
		if (gpsd_data->fix.mode == MODE_2D || gpsd_data->fix.mode == MODE_3D) {
			latitude = gpsd_data->fix.latitude;
			longitude = gpsd_data->fix.longitude;
		}
		return 0;
	}

	int GPSMonitor::update() {
		if (gpsd_client.waiting(0))
			return blocking_update();
		return -1;
	}
}
