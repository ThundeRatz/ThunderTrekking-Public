#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctime>

#include "GPS.hh"
#include "GPS.hh"

#include "compass.h"

#define MS	1000000
#define len(array)	((&array)[1] - array)

using namespace std;

static const struct timespec gps_wait = {.tv_sec = 1, .tv_nsec = 0 * MS};

Trekking::GPS testes[] = {
	Trekking::GPS(-23.55392375, -46.72891395),  //Mesa elétrica
	Trekking::GPS(-23.55387477, -46.72899905),  //Interruptor
	Trekking::GPS(-23.55344336, -46.72915281),  //Hidrante
	//{.latitude = -23.64701287, .longitude = -46.57263634},
};

int main() {
	cout << fixed << setprecision(8);

	double angle, dist;

	// Media das medidas do GPS na posicao atual
	Trekking::GPSStats stats;
	Trekking::GPSMonitor position;
	for (int n = 1; n <= 15; n++) {
		while (!position.blocking_update()) ;
		cout << "Posicao atual: " << position.latitude << " " << position.longitude << "\n";
		stats.sample(position);
	}
	cout << "Posicao atual media: " << stats.latitude_stats().mean() << " " << stats.longitude_stats().mean() << "\n";

	return 0;
}
