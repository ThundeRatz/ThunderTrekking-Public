#include <iostream>
#include <iomanip>
#include <cmath>

#include "GPS.hh"
#include "sleep.hh"
//#include "BNO055.hh"

#include "compass.h"

#define len(array)     ((&array)[1] - array)

using namespace std;
using namespace Trekking;

int main() {
	cout << fixed << setprecision(10);
	GPS pos[] = { GPS(-0.4110905127, -0.8155817966),    // Banco 1
				  GPS(-0.4110880729, -0.8155855454),    // Pe da escada
				  GPS(-0.4110880384, -0.8155887659),    // Arena do Hockey
				  GPS(-0.4110855167, -0.8155872874), }; // Outro banco
	GPSMonitor gps(pos[0]);
	//BNO055 bno;
	static Eigen::Rotation2D<double> heading;

	for (int i = 0; i < len(pos); i++)
		pos[i].to_2d(pos[i].point, pos[0]);

	for (;;) {
		if(!gps.blocking_update()) {
			//cout << "Blocking update\n";
			sleep_ms(100);
			continue;
		}
		system("clear");
		//bno.heading(heading);
		cout << "Atual:\n"
			 << "---Esferico:                       Plano:\n"
		 	 << "-----Latitude:  " << gps.latitude  << "    X: " << gps.point[0] << endl
			 << "-----Longitude: " << gps.longitude << "    Y: " << gps.point[1] << endl;
		cout << "---------------------------------------------------------------" << endl;

		for (int i = 0; i < len(pos); i++) {
			cout << "pos[" << i << "]\n"
			     << "---Esferico:                       Plano:\n"
			 	 << "-----Latitude:  " << pos[i].latitude  << "    X: " << pos[i].point[0] << endl
				 << "-----Longitude: " << pos[i].longitude << "    Y: " << pos[i].point[1] << endl
				 << "---Distancia: " << gps.distance_to(pos[i]) << " km" << endl;
			cout << "---Angulos:\n"
				 << "-----Azimuth: " << gps.azimuth_to(pos[i]) << endl;
			//	 << "-----Heading: " << heading.angle() << endl
			//	 << "-----Diff:    " << compass_diff(gps.azimuth_to(pos[i]), heading.angle()) << endl;
			cout << "---------------------------------------------------------------" << endl;
		}
		sleep_ms(50);
	}
}
