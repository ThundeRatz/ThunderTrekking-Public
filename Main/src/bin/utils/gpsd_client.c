#include <stdio.h>
#include <thread_spawn.h>
#include <gps_coord.h>
#include <unistd.h>

#include "thread_gpsd_client.h"

void gps_set(gps_coord_t *new_coordinates, double pass);

int main() {
    thread_spawn(&gps_serial_thread, NULL);
    for (;;)
        pause();
}

void gps_set(gps_coord_t *new_coordinates, double __attribute__((unused)) pass) {
    printf("%.5lf %.5lf\n", new_coordinates->latitude, new_coordinates->longitude);
}
