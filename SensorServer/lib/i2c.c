// https://www.kernel.org/doc/Documentation/i2c/dev-interface
// https://www.kernel.org/doc/Documentation/i2c/smbus-protocol
// https://www.kernel.org/doc/Documentation/i2c/functionality
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>
#include <linux/i2c.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"

#define I2C_CHECK_FEATURE(functions, feature)		printf("%32s | %s\n", #feature, (functions & feature) ? "YES" : "NO")

/***********************************************************************
 * i2c_open
 * Abre dispositivo i2c em /dev/i2c-<dev_n> e retorna fd.
 * dev_name não é necessário para a abertura, mas é usado para checar se
 * abrimos o adaptador correto. Usando NULL não há checagem.
***********************************************************************/
int i2c_open(int dev_n, char *dev_name) {
	int file;
	char dev[60], sys[60], name[60];
	long funcs;
	FILE *sys_f;
	
	if (dev_name != NULL) {
		sprintf(sys, "/sys/class/i2c-dev/i2c-%d/name", dev_n);
		sys_f = fopen(sys, "r");
		if (sys_f != NULL) {
			size_t len;
			fgets(name, sizeof(name), sys_f);
			fclose(sys_f);
			len = strlen(name);
			if (name[len - 1] == '\n')
				name[len - 1] = 0;
			if (dev_name != NULL && strcmp(name, dev_name)) {
				fprintf(stderr, "i2c_open: Falha na checagem do nome do adaptador\nEsperado %s, encontrou %s\n", dev_name, name);
				abort();
			} else
				LOG("Adaptador i2c: %s\n", name);
		} else {
			fprintf(stderr, "i2c_open: Falha ao ler nome do adaptador\n");
			abort();
		}
	}
	
	sprintf(dev, "/dev/i2c-%d", dev_n);
	file = open(dev, O_RDWR);
	if (file < 0) {
		perror("i2c open");
		return file;
	}
	
	if (ioctl(file, I2C_FUNCS, &funcs) == -1) {
		perror("i2c - ioctl I2C_FUNCS");
		close(file);
		return -1;
	}
	
#ifdef DEBUG
	printf("I2C features:\n");
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
#endif
	
	return file;
}
