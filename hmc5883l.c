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
#include "mod_i2c.h"

// 32 bytes é o tamanho máximo retornado por i2c_smbus_read_block_data
// pela especificação
static int16_t i2c_block[16];

int hmc5883l_init() {
	// Checa ID:
	if (mod_i2c_read(REG_HMC_ID_A) != HMC_ID_A_CHAR || mod_i2c_read(REG_HMC_ID_B) != HMC_ID_B_CHAR ||
			mod_i2c_read(REG_HMC_ID_C) != HMC_ID_C_CHAR) {
		fprintf(stderr, "hmc5883l_init: Device ID mismatch\n");
		return -1;
	}
	
	mod_i2c_write_now(REG_HMC_CONFIGURATION_A, HMC_SAMPLES_AVG_1 | HMC_OUTPUT_7_5HZ | HMC_NORMAL);
	// 1090 é o padrão e não precisa ser setado. Outros valores podem ser escolhidos aqui:
	// mod_i2c_write_force(REG_HMC_CONFIGURATION_B, HMC_GAIN_1090);
	mod_i2c_write_now(REG_HMC_MODE, HMC_CONTINUOUS_MEASUREMENT);
	return 0;
}

#warning Block transfer aqui
int16_t *hmc5883l_read() {
	if (mod_i2c_read(REG_HMC_STATUS) & HMC_READY) {
		i2c_block[0] = mod_i2c_read_word(HMC_DATA_OUT_X_MSB);
		i2c_block[1] = mod_i2c_read_word(HMC_DATA_OUT_Z_MSB);
		i2c_block[2] = mod_i2c_read_word(HMC_DATA_OUT_Y_MSB);
		return i2c_block;
	}
	// Sem dados novos
	return NULL;
}
