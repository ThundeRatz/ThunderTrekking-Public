#include <iostream>
#include <iomanip>
#include <cmath>

#include "GPS.hh"
#include "sleep.hh"

using namespace std;
using namespace Trekking;

int main() {
	cout << fixed << setprecision(10);
	GPS origem(-0.41109563, -0.8155735890);
	GPSMonitor gps(origem);
	for (;;) {
		if(!gps.blocking_update()) {
			//cout << "Blocking update\n";
			sleep_ms(100);
			continue;
		}
		cout << "Latitude:  " << gps.latitude
			<< "\nLongitude: " << gps.longitude
			<< "\nX: " << gps.point[0]
			<< "\nY: " << gps.point[1] << endl;
		cout << "\nDistancia: " << gps.distance_to(origem) << " km" << endl
		     <<   "           " << sqrt(gps.point[0]*gps.point[0] + gps.point[1]*gps.point[1]) << " km" << endl << endl << endl;
		sleep_ms(50);
	}
}
