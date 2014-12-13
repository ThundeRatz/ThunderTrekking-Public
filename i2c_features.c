#include <stdio.h>
#include <i2c.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int fd, dev_n;
	
	if (argc > 1)
		sscanf(argv[1], "%d", &dev_n);
	else {
		printf("i2c device file: ");
		scanf("%d", &dev_n);
	}
	fd = i2c_open(dev_n);
	
	if (fd < 0)
		printf("i2c_open failed\n");
	else
		close(fd);
	return 0;
}
