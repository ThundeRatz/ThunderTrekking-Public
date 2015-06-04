#include <math.h>
#include <stdint.h>

#define X_OFFSET	-55
#define Y_OFFSET	+75
#define ROTATION	M_PI

double compass_orientation(int16_t x, int16_t y) {
	return fmod(atan2((double) y + Y_OFFSET, (double) x + X_OFFSET) + ROTATION, 2 * M_PI);
}

double compass_diff(double target, double now) {
	double r;
	r = fmod((target - now), 2 * M_PI);
	if (r < -M_PI) {
		r += 2 * M_PI;
		return r;
	}
	if (r > M_PI)
		r -= 2 * M_PI;
	return r;
	//return (r <= M_PI ? r : r - 2 * M_PI);
}
