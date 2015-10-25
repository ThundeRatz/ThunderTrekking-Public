#include <iostream>
#include <iomanip>

#include "GPS.hh"
#include "sleep.hh"

using namespace std;
using namespace Trekking;

int main() {
	cout << fixed << setprecision(8);
	GPSMonitor gps(GPS(-0.411091, -0.815587));
	for (;;) {
		if(!gps.blocking_update()) {
			cout << "Blocking update\n";
			sleep_ms(200);
			continue;
		}
		cout << "Latitude: " << gps.latitude
			<< "\nLongitude: " << gps.longitude
			<< "\nX: " << gps.point[0]
			<< "\nY: " << gps.point[1] << endl;
		sleep_ms(100);
	}
}
