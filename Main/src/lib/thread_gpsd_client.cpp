#include <gps.h>
#include <easy_thread.h>

EASY_THREAD(gps_serial_thread) {
	/*
	int udp_socket;

	if ((udp_socket = udp_receiver_init(UDP_GPS, sizeof(gps_coord_t))) == -1) {
		perror("udp_sender_init");
		exit(-1);
	}

	for (;;) {
		gps_coord_t gps_novo;
		switch (udp_receiver_recv(udp_socket, &gps_novo, sizeof(gps_novo))) {
			case sizeof(gps_novo):
			gps.latitude = 0.1 * gps.latitude + 0.9 * gps_novo.latitude;
			gps.longitude = 0.1 * gps.longitude + 0.9 * gps_novo.longitude;
			break;

			case -1:
			perror("recvfrom");
			break;

			default:
			printf("Unexpected message size\n");
			break;
		}
	}
	*/
	struct gps_data_t gpsdata;
	int gps = gps_open(GPSD_SHARED_MEMORY, NULL, &gpsdata);

	if (gps == -1) {
		perror("gps_open");
		pthread_exit(NULL);

	}
	gps_stream(&gpsdata, WATCH_ENABLE | WATCH_JSON, NULL);
	for (;;) {
		const struct timespec gps_wait = {.tv_sec = 1, .tv_nsec = 0};
		nanosleep(&gps_wait, NULL);
		if (gps_read(&gpsdata) == -1) {
			perror("gps_read");
		}
		if (gpsdata.fix.mode == MODE_2D || gpsdata.fix.mode == MODE_3D)
			printf("GPSD - %lf %lf\n", gpsdata.fix.latitude, gpsdata.fix.longitude);
		else
			printf("GPSD - No fix\n");
	}
}
