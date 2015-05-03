/***********************************************************************
 * Driver para a bússola HMC5883L
 * O datasheet é bem curto e há apenas 3 registradores para escrita.
 * A função de configuração é bem curta também.
 * 
 * Notas:
 * Pela especificação, nessa bússola há um ponteiro interno indicando
 * o endereço desejado de leitura. Quando uma leitura acaba, o endereço
 * é incrementado. Para ler o próximo byte, podemos pedir o byte sem
 * enviar o endereço (usar i2c_smbus_read_byte ao invés de
 * i2c_smbus_write_byte_data) para reduzir o uso do bus.
 * 
 * É possível atingir frequências de até 160Hz usando Single Measurement
 * e checando DRDY ou até 75Hz usando Continuous Measurement.
 * 
 * Para manter sincronia entre os valores X, Y e Z lidos, quando um
 * registrador Data Output é lido, todos devem ser lidos para que outros
 * valores sejam escritos. RDY e DRDY também param se ser atualizados.
 * O bit LOCK do registrador de status indica se o dispositivo está travado
 * e é setado se estiver no meio de uma leitura (apenas parte dos registradores
 * foi lida) ou se o registrador de modo foi lido. Para resetá-lo, é
 * necessário realizar um desses:
 * -Ler os seis bytes de saída
 * -Mudar o registrador de modo
 * -Mudar a configuração da medição (Configuration Register A)
 * -Resetar o dispositivo
 * 
 * Usando i2c_smbus_read_block_data podemos ler os 6 bytes de saída de uma
 * vez.
 * 
 * Sobre a implementação:
 * O buffer de leitura da bússola é estático, não chamar hmc5883l_read()
 * de threads diferentes.
 **********************************************************************/

#include <stdio.h>
#include "hmc5883l_registers.h"
#include "i2c.h"

#define I2C_BUS		0

// 32 bytes é o tamanho máximo retornado por i2c_smbus_read_block_data
// pela especificação
static int16_t i2c_block[16];
static int i2c;

int hmc5883l_init() {
#if I2C_BUS == 0
	if ((i2c = i2c_open(0, "20205000.i2c")) == -1) {
#elif I2C_BUS == 1
	if ((i2c = i2c_open(1, "20804000.i2c")) == -1) {
#endif
		perror("i2c_open");
		return -1;
	}
	
	if (i2c_slave(i2c, 0x1e)) {
		perror("i2c_slave");
		return -1;
	}
	
	// Checa ID:
	if (i2c_smbus_read_byte_data(i2c, HMC_ID_A) != HMC_ID_A_CHAR ||
		i2c_smbus_read_byte_data(i2c, HMC_ID_B) != HMC_ID_B_CHAR ||
		i2c_smbus_read_byte_data(i2c, HMC_ID_C) != HMC_ID_C_CHAR) {
		fprintf(stderr, "hmc5883l_init: ID errado\n");
		return -1;
	}
	
	if (i2c_smbus_write_byte_data(i2c, HMC_CONFIGURATION_A, HMC_SAMPLES_AVG_2 | HMC_OUTPUT_75HZ | HMC_NORMAL) ||
		i2c_smbus_write_byte_data(i2c, HMC_CONFIGURATION_B, HMC_GAIN_820) ||
		i2c_smbus_write_byte_data(i2c, HMC_MODE, HMC_CONTINUOUS_MEASUREMENT)) {
		perror("i2c_smbus_write_byte_data");
		return -1;
	}
	return 0;
}

int16_t *hmc5883l_read() {
	int32_t status = i2c_smbus_read_byte_data(i2c, HMC_STATUS);
	
	if (status & HMC_READY) {
		status = i2c_smbus_read_word_data(i2c, HMC_DATA_OUT_X_MSB);
		if (status == -1) {
			perror("i2c_smbus_read_word_data");
			return NULL;
		}
		i2c_block[0] = status << 8 | ((status >> 8) & 0xff);
		
		status = i2c_smbus_read_word_data(i2c, HMC_DATA_OUT_Z_MSB);
		if (status == -1) {
			perror("i2c_smbus_read_word_data");
			return NULL;
		}
		i2c_block[1] = status << 8 | ((status >> 8) & 0xff);
		
		status = i2c_smbus_read_word_data(i2c, HMC_DATA_OUT_Y_MSB);
		if (status == -1) {
			perror("i2c_smbus_read_word_data");
			return NULL;
		}
		i2c_block[2] = status << 8 | ((status >> 8) & 0xff);
		
		if (i2c_block[0] == HMC_OVERFLOW || i2c_block[1] == HMC_OVERFLOW ||
			i2c_block[2] == HMC_OVERFLOW) {
			fprintf(stderr, "hmc5883l_read: Overflow\n");
			return NULL;
		}
		
		return i2c_block;
	}
	
	// Sem dados novos
	return NULL;
}
