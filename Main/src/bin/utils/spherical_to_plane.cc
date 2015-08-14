#include <iostream>
#include <iomanip>
#include <cmath>

#include "GPS.hh"
#include "Point.hh"

#define MS	1000000

#define TO_DEGREES(x)	((x) * 180. / M_PI)

using namespace std;
using namespace Trekking;

void read_coordinates(GPS &pos) {
	cout << "Lat: ";
	cin >> pos.latitude;
	cout << "Lon: ";
	cin >> pos.longitude;
	if (!cin.good())
		exit(0);
}

int main() {
	cout << fixed << setprecision(10);
	GPS initial_position, position;
	Point ponto(0, 0);

	//read_coordinates(initial_position);
	initial_position.latitude  = -23.64696611;
	initial_position.longitude = -46.57281075;
	initial_position.to_2d(ponto, initial_position);
	cout << " " << ponto.x << "  " << ponto.y << endl;
	cout << endl;

	for (;;) {
		//read_coordinates(position);
		position.latitude  = -23.64733556;
		position.longitude = -46.57264100;
		cout << initial_position.azimuth_to(position) << endl;

		position.to_2d(ponto, initial_position);
		cout << " " << ponto.x << "  " << ponto.y << endl;
		cout << M_PI - atan2(ponto.x, fabs(ponto.y)) << endl;

		initial_position.move_towards(ponto);
		cout << initial_position.latitude << " " << initial_position.longitude << endl;

		cin.get();
	}
	return 0;
}
