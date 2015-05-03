#include <gps.h>
#include <easy_thread.h>

EASY_THREAD(gps_serial_thread) {
	struct gps_data_t gpsdata;
	int gps = gps_open("localhost", DEFAULT_GPSD_PORT, &gpsdata);

	if (gps == -1) {
		perror("gps_open");
		pthread_exit(NULL);
	}

	if (gps_stream(&gpsdata, WATCH_ENABLE | WATCH_JSON, NULL)) {
		perror("gps_stream");
		pthread_exit(NULL);
	}

	for (;;) {
		//const struct timespec gps_wait = {.tv_sec = 1, .tv_nsec = 0};
		//nanosleep(&gps_wait, NULL);
		if (gps_read(&gpsdata) == -1) {
			perror("gps_read");
		} else if (gpsdata.fix.mode == MODE_2D || gpsdata.fix.mode == MODE_3D)
			printf("GPSD - %lf %lf\n", gpsdata.fix.latitude, gpsdata.fix.longitude);
		else
			printf("GPSD - No fix\n");
	}
}
