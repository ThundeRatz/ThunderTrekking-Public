#include <stdint.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define i2c_write(i2c_file, i2c_reg, i2c_byte)		i2c_smbus_write_byte_data(i2c_file, i2c_reg, i2c_byte)
#define i2c_slave(i2c_file, i2c_slave)				ioctl(i2c_file, I2C_SLAVE, i2c_slave)

int i2c_open(char *dev);
