#include <libfreenect.h>
#include <stdlib.h>
#include <stdio.h>

void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp) {
	int i;
	static int samples = 0, samples_avg = 0;
	uint16_t *depth = (uint16_t*)v_depth, avg = 0;
	for (i = 0; i < 640 * 480; i++) {
		avg += depth[i];
	}
	//avg /= 640 * 480;
	//avg /= 100;
	samples_avg += avg;
	if (++samples == 10) {
		samples = 0;
		printf("%hu\n", samples_avg);
		samples_avg = 0;
	}
}

int main() {
	freenect_context *f_ctx;
	freenect_device *f_dev;
	int nr_devices, in;
	
	if (freenect_init(&f_ctx, NULL) < 0) {
		fprintf(stderr, "freenect_init() failed\n");
		return -1;
	}

	freenect_set_log_level(f_ctx, FREENECT_LOG_DEBUG);
	freenect_select_subdevices(f_ctx, (freenect_device_flags) FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA);

	nr_devices = freenect_num_devices (f_ctx);
	printf("Number of devices found: %d\n", nr_devices);
	
	if (nr_devices < 1) {
		freenect_shutdown(f_ctx);
		return -1;
	}
	
	if (freenect_open_device(f_ctx, &f_dev, 0) < 0) {
		fprintf(stderr, "Could not open device\n");
		freenect_shutdown(f_ctx);
		return -1;
	}
	freenect_set_depth_callback(f_dev, depth_cb);
	//freenect_set_video_callback(f_dev, rgb_cb);
	//freenect_set_video_mode(f_dev, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, current_format));
	freenect_set_depth_mode(f_dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT));
	//freenect_set_video_buffer(f_dev, rgb_back);

	freenect_start_depth(f_dev);
	//freenect_start_video(f_dev);
	freenect_set_led(f_dev, LED_BLINK_RED_YELLOW);
	
	while (freenect_process_events(f_ctx) >= 0) ;
	
	freenect_set_led(f_dev, LED_OFF);
	return 0;
}

