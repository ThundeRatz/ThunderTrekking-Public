#pragma once

#include "Point.hh"

namespace Trekking {
	class Point;
	class GPS {
	public:
		double latitude, longitude;
		GPS();
		GPS(double latitude, double longitude);
		double distance_to(const GPS& to);
		double azimuth_to(const GPS& to);
		void final_position(GPS& return_position, double dist, double bearing);
		void move_towards(double dist, double bearing);
		GPS operator/ (int);
		GPS operator= (int);
		void to_2d(Point& point, GPS& origin);

	private:
		void init();
		double haversine(double a);
	};
}
