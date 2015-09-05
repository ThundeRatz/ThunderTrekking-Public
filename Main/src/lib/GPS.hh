#pragma once

#include <eigen3/Eigen/Dense>
#include <libgpsmm.h>

#include "Stats.hh"

namespace Trekking {
	class GPS {
	public:
		double latitude, longitude;
		GPS();
		GPS(double latitude, double longitude);
		double distance_to(const GPS& to);
		double azimuth_to(const GPS& to);
		void to_2d(Eigen::Vector2d& point, GPS& origin);

	private:
		double haversine(double a);
	};

	class GPSMonitor: public GPS {
	public:
		gps_data_t *gpsd_data;
		GPSMonitor();
		bool blocking_update();
		bool update();

	private:
		gpsmm gpsd_client;
	};

	class GPSStats {
		public:
			GPSStats();
			const Stats &latitude_stats();
			const Stats &longitude_stats();
			void sample(const GPS &gps);

		private:
			Stats _latitude_stats, _longitude_stats;
	};
}
