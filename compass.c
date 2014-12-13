#include <math.h>
#include <stdint.h>

double compass_orientation(int16_t x, int16_t y) {
	return atan2((double) y, (double) x);
}

double compass_diff(double target, double now) {
	double r;
	r = fmod((target - now), 2 * M_PI);
	return (r < M_PI ? r : r - 2 * M_PI);
}
