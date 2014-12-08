#ifndef __COMPASS_H__
#define __COMPASS_H__

struct compass {
	int16_t x, y, z;
};

//void compass_init();
void compass_read(struct compass *c);
double compass_orientation();
double compass_diff(double target, double now);

#endif
