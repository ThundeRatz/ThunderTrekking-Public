#include <iostream>
#include <cstring>
#include <mutex>

#include "ThreadSpawn.hh"

#include "udp_receiver.h"
#include "ports.h"
#include "gps_coord.h"

using namespace std;

static gps_coord_t gps = {.latitude = 0., .longitude = 0.};
mutex gps_mutex;

void __attribute__((noreturn)) gps_thread() {
	int udp_socket;

	if ((udp_socket = udp_receiver_init(UDP_GPS_CELL, sizeof(gps_coord_t))) == -1) {
		cerr << "udp_receiver_init: " << strerror(errno) << endl;
		exit(-1);
	}

	for (;;) {
		gps_coord_t gps_novo;
		switch (udp_receiver_recv(udp_socket, &gps_novo, sizeof(gps_novo))) {
			case sizeof(gps_novo):
			if (gps.latitude != 0 || gps.longitude != 0) {
				gps.latitude = 0.2 * gps.latitude + 0.8 * gps_novo.latitude;
				gps.longitude = 0.2 * gps.longitude + 0.8 * gps_novo.longitude;
			} else {
				gps.latitude = gps_novo.latitude;
				gps.longitude =  gps_novo.longitude;
			}
			break;

			case -1:
			cerr << "recvfrom: " << strerror(errno) << endl;
			break;

			default:
			cout << "Unexpected message size\n";
			break;
		}
	}
}

gps_coord_t * gps_get() {
	return &gps;
}

void gps_set(gps_coord_t *nova, double pass) {
	try {
		gps_mutex.lock();
		if (gps.latitude == 0.) {
			gps.latitude = nova->latitude;
			gps.longitude = nova->longitude;
		} else {
			gps.latitude = gps.latitude * (1 - pass) + pass * nova->latitude;
			gps.longitude = gps.longitude * (1 - pass) + pass * nova->longitude;
		}
		gps_mutex.unlock();
	} catch (system_error& e) {
		cerr << "GPS Mutex: " << e.what() << endl;
	}
}

void gps_move(double dist, double bearing) {
	final_position(&gps, dist, bearing);
}
