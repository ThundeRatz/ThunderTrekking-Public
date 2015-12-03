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
#include <iostream>
#include <stdexcept>

#include "Compass.hh"
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

	/**
	 * Forward Azimuth (initial angle).
	 * @param[in] to other coordinate
	 */
	double GPS::azimuth_to(const GPS &to) {
		pair<double, double> position = convert_plane(to);
		return atan2(position.second, position.first);
	}

	double GPS::angle_to(const GPS &to) {
		/// @todo Unit test
		pair<double, double> position = convert_plane(to);
		return atan2(position.first, position.second);
	}

	Eigen::Vector2d GPS::vector_to(const GPS& to) {
		/// @todo Unit test
		double distance, angle;
		angle = angle_to(to);
		distance = distance_to(to);
		return (Eigen::Vector2d() << distance * sin(angle), distance * cos(angle)).finished();
	}

	/**
	 * GPS to plane conversion.
	 * Conversion to plane coordinates for calculation of heading/angle. This
	 * isn't for distances!
	 */
	pair<double, double> GPS::convert_plane(const GPS &to) {
		pair<double, double> position;
		position.second = sin(to.longitude - longitude) * cos(to.latitude);
		position.first = cos(latitude) * sin(to.latitude) -
			sin(latitude) * cos(to.latitude) *
			cos(to.longitude - longitude);
		return position;
	}

	/**
	 * Haversine calculation.
	 * haversine(x) = sin²(x / 2);
	 */
	double GPS::haversine(double a) {
		double sin_a2 = sin(a / 2);
		return sin_a2 * sin_a2;
	}

	GPSMonitor::GPSMonitor() : gpsd_client("localhost", DEFAULT_GPSD_PORT) {
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
			return true;
		}
		return false;
	}

	bool GPSMonitor::update() {
		if (gpsd_client.waiting(0))
			return blocking_update();
		return false;
	}

	DifferentialGPSMonitor::DifferentialGPSMonitor(const GPSMonitor *gps_origin) {
		position << 0, 0;
		if (gps_origin != NULL) {
			if (!(gps_origin->gpsd_data->set & LATLON_SET))
				throw runtime_error("DifferentialGPSMonitor initialized with gps_origin without LATLON_SET");
			origin = *gps_origin;
		} else {
			while (!blocking_update())
				cout << "DifferentialGPSMonitor: waiting origin\n";
			origin = *this;
		}
	}

	bool DifferentialGPSMonitor::blocking_update() {
		if (!GPSMonitor::blocking_update())
			return false;
		set_position();
		return true;
	}

	bool DifferentialGPSMonitor::update() {
		if (!GPSMonitor::update())
			return false;
		set_position();
		return true;
	}

	void DifferentialGPSMonitor::set_position() {
		double distance = origin.distance_to(*this);
		double angle = origin.angle_to(*this);
		position << distance * sin(angle), distance * cos(angle);
	}

	double DifferentialGPSMonitor::distance_to(const Eigen::Vector2d& to) {
		return (to - position).norm();
	}

	double DifferentialGPSMonitor::angle_to(const Eigen::Vector2d& to) {
		Eigen::Vector2d resultant = to - position;
		return atan2(resultant[1], resultant[0]);
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
