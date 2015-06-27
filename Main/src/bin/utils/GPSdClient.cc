#include <iostream>
#include <cstdio>
#include <ctime>

#include "GPSMonitor.hh"

using namespace std;

const char *format_string(const char *string) {
    static const char string_null[] = "NULL";
    static const char string_empty[] = "EMPTY";
    if (string == NULL)
        return string_null;
    if (*string == 0)
        return string_empty;
    return string;
}

int main() {
    Trekking::GPSMonitor gps;
    for (;;) {
        if (!gps.blocking_update()) {
            if (gps.gpsd_data == NULL)
                cout << "NULL gpsd_data" << endl;
            else {
                cout << "dev {" << "\n\t"
    		        << "path: " << format_string(gps.gpsd_data->dev.path) << "\n\t"
                    << "flags: " << gps.gpsd_data->dev.flags << " (1 = SEEN_GPS, 2 = SEEN_RTCM2, 4 = SEEN_RTCM2, 8 = SEEN_AIS)" << "\n\t"
                    << "driver: " << format_string(gps.gpsd_data->dev.driver) << "\n\t"
                    << "subtype: " << format_string(gps.gpsd_data->dev.subtype) << "\n\t"
                    << "activated: " << gps.gpsd_data->dev.activated << "\n\t"
                    << "baud: " << gps.gpsd_data->dev.baudrate << "\n\t"
                    << "stopbits: " << gps.gpsd_data->dev.stopbits << "\n\t"
                    << "parity: " << gps.gpsd_data->dev.parity << "\n\t"
                    << "cicle/mincicle: " << gps.gpsd_data->dev.cycle << "/" << gps.gpsd_data->dev.mincycle << "\n\t"
                    << "native: " << gps.gpsd_data->dev.driver_mode << "\n"
                << "}" << "\n";

                cout << "fix {" << "\n\t"
                    << "time: " << ctime((time_t *) &gps.gpsd_data->fix.time) << "\n\t"
                    << "mode: " << gps.gpsd_data->fix.mode << " {MODE_NOT_SEEN, MODE_NO_FIX, MODE_2D, MODE_3D}" << "\n\t"
                << "}" << "\n";
            }
        } else {
            const struct timespec sleep_time = {.tv_sec = 1, .tv_nsec = 0};
            cout << "blocking_update error" << endl;
            if (nanosleep(&sleep_time, NULL))
                perror("nanosleep");
        }
    }
}
