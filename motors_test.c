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

static void sig_handler(__attribute__((unused)) int signo) {
}

int main() {
	struct sigaction sinal;
	
	memset((char *) &sinal, 0, sizeof(sinal));
	sinal.sa_handler = sig_handler;
	if (sigaction(SIGINT, &sinal, NULL) == -1) {
		perror("sigaction");
		return -1;
	}
	
	if (file_lock("/tmp/trekking") == -1)
		return -1;
	
	mod_i2c_create();
	motor(255, -255);
	printf("Rodando até SIGINT...\n");
	pause();
	printf("Parando...\n");
	motor(0, 0);
#warning TODO: i2c write wait
	sleep(1);
	return 0;
}

