// http://www.movable-type.co.uk/scripts/latlong.html possui explicações
// e exemplos de várias fórmulas para cálculo de deslocamentos, ângulos,
// menores distância e outros em coordenadas esféricas

#include <math.h>

#include "gps.h"

#define EARTH_R			6371
#define TO_DEGREES(x)	((x) * 180. / M_PI)
#define TO_RAD(x)		((x) * M_PI / 180.)

// haversine(x) = sin(x / 2) ^ 2;
static double haversine(double a) {
	double sin_a2 = sin(a / 2);
	return sin_a2 * sin_a2;
}


// Cálculo pelo método haversine.
// O uso da lei esférica dos cossenos também é possível, mas não é
// recomendado para pequenas distâncias
// (ver http://gis.stackexchange.com/questions/4906/why-is-law-of-cosines-more-preferable-than-haversine-when-calculating-distance-b)
// Apesar de mais simples, cos(x) é muito próximo de 1 com x muito próximo
// de 0. Com distâncias próximas, a multiplicação dos 3 cossenos é ~1 e
// somando com o valor dos senos pode ser >1 por erro de aredondamento,
// gerando erro ao usar acos. Os resultados válidos divergem
// consideravelmente do método haversine (ver link).
// O uso de haversine (definido acima) evita erros de precisão.
double gps_distance(const gps_coord_t *from, const gps_coord_t *to) {
	double	phy1 = TO_RAD(from->latitude),
			phy2 = TO_RAD(to->latitude),
			delta_lat = TO_RAD(to->latitude - from->latitude),
			delta_long = TO_RAD(to->longitude - from->longitude);
	double a, dist_angular;
	
	a = haversine(delta_lat) + haversine(delta_long) * cos(phy1) * cos(phy2);
	dist_angular = 2 * atan2(sqrt(a), sqrt(1 - a));
	
	return EARTH_R * dist_angular;
}

// Forward Azimuth (angulação inicial)
double azimuth(const gps_coord_t *from, const gps_coord_t *to) {
	double	y = sin(to->longitude - from->longitude) * cos(to->longitude),
			x = cos(from->latitude) * sin(to->latitude) - sin(from->latitude) * cos(to->latitude) * cos(to->longitude - from->longitude);
	return atan2(y, x);
}
