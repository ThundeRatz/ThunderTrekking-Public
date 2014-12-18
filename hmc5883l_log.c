#undef DEBUG
#define _POSIX_C_SOURCE		199309L

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include "hmc5883l.h"
#include "hmc5883l_registers.h"
#include "file_lock.h"
#include "compass.h"
#include "mod_i2c.h"
#include "motors.h"

#define MS					1000000

static int running = 1;
static void sig_handler(int signo) {
	if (signo == SIGINT)
		running = 0;
}

int main() {
	int16_t *data;
	// Manter sleep pequeno para manter a timer preciso
	const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 900 * MS};
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
	if (hmc5883l_init()) {
		printf("hmc5883l_init falhou\n");
		return -1;
	}
	
	printf(	"HMC5883L logger\n"
			"Gera log de saídas da bússola na saída padrão.\n"
			"A saída pode ser visualizada com algum programa que plote gráficos\n"
			"ou processada para calcular deslocamentos/rotações para corrigir o\n"
			"campo magnético recebido da bússola\n");
	motor(255, -255);
	while (running) {
		struct timespec ts;
		data = hmc5883l_read();
		if (data != NULL) {
			if (clock_gettime(CLOCK_MONOTONIC, &ts)) {
				perror("clock_gettime");
			}
			printf("%ld.%.9ld\t%hd\t%hd\t%hd\n", ts.tv_sec, ts.tv_nsec, data[0], data[1], data[2]);
		}
		if (nanosleep(&sleep_time, NULL) == -1)
			perror("nanosleep");
	}
	
	mod_i2c_destroy();
	return 0;
}
