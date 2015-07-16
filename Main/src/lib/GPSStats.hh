#pragma once

#include "Stats.hh"
#include "GPS.hh"

namespace Trekking {
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
