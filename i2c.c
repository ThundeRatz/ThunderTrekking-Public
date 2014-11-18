// https://www.kernel.org/doc/Documentation/i2c/dev-interface
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#define I2C_CHECK_FEATURE(functions, feature)		printf("i2c features: %s%s\n", (functions & feature) ? "" : "no ", #feature)

int i2c_open(char *dev) {
	int file, status;
	//char dev[40];
	long funcs;
	//sprintf(dev, "/dev/i2c-%d", dev_n);
	
	file = open(dev, O_RDWR);
	if (file < 0) {
		perror("open");
		return file;
	}
	
	if (ioctl(file, I2C_FUNCS, &funcs) == -1) {
		perror("ioctl");
		close(file);
		return -1;
	}
	
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_10BIT_ADDR);	// suporta endereços de 10 bits
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_PEC);	// packet error checking
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_I2C);			// combined read/write
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_PROTOCOL_MANGLING);
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_NOSTART);
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_BLOCK_PROC_CALL);
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_QUICK);
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_READ_BYTE);		// ou
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_WRITE_BYTE);	// I2C_FUNC_SMBUS_BYTE
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_READ_BYTE_DATA);
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_WRITE_BYTE_DATA);
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_READ_WORD_DATA);
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_WRITE_WORD_DATA);
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_PROC_CALL);
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_READ_BLOCK_DATA);
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_WRITE_BLOCK_DATA);
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_READ_I2C_BLOCK);
	I2C_CHECK_FEATURE(funcs, I2C_FUNC_SMBUS_WRITE_I2C_BLOCK);
	
	return file;
}
