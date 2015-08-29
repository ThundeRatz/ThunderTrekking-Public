#include <stdio.h>

#include "GPS.hh"

int main() {
	Trekking::GPS from, to;

	while (scanf("%lf%lf%lf%lf", &from.latitude, &from.longitude, &to.latitude, &to.longitude) == 4)
		printf("%lf\n", from.distance_to(to));
	return 0;
}
