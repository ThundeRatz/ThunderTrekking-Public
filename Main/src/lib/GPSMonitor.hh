#pragma once

#include <libgpsmm.h>
#include "GPS.hh"

namespace Trekking {
	class GPSMonitor: public GPS {
	public:
		gps_data_t *gpsd_data;
		GPSMonitor();
		void blocking_update();
		void update();

	private:
		static gpsmm gpsd_client;
		static int instances;
	};
}
