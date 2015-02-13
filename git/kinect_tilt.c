#include <libfreenect.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	freenect_context *f_ctx;
	freenect_device *f_dev;
	int nr_devices, in;
	
	if (freenect_init(&f_ctx, NULL) < 0) {
		fprintf(stderr, "freenect_init() failed\n");
		return -1;
	}

	freenect_set_log_level(f_ctx, FREENECT_LOG_DEBUG);
	freenect_select_subdevices(f_ctx, (freenect_device_flags) FREENECT_DEVICE_MOTOR);

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
	
	freenect_set_led(f_dev, LED_BLINK_RED_YELLOW);
	
	while (scanf("%d", &in)) {
		if (in > 30)
			in = 30;
		if (in < -30)
			in = -30;
		freenect_set_tilt_degs(f_dev, in);
	}
	
	freenect_set_led(f_dev, LED_OFF);

	return 0;
}
