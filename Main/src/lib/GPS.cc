/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 ThundeRatz

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

// http://www.movable-type.co.uk/scripts/latlong.html possui explicações
// e exemplos de várias fórmulas para cálculo de deslocamentos, ângulos,
// menores distâncias e outros em coordenadas esféricas

#include <cmath>
#include <stdexcept>

#include "compass.h"
#include "GPS.hh"

#define EARTH_R			6371
#define TO_DEGREES(x)	((x) * 180. / M_PI)
#define TO_RAD(x)		((x) * M_PI / 180.)

using namespace std;

namespace Trekking {
	GPS::GPS() {}
	GPS::GPS(double latitude, double longitude) : latitude(latitude), longitude(longitude) {}

	/**
	 * Calculate distance between coordinates.
	 * Uses the haversine method.
	 * The spherical law of cosines is also possible, but not recommended for
	 * small distances (see
	 * http://gis.stackexchange.com/questions/4906/why-is-law-of-cosines-more-preferable-than-haversine-when-calculating-distance-b)
	 * @param[in] to other coordinate
	 */
	double GPS::distance_to(const GPS &to) {
		double	phy1 = latitude,
				phy2 = to.latitude,
				delta_lat = to.latitude - latitude,
				delta_long = to.longitude - longitude;
		double a, dist_angular;

		a = haversine(delta_lat) + haversine(delta_long) * cos(phy1) * cos(phy2);
		dist_angular = 2 * atan2(sqrt(a), sqrt(1 - a));

		return EARTH_R * dist_angular;
	}

	double GPS::distance_to_2d(const GPS& to) {
		return sqrt(point[0] * to.point[0] + point[1] * to.point[1]);
	}

	/**
	 * Forward Azimuth (initial angle)
	 * @param[in] to other coordinate
	 */
	double GPS::azimuth_to(const GPS &to) {
		double	y = sin(to.longitude - longitude) * cos(to.latitude),
	  			x = cos(latitude) * sin(to.latitude) -
				sin(latitude) * cos(to.latitude) *
				cos(to.longitude - longitude);
		return atan2(y, x);
	}

	double GPS::azimuth_to_2d(const GPS& to) {
		// return compass_orientation((to.point - point)[0], (to.point - point)[1]);
		double x = to.point[0] - point[0], y = to.point[1] - point[1];
		if (x >= 0 && y >= 0)
			return atan2(x, y);
		if (x < 0 && y >= 0)
			return 2*M_PI - atan2(fabs(x), y);
		if (x < 0 && y < 0)
			return M_PI + atan2(fabs(x), fabs(y));
		return M_PI - atan2(x, fabs(y));
	}

	void GPS::to_2d(Eigen::Vector2d& point, GPS& origin) {
		double distance, azimuth;
		azimuth = origin.azimuth_to(*this);
		distance = origin.distance_to(*this);
		point << distance * sin(azimuth), distance * cos(azimuth);
	}

	/**
	 * Haversine calculation
	 * haversine(x) = sin²(x / 2);
	 */
	double GPS::haversine(double a) {
		double sin_a2 = sin(a / 2);
		return sin_a2 * sin_a2;
	}

	GPSMonitor::GPSMonitor(GPS origin) : gpsd_client("localhost", DEFAULT_GPSD_PORT), origin(origin) {
		if (!gpsd_client.is_open())
			throw runtime_error("gpsmm() initialization failed");
		gpsd_client.clear_fix();
		gpsd_client.stream(WATCH_ENABLE | WATCH_JSON);
	}

	bool GPSMonitor::blocking_update() {
		gpsd_data = gpsd_client.read();
		if (gpsd_data == NULL)
			return false;
		if (gpsd_data->set & LATLON_SET) {
			latitude = TO_RAD(gpsd_data->fix.latitude);
			longitude = TO_RAD(gpsd_data->fix.longitude);
			update2d();
			return true;
		}
		return false;
	}

	bool GPSMonitor::update() {
		if (gpsd_client.waiting(0))
			return blocking_update();
		return false;
	}

	void GPSMonitor::update2d() {
		double distance, azimuth;
		azimuth = origin.azimuth_to(*this);
		distance = origin.distance_to(*this);
		point << distance * sin(azimuth), distance * cos(azimuth);
	}

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
