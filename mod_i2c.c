//pthread_mutex_t mod_i2c = PTHREAD_MUTEX_INITIALIZER;
#include "i2c.h"
#define I2C_DEV_COUNT	1
static uint8_t devices[I2C_DEV_COUNT];
static int fd;
void *mod_i2c_create(void *args) {
	uint8_t *target_devices = args;
	int i;
	
	for (i = 0; i < I2C_DEV_COUNT; i++) {
		devices[i] = i2c_open(1, target_devices[i]);
		if (devices[i] == -1)
			exit(-1);
	}
	
	
}
