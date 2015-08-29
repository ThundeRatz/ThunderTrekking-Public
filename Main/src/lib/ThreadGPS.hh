#pragma once

#include "GPS.hh"

void __attribute__((noreturn)) gps_thread();
Trekking::GPS& gps_get();
void gps_set(Trekking::GPS& nova, double pass);
void gps_move(double dist, double bearing);
