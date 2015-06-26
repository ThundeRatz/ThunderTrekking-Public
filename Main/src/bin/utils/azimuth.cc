#include <stdio.h>

#include "gps_coord.h"

int main() {
	gps_coord_t from, to;
	
	while (scanf("%lf%lf%lf%lf", &from.latitude, &from.longitude, &to.latitude, &to.longitude) == 4)
		printf("%lf\n", azimuth(&from, &to));
	return 0;
}
