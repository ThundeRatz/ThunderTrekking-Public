#ifndef __THREAD_GPS_H__
#define __THREAD_GPS_H__

#include "gps_coord.h"
void __attribute__((noreturn)) *gps_thread(__attribute__((unused)) void *ignored);
gps_coord_t *gps_get();
void gps_set(gps_coord_t *nova, double pass);
void gps_move(double dist, double bearing);

#endif
