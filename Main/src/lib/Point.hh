#pragma once

#include "GPS.hh"

namespace Trekking {
	class GPS;
	class Point {
	public:
		double x, y;

		Point();
		Point(double x, double y);
		void to_3d(GPS& final, GPS origin);
	};
}
