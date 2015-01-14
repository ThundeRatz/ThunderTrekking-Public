#include <math.h>
#include <stdint.h>

#define X_OFFSET	22.5
#define Y_OFFSET	-131

double compass_orientation(int16_t x, int16_t y) {
	return atan2((double) y - Y_OFFSET, (double) x - X_OFFSET);
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
