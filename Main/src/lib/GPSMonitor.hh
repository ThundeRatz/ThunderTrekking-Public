#pragma once

#include <libgpsmm.h>
#include "GPS.hh"

namespace Trekking {
	class GPSMonitor: public GPS {
	public:
		gps_data_t *gpsd_data;
		GPSMonitor();
		bool blocking_update();
		bool update();

	private:
		gpsmm gpsd_client;
	};
}
