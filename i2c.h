#include <stdint.h>
#include <linux/i2c-dev.h>

#define i2c_write(i2c_file, i2c_reg, i2c_byte)		i2c_smbus_write_byte_data(i2c_file, i2c_reg, i2c_byte)

int i2c_open(int dev_n, uint8_t addr);
