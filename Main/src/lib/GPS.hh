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
		double angle_to(const GPS& to);
		Eigen::Vector2d vector_to(const GPS& to);

	private:
		double haversine(double a);
		std::pair<double, double> convert_plane(const GPS &to);
	};

	class GPSMonitor : public GPS {
	public:
		gps_data_t *gpsd_data;
		GPSMonitor();
		bool blocking_update();
		bool update();

	private:
		gpsmm gpsd_client;
	};

	class DifferentialGPSMonitor : public GPSMonitor {
	public:
		gps_data_t *gpsd_data;
		DifferentialGPSMonitor(const GPS *gps_origin = NULL);
		bool blocking_update();
		bool update();
		double distance_to(const Eigen::Vector2d& to);
		double angle_to(const Eigen::Vector2d& to);
		void set_position();

		Eigen::Vector2d position;

	private:
		GPS origin;

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
