#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "hmc5883l.h"
#include "file_lock.h"

#define MS						1000000

int main() {
	int16_t *data;
	const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 50 * MS};
	
	if (file_lock("/tmp/trekking") == -1)
		return -1;
	
	if (hmc5883l_init()) {
		printf("hmc5883l_init falhou\n");
		return -1;
	}
	
	for (;;) {
		data = hmc5883l_read();
		if (data != NULL) {
			printf("%hd\t%hd\t%hd\n", data[0], data[1], data[2]);
		}
		if (nanosleep(&sleep_time, NULL) == -1)
			perror("nanosleep");
	}
}
