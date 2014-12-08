#include <stdio.h>
#include <i2c.h>
#include <unistd.h>

int main() {
	char dev[50];
	int fd;
	printf("i2c device file: ");
	scanf("%s", dev);
	
	fd = i2c_open(dev);
	if (fd < 0)
		printf("i2c_open failed\n");
	else
		close(fd);
	return 0;
}
