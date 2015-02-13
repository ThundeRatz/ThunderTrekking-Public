#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "hmc5883l.h"
#include "hmc5883l_registers.h"
#include "file_lock.h"
#include "compass.h"
#include "mod_i2c.h"

#define MS						1000000

int main() {
	int16_t *data;
	double inicial, atual;
	const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 1 * MS};
	
	if (file_lock("/tmp/trekking") == -1)
		return -1;
	
	mod_i2c_create();
	if (hmc5883l_init()) {
		printf("hmc5883l_init falhou\n");
		return -1;
	}
	
	data = hmc5883l_read();
	// Deveríamos checar overflow na primeira amostra também
	inicial = compass_orientation(data[0], data[2]);
	
	for (;;) {
		data = hmc5883l_read();
		if (data != NULL) {
			if (data[0] == HMC_OVERFLOW)
				printf("X: OVFLW\t");
			else
				printf("%hd\t", data[0]);
			if (data[1] == HMC_OVERFLOW)
				printf("Z: OVFLW\t");
			else
				printf("%hd\t", data[1]);
			if (data[2] == HMC_OVERFLOW)
				printf("Y: OVFLW\n");
			else
				printf("%hd\n", data[2]);
			atual = compass_orientation(data[0], data[2]);
			printf("Atual: %.4g\tDiff: %g\n", atual, compass_diff(inicial, atual));
			
		}
		if (nanosleep(&sleep_time, NULL) == -1)
			perror("nanosleep");
	}
}
