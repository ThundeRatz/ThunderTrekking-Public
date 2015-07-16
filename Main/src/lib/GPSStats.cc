#include "GPSStats.hh"

namespace Trekking {
	GPSStats::GPSStats() : _latitude_stats(), _longitude_stats() {}

	const Stats& GPSStats::latitude_stats() {
		return _latitude_stats;
	}

	const Stats& GPSStats::longitude_stats() {
		return _longitude_stats;
	}

	void GPSStats::sample(const GPS &gps) {
		_latitude_stats.sample(gps.latitude);
		_longitude_stats.sample(gps.longitude);
	}
}
