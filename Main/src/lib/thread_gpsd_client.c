#include <gps.h>
#include <thread_gps.h>
#include <easy_thread.h>

EASY_THREAD(gps_serial_thread) {
	struct gps_data_t gpsdata;
	int gps = gps_open("localhost", DEFAULT_GPSD_PORT, &gpsdata);

	if (gps == -1) {
		// usar gps_errstr aqui
		pthread_exit(NULL);
	}

	if (gps_stream(&gpsdata, WATCH_ENABLE | WATCH_JSON, NULL)) {
		perror("gps_stream");
		pthread_exit(NULL);
	}

	for (;;) {
		if (gps_read(&gpsdata) == -1) {
			perror("gps_read");
		} else if (gpsdata.fix.mode == MODE_2D || gpsdata.fix.mode == MODE_3D) {
			gps_coord_t gps = {.latitude = gpsdata.fix.latitude, .longitude = gpsdata.fix.longitude};
			gps_set(&gps, 0.8);
		} else
			printf("GPSD - No fix\n");
	}
}
