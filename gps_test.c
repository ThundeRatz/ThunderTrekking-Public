#include <stdio.h>
#include "gps.h"

int main() {
	gps_coord_t gps_inicial, gps_final;
	
	scanf("%lg %lg %lg %lg", &gps_inicial.latitude, &gps_inicial.longitude, &gps_final.latitude, &gps_final.longitude);
	printf("Azimuth: %g° - Distância: %g km\n", azimuth(&gps_inicial, &gps_final), gps_distance(&gps_inicial, &gps_final));
	return 0;
}
