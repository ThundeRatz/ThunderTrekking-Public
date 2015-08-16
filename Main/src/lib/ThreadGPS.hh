#pragma once

#include "gps_coord.h"

void __attribute__((noreturn)) gps_thread();
gps_coord_t *gps_get();
void gps_set(gps_coord_t *nova, double pass);
void gps_move(double dist, double bearing);
