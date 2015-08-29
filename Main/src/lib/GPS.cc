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
		double	phy1 = latitude,
				phy2 = to.latitude,
				delta_lat = to.latitude - latitude,
				delta_long = to.longitude - longitude;
		double a, dist_angular;

		a = haversine(delta_lat) + haversine(delta_long) * cos(phy1) * cos(phy2);
		dist_angular = 2 * atan2(sqrt(a), sqrt(1 - a));

		return EARTH_R * dist_angular;
	}

	// Forward Azimuth (angulação inicial)
	double GPS::azimuth_to(const GPS &to) {
		double	y = sin(to.longitude - longitude) * cos(to.latitude),
	  			x = cos(latitude) * sin(to.latitude) -
				sin(latitude) * cos(to.latitude) *
				cos(to.longitude - longitude);
		return atan2(y, x);
	}
#warning testar essa
	void GPS::final_position(GPS &return_position, double dist, double bearing) {
		double dist_angular = dist / EARTH_R, final_latitude;
		final_latitude = asin(sin(latitude) * cos(dist_angular) +
			cos(latitude) * sin(dist_angular) * cos(bearing));
		return_position.latitude = final_latitude;
		return_position.longitude += atan2(sin(bearing) * sin(dist_angular) * cos(latitude),
			cos(dist_angular) - sin(latitude) * sin(final_latitude));
	}
#warning testar essa
	void GPS::move_towards(double dist, double bearing) {
		double dist_angular = dist / EARTH_R, initial_latitude = latitude;
		latitude = asin(sin(latitude) * cos(dist_angular) +
			cos(latitude) * sin(dist_angular) * cos(bearing));
		longitude += atan2(sin(bearing) * sin(dist_angular) * cos(initial_latitude),
			cos(dist_angular) - sin(initial_latitude) * sin(latitude));
	}

	void GPS::move_towards(const Point& ponto) {
		double dist, angle;
		dist = sqrt(ponto.x * ponto.x + ponto.y * ponto.y);
		angle = M_PI - atan2(ponto.x, fabs(ponto.y)); // Quarto quadrante, colocar os outros depois
		this->move_towards(dist, angle);
	}

	// haversine(x) = sin(x / 2) ^ 2;
	double GPS::haversine(double a) {
		double sin_a2 = sin(a / 2);
		return sin_a2 * sin_a2;
	}

	void GPS::to_2d(Point& point, GPS& origin) {
		double distance, azimuth;
		azimuth = origin.azimuth_to(*this);
		distance = origin.distance_to(*this);
		point.x = distance * sin(azimuth);
		point.y = distance * cos(azimuth);
	}

	GPS GPS::operator/ (int a) {
		latitude /= a;
		longitude /= a;
		return *this;
	}

	GPS GPS::operator= (int a) {
		latitude = a;
		longitude = a;
		return *this;
	}
}
