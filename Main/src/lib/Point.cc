#include <cmath>

#include "Point.hh"

namespace Trekking {
	Point::Point() {}
	Point::Point(double x, double y) : x(x), y(y) {}

	// void Point::to_3d(GPS& final, const GPS& origin) {
	// 	double distance, angle;
	// 	distance = sqrt(this->x * this->x + this->y * this->y);
	// 	angle = atan2(this->y, this->x);
	// 	final = origin;
	// 	final.move_towards(distance, angle);
	// }
}
