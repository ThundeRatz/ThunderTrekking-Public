#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include "gpio_dma.h"

static int fd;

typedef struct {
	int pin, value;
} pin_data;

void gpio_dma_init() {
	if ((fd = open("/dev/pind", O_RDWR)) < 0) {
		perror("/dev/pind open");
		exit(-1);
	}
}

void gpio_dma_free() {
	close(fd);
}

void gpio_dma_set(unsigned int pin, unsigned int value) {
	pin_data data;
	data.pin = pin;
	data.value = value;
	write(fd, &data, sizeof(data));
}

uint8_t gpio_dma_get(unsigned int pin) {
	uint8_t data;
	if (read(fd, &data, 1) == -1) {
		perror("gpio_dma_get - read");
		exit(-1);
	}
	return data;
}
