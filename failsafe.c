#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

#include "motors.h"
#include "file_lock.h"
#include "mod_i2c.h"

int main() {
	int fd;
	for (;;) {
		if ((fd = file_lock("/tmp/trekking")) != -1) {
			printf("failsafe: Parando...\n");
			mod_i2c_create();
			motor_init();
#warning WAIT aqui
			sleep(1);
			file_unlock(fd);
			return 0;
		}
		sleep(1);
	}
}
