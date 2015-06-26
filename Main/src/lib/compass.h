#ifndef __COMPASS_H__
#define __COMPASS_H__

#include <cstdint>

double compass_orientation(int16_t x, int16_t y);
double compass_diff(double target, double now);

#endif
