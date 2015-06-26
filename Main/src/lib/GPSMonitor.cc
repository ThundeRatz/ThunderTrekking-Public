#include <iostream>
#include <libgpsmm.h>

#include "GPSMonitor.hh"
#include "TooManyInstances.hh"

namespace Trekking {
	static gpsmm gpsd_client("localhost", DEFAULT_GPSD_PORT);
	GPSMonitor::GPSMonitor() {
		if (!instances) {
			instances++;
			gpsd_client.clear_fix();
			if (!gpsd_client.is_open()) {
				std::cout << "gpsmm() initialization failed" << std::endl;
				exit(0);
			}
		} else
			throw new TooManyInstances;
	}

	void GPSMonitor::blocking_update() {
		gpsd_data = gpsd_client.read();
		if (gpsd_data->fix.mode == MODE_2D || gpsd_data->fix.mode == MODE_3D) {
			latitude = gpsd_data->fix.latitude;
			longitude = gpsd_data->fix.longitude;
		}
	}

	void GPSMonitor::update() {
		if (gpsd_client.waiting(0))
			blocking_update();
	}
	int GPSMonitor::instances = 0;
	gpsmm GPSMonitor::gpsd_client("localhost", DEFAULT_GPSD_PORT);
}
