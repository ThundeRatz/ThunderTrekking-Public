#include <iostream>
#include <iomanip>
#include <cmath>

#include "GPS.hh"
#include "sleep.hh"
#include "BNO055.hh"

#include "compass.h"

using namespace std;
using namespace Trekking;

int main() {
	cout << fixed << setprecision(10);
	GPS origem(-0.41109563, -0.8155735890);
	GPS pos1();
	GPS pos2();
	GPSMonitor gps(origem);
	BNO055 bno;
	static Eigen::Rotation2D<double> heading;

	for (;;) {
		if(!gps.blocking_update()) {
			//cout << "Blocking update\n";
			sleep_ms(100);
			continue;
		}
		bno.heading(heading);
		cout << "Latitude:  " << gps.latitude
			<< "\nLongitude: " << gps.longitude
			<< "\nX: " << gps.point[0]
			<< "\nY: " << gps.point[1] << endl;
		cout << "\nDistancia: " << gps.distance_to(origem) << " km" << endl
		     <<   "           " << sqrt(gps.point[0] * gps.point[0] + gps.point[1] * gps.point[1]) << " km" << endl;
		cout << "Azimuth: " << gps.azimuth_to(origem) << endl;
		cout << "Heading: " << heading.angle() << endl;
		cout << "Angle Diff: " << compass_diff(gps.azimuth_to(origem), heading.angle()) << endl << endl << endl;
		sleep_ms(50);
	}
}
