#include <iostream>
#include <iomanip>

#include "GPS.hh"

#include <stdio.h>
#include <time.h>
#include "ports.h"
#include "compass.h"
#include "thread_spawn.h"
#include "thread_hmc5883l.h"
#include "thread_gps.h"

#define MS	1000000
#define len(array)	((&array)[1] - array)

using namespace std;

Trekking::GPS soma, media;
static int n;
static const struct timespec gps_wait = {.tv_sec = 1, .tv_nsec = 0 * MS};

Trekking::GPS testes[] = {
	Trekking::GPS(-23.55392375, -46.72891395),  //Mesa elétrica
	Trekking::GPS(-23.55387477, -46.72899905),  //Interruptor
	Trekking::GPS(-23.55344336, -46.72915281),  //Hidrante
	//{.latitude = -23.64701287, .longitude = -46.57263634},
};

int main()
{
	cout << fixed << setprecision(8);

	int i;
	double angle, dist;
	thread_spawn(gps_thread, NULL);
	thread_spawn(hmc5883l_thread, NULL);

	// Media das medidas do GPS na posicao atual
	nanosleep(&gps_wait, NULL);
	nanosleep(&gps_wait, NULL);
	nanosleep(&gps_wait, NULL);
	soma = 0;
	for (n = 1; n <= 15; n++) {
		soma.latitude += gps_get()->latitude;
		soma.longitude += gps_get()->longitude;
		nanosleep(&gps_wait, NULL);
		cout << "Posicao atual: " << gps_get()->latitude << " " << gps_get()->longitude << "\n";
	}
	media = soma / (n-1);
	cout << "Posicao atual media: " << media.latitude << " " << media.longitude << "\n";

	dist = testes[0].distance_to(media); //eventos[0].pos.latitude //eventos[0].pos.longitude
	cout << "Diferenca calculada: " << dist << "\n";
	angle = media.azimuth_to(testes[0]);
	cout << "Angulo: " << angle << "\n";

	for (i = 0; i < len(testes); i++)         // Desloca todas as coordenadas
		testes[i].move_towards(dist, angle);   //eventos[i].pos.latitude

	nanosleep(&gps_wait, NULL);
	nanosleep(&gps_wait, NULL);
	for (i = 0; i < len(testes); i++) {
		cout << "--- Posicao " << i << "\n"
		     << "Coordenadas: "<< testes[i].latitude << ", " << testes[i].longitude << "\n"; //eventos[i].pos.latitude, eventos[i].pos.longitude
		     //<< "Azimuth: " << azimuth(gps_get(), &testes[i]) << "\n" //&eventos[i].pos)
		     //<< "Distancia: " << gps_distance(gps_get(), &testes[i]) << "\n"
		     //<< "Direção atual: ", direcao_atual << "\n"
		     //<< "Diff " << compass_diff(azimuth(gps_get(), &testes[i]), direcao_atual) << "\n" //&eventos[i].pos)*/
	}
	return 0;
}
