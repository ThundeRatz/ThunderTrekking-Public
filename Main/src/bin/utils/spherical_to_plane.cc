#include <iostream>
#include <cmath>

#include "GPS.hh"

using namespace std;
using namespace Trekking;

void read_coordinates(GPS &pos) {
	int latitude, longitude;
	cin >> latitude >> longitude;
	if (!cin.good())
		exit(0);
	pos.latitude = latitude;
	pos.longitude = longitude;
}

int main() {
	GPS initial_position, position;

	read_coordinates(initial_position);
	cout << "0 0" << endl;
	for (;;) {
		double distance, azimuth;
		read_coordinates(position);
		azimuth = initial_position.azimuth_to(position);
		distance = initial_position.distance_to(position);

		cout << distance * sin(azimuth) << " " << distance * cos(azimuth) << endl;
	}
	return 0;
}
