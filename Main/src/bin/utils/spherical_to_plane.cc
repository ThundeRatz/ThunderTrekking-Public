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
	initial_position.latitude  = -0.412717417;
	initial_position.longitude = -0.812848889;
	initial_position.to_2d(ponto, initial_position);
	cout << " " << ponto.x << "  " << ponto.y << endl;
	cout << endl;

	for (;;) {
		//read_coordinates(position);
		position.latitude  = -0.412723865;
		position.longitude = -0.812845927;
		cout << initial_position.azimuth_to(position) << endl;

		position.to_2d(ponto, initial_position);
		cout << " " << ponto.x << "  " << ponto.y << endl;
		cout << M_PI - atan2(ponto.x, fabs(ponto.y)) << endl;

		double dist = sqrt(ponto.x * ponto.x + ponto.y * ponto.y);
		cout 	<< initial_position.distance_to(position) << endl;
		cout << dist << endl;
		double angle = M_PI - atan2(ponto.x, fabs(ponto.y)); // Quarto quadrante, colocar os outros depois
		initial_position.move_towards(dist, angle);
		cout << initial_position.latitude << " " << initial_position.longitude << endl;
		cout << initial_position.distance_to(position) << endl;

		cin.get();
	}
	return 0;
}
