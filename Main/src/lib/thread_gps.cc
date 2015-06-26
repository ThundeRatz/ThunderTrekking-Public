#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#include "thread_spawn.h"
#include "udp_receiver.h"
#include "ports.h"
#include "gps_coord.h"

#define status_perror(__msg, __errno)	do {if ((__errno)) {char __errmsg[50]; fprintf(stderr, "%s: %s\n", __msg, strerror_r(__errno, __errmsg, sizeof(__errmsg)));}} while (0)

static gps_coord_t gps = {.latitude = 0., .longitude = 0.};

void __attribute__((noreturn)) *gps_thread(__attribute__((unused)) void *ignored) {
	int udp_socket;

	if ((udp_socket = udp_receiver_init(UDP_GPS_CELL, sizeof(gps_coord_t))) == -1) {
		perror("udp_sender_init");
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
			perror("recvfrom");
			break;

			default:
			printf("Unexpected message size\n");
			break;
		}
	}
}

gps_coord_t * gps_get() {
	return &gps;
}

void gps_set(gps_coord_t *nova, double pass) {
	static pthread_mutex_t gps_mutex = PTHREAD_MUTEX_INITIALIZER;
	status_perror("pthread_mutex_lock", pthread_mutex_lock(&gps_mutex));
	if (gps.latitude == 0.) {
		gps.latitude = nova->latitude;
		gps.longitude = nova->longitude;
	} else {
		gps.latitude = gps.latitude * (1 - pass) + pass * nova->latitude;
		gps.longitude = gps.longitude * (1 - pass) + pass * nova->longitude;
	}
	status_perror("pthread_mutex_unlock", pthread_mutex_unlock(&gps_mutex));
}

void gps_move(double dist, double bearing) {
	final_position(&gps, dist, bearing);
}
