#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <linux/i2c-dev.h>

int i2c_open(int dev_n, uint8_t addr) {
	int file, status;
	char dev[40];
	long funcs;
	sprintf(dev, "/dev/i2c-%d", dev_n);
	
	file = open(dev, O_RDWR);
	if (file < 0) {
		perror("open");
		return file;
	}
	
	ioctl(file, I2C_FUNCS, &funcs);
	if (funcs & I2C_FUNC_10BIT_ADDR)
		printf("I2Cinfo - 10 bit addresses\n");
	
	if (funcs & I2C_FUNC_SMBUS_PEC)
		printf("I2Cinfo - Packet error checking\n");
	
	if (funcs & I2C_FUNC_I2C)
		printf("I2Cinfo - Combined read/write\n");
	
	if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)) {
		printf("No write byte\n");
		return -1;
	}
	
	status = ioctl(file, I2C_SLAVE, addr);
	if (status < 0) {
		fprintf(stderr, "Failed to set i2c slave\n");
		perror("ioctl");
		return status;
	}
	return file;
}
