#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

#include "motors.h"

int file_lock(char *tmpfile) {
	int fd;
	fd = open(tmpfile, O_RDONLY | O_CREAT, S_IRUSR);
	if (fd == -1)
		return -1;
	if (flock(fd, LOCK_EX) == -1)) {
		close(fd);
		return -1;
	}
	return fd;
}

void file_unlock(int fd) {
	// Desnecessário (lock é removido ao fechar o arquivo)
	flock(fd, LOCK_UN);
	close(fd);
}

int main() {
	int fd;
	for (;;) {
		sleep(1);
		while ((fd = file_lock("/tmp/trekking"))) ;
		
		if (motors_init() == -1) {
			perror("motors_init");
			file_unlock(fd);
			continue;
		}
		
		if (motor(0, 0) == -1)
			perror("motor");
		motor_free();
		file_unlock(fd);
	}
}
