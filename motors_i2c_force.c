#undef DEBUG
#define _POSIX_C_SOURCE		199309L

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "hmc5883l.h"
#include "hmc5883l_registers.h"
#include "file_lock.h"
#include "compass.h"
#include "mod_i2c.h"
#include "motors.h"

#define MS	1000000

int running = 1;

static void sig_handler(__attribute__((unused)) int signo) {
	running = 0;
}

int main() {
	const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 4 * MS};
	struct sigaction sinal;
	int speed = 0, aumentando = 1;
	
	memset((char *) &sinal, 0, sizeof(sinal));
	sinal.sa_handler = sig_handler;
	if (sigaction(SIGINT, &sinal, NULL) == -1) {
		perror("sigaction");
		return -1;
	}
	
	if (file_lock("/tmp/trekking") == -1)
		return -1;
	
	mod_i2c_create();
	
	while (running) {
		if (aumentando) {
			if (speed == 255)
				aumentando = 0;
			else
				speed++;
		} else {
			if (speed == -255)
				aumentando = 1;
			else
				speed--;
		}
		
		motor(speed, -speed);
		nanosleep(&sleep_time, NULL);
	}
	motor(0, 0);
#warning TODO: i2c write wait
	return 0;
}

