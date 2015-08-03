#include <iostream>
#include <iomanip>
#include <cmath>

#include "TrekkingKF.hh"

#include "thread_spawn.h"
#include "thread_gps.h"
#include "gps_coord.h"
#include "GPS.hh"

using namespace std;
using namespace Trekking;

typedef TrekkingKF::Vector Vetor;
typedef TrekkingKF::Matrix Matriz;

int main () {
	thread_spawn(gps_thread, NULL);

	TrekkingKF filtro;
	const int n = 4;
	const int m = 4;

	const double _P0[] = {100.0, 0.0, 0.0, 0.0,  // TODO Ver esses valores
						  0.0, 0.0, 0.0, 0.0,
						  0.0, 0.0, 100.0, 0.0,
						  0.0, 0.0, 0.0, 0.0};

	Vetor x(n);
	Matriz P0(n, n, _P0);
	Vetor z(m);

	// Estimativas iniciais
	x(0) = gps_get()->longitude; // TODO Tem que ser planificado
	x(1) = 0; // Velocidade inicial no sentido da longitude
	x(2) = gps_get()->latitude; // Tem que ser planificado
	x(3) = 0; // Velocidade incial no sentido da latitude

	filtro.init(x, P0);

	for(;;) {
		z(0) = gps_get()->longitude;
		z(1) = 0; // TODO colocar a velocidade aqui
		z(2) = gps_get()->latitude;
		z(3) = 0;

		Vetor u(1, 0.0);

		filtro.step(u, z);

		cout << filtro.getX() << endl;
	}
}
