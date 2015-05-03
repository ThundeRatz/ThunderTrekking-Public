#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "serial.h"

int main() {
	const int baudrate = 2400;
	int fd = serial_open("/dev/ttyACM1", &baudrate, O_WRONLY);
	
	for (;;) {
		int motor, direction, brightness, message;
		scanf("%d%d%d", &motor, &direction, &brightness);
		message = ((!!motor) << 7) | ((!!direction) << 6) | (brightness > 0x3f ? 0x3f : brightness);
		printf("%d\n", message);
		if (write(fd, &message, 1) == -1) {
			perror("write");
			break;
		}
		fsync(fd);
	}
	return 0;
}
