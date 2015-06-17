#include <iostream>

#include "GPSMonitor.hh"

using namespace std;

int main() {
    Trekking::GPSMonitor gps;
    for (;;) {
        gps.blocking_update();
		cout << gps.gpsd_data->dev.path << std::endl;
    }
}
