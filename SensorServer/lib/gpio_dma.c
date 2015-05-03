#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "gpio_dma.h"

#define BCM2708_PERI_BASE	0x20000000
#define GPIO_BASE			(BCM2708_PERI_BASE + 0x200000)

#define PAGE_SIZE			(4*1024)
#define BLOCK_SIZE			(4*1024)

#define GPIO_PULL			*(gpio_dma_map+37)			// Pull up/pull down
#define GPIO_PULLCLK0		*(gpio_dma_map+38)			// Pull up/pull down clock
#define GPIO_PULLCLK1		*(gpio_dma_map+39)			// Pull up/pull down clock

volatile unsigned *gpio_dma_map;

void gpio_dma_init() {
	int mem_fd;
	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0) {
		perror("/dev/mem open");
		exit(-1);
	}
	
	gpio_dma_map = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, GPIO_BASE);
	close(mem_fd);
	
	if (gpio_dma_map == MAP_FAILED) {
		printf("mmap error %d\n", (int) gpio_dma_map);
		exit(-1);
	}
}

void gpio_dma_free() {
	munmap((void *) gpio_dma_map, BLOCK_SIZE);
}

void gpio_dma_input_with_pull(unsigned int pin, unsigned int pull_up_down) {
	static const struct timespec delay_5_us = {.tv_sec = 0, .tv_nsec = 5 * 1000};
	gpio_dma_input(pin);
	GPIO_PULL = pull_up_down;
	
	// http://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
	// Wait at least 150 cycles
	nanosleep(&delay_5_us, NULL);
	if (pin < 31)
		GPIO_PULLCLK0 = 1 << pin;
	else
		GPIO_PULLCLK1 = 1 << pin;
	nanosleep(&delay_5_us, NULL);
	
	GPIO_PULL = 0;
	GPIO_PULLCLK0 = 0;
}

void gpio_dma_enable_i2c_id() {
	gpio_dma_alt(0, 4);
	gpio_dma_alt(1, 4);
	gpio_dma_alt(28, 0);
	gpio_dma_alt(29, 0);
}
