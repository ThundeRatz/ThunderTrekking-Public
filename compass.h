#ifndef __COMPASS_H__
#define __COMPASS_H__

double compass_orientation();
double compass_diff(double target, double now);
double pass_filter(double last, double now, double pass);

#endif
