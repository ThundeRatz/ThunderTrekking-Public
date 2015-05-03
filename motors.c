#define _GNU_SOURCE
#include <pthread.h>

#include "i2c.h"
#include "status.h"

#define MAX_TRIES	20
#define MAX_SPEED	240

#define MS			1000000

static void *motor_thread(__attribute__((unused)) void *ignored);

static int i2c, speed_r = 0, speed_l = 0;

int motor_init() {
	pthread_t thread;
	pthread_attr_t attr;
	
	i2c = i2c_open(1, "i915 gmbus vga");
	if (i2c == -1) {
		perror("i2c_open");
		return -1;
	}
	
	if (i2c_slave(i2c, 0x69)) {
		perror("i2c_slave");
		return -1;
	}
	
	status_try(pthread_attr_init(&attr), "pthread_attr_init");
	status_try(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED), "pthread_attr_setdetachstate");
	status_try(pthread_create(&thread, NULL, motor_thread, NULL), "pthread_create");
	
	return 0;
}

void motor(int left, int right) {
	if (left > MAX_SPEED)
		speed_l = MAX_SPEED;
	else if (left < -MAX_SPEED)
		speed_l = -MAX_SPEED;
	else
		speed_l = left;
	
	if (right > MAX_SPEED)
		speed_r = MAX_SPEED;
	else if (right < -MAX_SPEED)
		speed_r = -MAX_SPEED;
	else
		speed_r = right;
}

void motor_wait() {
	if (i2c_smbus_write_byte_data(i2c, 0, speed_l < 0))
		perror("i2c_smbus_write_byte_data");
	if (i2c_smbus_write_byte_data(i2c, 1, speed_l >= 0 ? speed_l : -speed_l))
		perror("i2c_smbus_write_byte_data");
	if (i2c_smbus_write_byte_data(i2c, 2, speed_r < 0))
		perror("i2c_smbus_write_byte_data");
	if (i2c_smbus_write_byte_data(i2c, 3, speed_r >= 0 ? speed_r : -speed_r))
		perror("i2c_smbus_write_byte_data");
}

// Versão mod_i2c.c substituida por pouco mais de 10 linhas!
static void *motor_thread(__attribute__((unused)) void *ignored) {
	static const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 50 * MS};
	for (;;) {
		nanosleep(&sleep_time, NULL);
		if (i2c_smbus_write_byte_data(i2c, 0, speed_l < 0))
			perror("i2c_smbus_write_byte_data");
		if (i2c_smbus_write_byte_data(i2c, 1, speed_l >= 0 ? speed_l : -speed_l))
			perror("i2c_smbus_write_byte_data");
		if (i2c_smbus_write_byte_data(i2c, 2, speed_r < 0))
			perror("i2c_smbus_write_byte_data");
		if (i2c_smbus_write_byte_data(i2c, 3, speed_r >= 0 ? speed_r : -speed_r))
			perror("i2c_smbus_write_byte_data");
	}
	return NULL;
}
