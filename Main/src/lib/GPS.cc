// http://www.movable-type.co.uk/scripts/latlong.html possui explicações
// e exemplos de várias fórmulas para cálculo de deslocamentos, ângulos,
// menores distâncias e outros em coordenadas esféricas

#include <cmath>

#include "GPS.hh"

#define EARTH_R			6371
#define TO_DEGREES(x)	((x) * 180. / M_PI)
#define TO_RAD(x)		((x) * M_PI / 180.)

namespace Trekking {
	GPS::GPS() {}
	GPS::GPS(double latitude, double longitude) : latitude(latitude), longitude(longitude) {}

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
	double GPS::distance_to(const GPS &to) {
		double	phy1 = TO_RAD(latitude),
				phy2 = TO_RAD(to.latitude),
				delta_lat = TO_RAD(to.latitude - latitude),
				delta_long = TO_RAD(to.longitude - longitude);
		double a, dist_angular;

		a = haversine(delta_lat) + haversine(delta_long) * cos(phy1) * cos(phy2);
		dist_angular = 2 * atan2(sqrt(a), sqrt(1 - a));

		return EARTH_R * dist_angular;
	}

	// Forward Azimuth (angulação inicial)
	double GPS::azimuth_to(const GPS &to) {
		double	y = sin(TO_RAD(to.longitude) - TO_RAD(longitude)) * cos(TO_RAD(to.latitude)),
				x = cos(TO_RAD(latitude)) * sin(TO_RAD(to.latitude)) -
				sin(TO_RAD(latitude)) * cos(TO_RAD(to.latitude)) *
				cos(TO_RAD(to.longitude) - TO_RAD(longitude));
		return atan2(y, x);
	}
#warning testar essa
	void GPS::final_position(GPS &return_position, double dist, double bearing) {
		double dist_angular = dist / EARTH_R, final_latitude;
		final_latitude = asin(sin(TO_RAD(latitude)) * cos(dist_angular) +
			cos(TO_RAD(latitude)) * sin(dist_angular) * cos(bearing));
		return_position.latitude = TO_DEGREES(final_latitude);
		return_position.longitude += TO_DEGREES(atan2(sin(bearing) * sin(dist_angular) * cos(TO_RAD(latitude)),
			cos(dist_angular) - sin(TO_RAD(latitude)) * sin(final_latitude)));
	}
#warning testar essa
	void GPS::move_towards(double dist, double bearing) {
		double dist_angular = dist / EARTH_R, initial_latitude = TO_RAD(latitude);
		latitude = asin(sin(TO_RAD(latitude)) * cos(dist_angular) +
			cos(TO_RAD(latitude)) * sin(dist_angular) * cos(bearing));
		longitude += TO_DEGREES(atan2(sin(bearing) * sin(dist_angular) * cos(initial_latitude),
			cos(dist_angular) - sin(initial_latitude) * sin(TO_RAD(latitude))));
		latitude = TO_DEGREES(latitude);
	}

	// haversine(x) = sin(x / 2) ^ 2;
	double GPS::haversine(double a) {
		double sin_a2 = sin(a / 2);
		return sin_a2 * sin_a2;
	}
}
