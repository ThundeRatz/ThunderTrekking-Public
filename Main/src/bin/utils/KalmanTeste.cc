#include <iostream>
#include <iomanip>
#include <cmath>

#include "TrekkingKF.hh"
#include "GPS.hh"

#define MS	1000000

using namespace std;
using namespace Trekking;

static const struct timespec cycle = {.tv_sec = 0, .tv_nsec = 100 * MS};

int main () {
	cout << fixed << setprecision(8);

	TrekkingKF filtro;
	GPS origem, atual;

	const int n = 4;
	const int m = 4;

	const double _P0[] = {100.0*100.0, 0.0, 0.0, 0.0,  // TODO Ver esses valores
						  0.0, 10.0, 0.0, 0.0,
						  0.0, 0.0, 100.0*100.0, 0.0,
						  0.0, 0.0, 0.0, 10.0};

	Vetor x(n);
	Matriz P0(n, n, _P0);
	Vetor z(m);

	origem.latitude = gps_get()->latitude;   // eventos[0]
	origem.longitude = gps_get()->longitude; // eventos[0]

	// Estimativas iniciais
	x(0) = posicao.y;
	x(1) = 0.0; // Velocidade inicial no sentido da longitude
	x(2) = posicao.x;
	x(3) = 0.0; // Velocidade incial no sentido da latitude

	filtro.init(x, P0);
	cout << filtro.getX() << endl;

	for(;;) {
		atual.latitude = gps_get()->latitude;
		atual.longitude = gps_get()->longitude;

		atual.to_2d(posicao, origem);

		z(0) = posicao.y;
		z(1) = 0.0; // TODO colocar a velocidade aqui
		z(2) = posicao.x;
		z(3) = 0.0;

		Vetor u(1, 0.0);

		filtro.step(u, z);

		cout << filtro.getX() << endl;
		origem.move_towards(posicao);
		atual = origem;

		cout << "  " << atual.longitude << "                "
			 << atual.latitude << endl;

		if (nanosleep(&cycle, NULL))
			cerr << "nanosleep" << endl;
	}
}
