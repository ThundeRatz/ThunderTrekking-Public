#include <iostream>
#include <iomanip>
#include <cmath>

#include "GPS.hh"
#include "sleep.hh"
#include "BNO055.hh"

#include "Compass.hh"

#define len(array)     ((&array)[1] - array)

using namespace std;
using namespace Trekking;

int main() {
	cout << fixed << setprecision(10);
	GPS pos[] = { GPS(-0.4110876552, -0.8155653042),
			GPS(-0.4110851508, -0.8155695841),
			// GPS(-0.3705620335, -0.7852462032),
			//GPS(-0.3705658885, -0.7852487568),
	};
	Eigen::Vector2d positions[4];
	DifferentialGPSMonitor gps;
	BNO055 bno;
	static Eigen::Rotation2D<double> angle;

	for (int i = 0; i < len(pos); i++)
		positions[i] = pos[0].vector_to(pos[i]);

	for (;;) {
		if(!gps.blocking_update()) {
			cout << "Blocking update\n";
			sleep_ms(100);
			continue;
		}
		system("clear");
		angle = bno.read_angle();
		cout << "Atual:\n"
			<< "---Esferico:                       Plano:\n"
			<< "-----Latitude:  " << gps.latitude  << "    X: " << gps.position[0] << endl
			<< "-----Longitude: " << gps.longitude << "    Y: " << gps.position[1] << endl;
		cout << "---------------------------------------------------------------" << endl;

		for (int i = 0; i < len(pos); i++) {
			cout << "pos[" << i << "]\n"
				<< "---Esferico:                       Plano:\n"
				<< "-----Latitude:  " << pos[i].latitude  << "    X: " << positions[i][0] << endl
				<< "-----Longitude: " << pos[i].longitude << "    Y: " << positions[i][1] << endl
				<< "---Distancia: " << gps.distance_to(positions[i]) << " km" << endl;
			cout << "---Angulos:\n"
				<< "-----Azimuth: " << gps.azimuth_to(pos[i]) << endl
				<< "-----Compass angle: " << angle.angle() << endl
				<< "-----Diff:    " << Compass::diff(angle.angle(), gps.azimuth_to(pos[i])) << endl;
			cout << "---------------------------------------------------------------" << endl;
		}
		sleep_ms(50);
	}
}
