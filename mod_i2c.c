/***********************************************************************
 * mod_i2c.c
 * Módulo i2c
 * O objetivo desse arquivo é funcionar como uma interface mais alto nível
 * para o i2c. Funções de acesso ao i2c estão em i2c.c.
 * O módulo limita o acesso aos registradores i2c aos definidos em targets.
 * Quando se deseja escrever em um registrador, o módulo adiciona o comando
 * de escrita a uma pilha e escreve quando o i2c estiver livre, impedindo
 * que outras funções fiquem esperando o i2c.
 * Além disso, se a função de escrita for chamada com o mesmo valor que
 * estava no registrador, a escrita é descartada.
***********************************************************************/

#define _POSIX_C_SOURCE 	199309L
#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <linux/i2c-dev.h>
#include "i2c.h"
#include "mod_i2c.h"
#include "status.h"
#define I2C_DEV			6

#define MOTORS_I2C_ADDR			0x69
#define LEDS_I2C_ADDR			0x24
#define COMPASS_I2C_ADDR		0x1e

#define MS						1000000

#define MAX_TRIES				10

#define len(array)				((&array)[1] - array)

static void *mod_i2c_thread(__attribute__((unused)) void *ignored);

union i2c_data {
	uint8_t byte;
	uint8_t *block;
};

typedef enum {
	BYTE, BYTE_DATA, BLOCK_DATA
} i2c_type;

struct i2c_packet {
	int next;
	const uint8_t dev, reg;
	i2c_type type;
	union i2c_data value;
};

#define NOT_QUEUED	-1
static struct i2c_packet targets[] = {
	{.dev = MOTORS_I2C_ADDR, .reg = 0, .next = NOT_QUEUED},		// DIR_D
	{.dev = MOTORS_I2C_ADDR, .reg = 1, .next = NOT_QUEUED},		// VEL_D
	{.dev = MOTORS_I2C_ADDR, .reg = 2, .next = NOT_QUEUED},		// DIR_E
	{.dev = MOTORS_I2C_ADDR, .reg = 3, .next = NOT_QUEUED},		// VEL_D
	
	{.dev = LEDS_I2C_ADDR, .reg = 0, .next = NOT_QUEUED},		// R
	{.dev = LEDS_I2C_ADDR, .reg = 1, .next = NOT_QUEUED},		// G
	{.dev = LEDS_I2C_ADDR, .reg = 2, .next = NOT_QUEUED},		// B
	{.dev = LEDS_I2C_ADDR, .reg = 3, .next = NOT_QUEUED},		// LOWADC
	{.dev = LEDS_I2C_ADDR, .reg = 4, .next = NOT_QUEUED},		// HIGHADC
	{.dev = LEDS_I2C_ADDR, .reg = 5, .next = NOT_QUEUED},		// MODE
	{.dev = LEDS_I2C_ADDR, .reg = 6, .next = NOT_QUEUED},		// TIMESTEP
	
	{.dev = COMPASS_I2C_ADDR, .reg = 0, .next = NOT_QUEUED},	// REG_HMC_CONFIGURATION_A
	{.dev = COMPASS_I2C_ADDR, .reg = 1, .next = NOT_QUEUED},	// REG_HMC_CONFIGURATION_B
	{.dev = COMPASS_I2C_ADDR, .reg = 2, .next = NOT_QUEUED},	// REG_HMC_MODE
	{.dev = COMPASS_I2C_ADDR, .reg = 3, .next = NOT_QUEUED},	// REG_HMC_DATA_OUT_X_MSB
	{.dev = COMPASS_I2C_ADDR, .reg = 4, .next = NOT_QUEUED},	// REG_HMC_DATA_OUT_X_LSB
	{.dev = COMPASS_I2C_ADDR, .reg = 5, .next = NOT_QUEUED},	// REG_HMC_DATA_OUT_Z_MSB
	{.dev = COMPASS_I2C_ADDR, .reg = 6, .next = NOT_QUEUED},	// REG_HMC_DATA_OUT_Z_LSB
	{.dev = COMPASS_I2C_ADDR, .reg = 7, .next = NOT_QUEUED},	// REG_HMC_DATA_OUT_Y_MSB
	{.dev = COMPASS_I2C_ADDR, .reg = 8, .next = NOT_QUEUED},	// REG_HMC_DATA_OUT_Y_LSB
	{.dev = COMPASS_I2C_ADDR, .reg = 9, .next = NOT_QUEUED},	// REG_HMC_STATUS
	{.dev = COMPASS_I2C_ADDR, .reg = 10, .next = NOT_QUEUED},	// REG_HMC_ID_A
	{.dev = COMPASS_I2C_ADDR, .reg = 11, .next = NOT_QUEUED},	// REG_HMC_ID_B
	{.dev = COMPASS_I2C_ADDR, .reg = 12, .next = NOT_QUEUED},	// REG_HMC_ID_C
};

static int fd, queue_last = -1;
static pthread_mutex_t i2c_lock = PTHREAD_MUTEX_INITIALIZER, i2c_data_available_mutex = PTHREAD_MUTEX_INITIALIZER, i2c_init_ended_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t i2c_data_available = PTHREAD_COND_INITIALIZER, i2c_init_ended = PTHREAD_COND_INITIALIZER;

void mod_i2c_create() {
	const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 10 * MS};
	pthread_t thread;
	pthread_attr_t attr;
	/*
	int i;
	// Uma ideia se forem usados múltiplos barramentos i2c
	for (i = 0; i < I2C_BUS_COUNT; i++) {
		i2c_devices[i] = i2c_open(1, target_devices[i]);
		if (i2c_devices[i] == -1)
			exit(-1);
	}
	*/
	
	if ((fd = i2c_open(I2C_DEV)) == -1) {
		fprintf(stderr, "i2c_open %d falhou\n", I2C_DEV);
		abort();
	}
	status_abort(pthread_attr_init(&attr), "pthread_attr_init");
	status_abort(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED), "pthread_attr_setdetachstate");
	status_abort(pthread_create(&thread, NULL, mod_i2c_thread, NULL), "pthread_create");
	
	// esperamos a thread inicializar antes de retornar. Sem a espera, os primeiros comandos i2c eram
	// adicionados na pilha mas só enviados quando houvesse mais uma escrita após a inicialização
	// que gerasse o sinal para a thread. No programa, a inicialização da bússola só era enviada
	// após o próximo comando (para mover o robô) e até lá as leituras davam erro.
	status_abort(pthread_cond_wait(&i2c_init_ended, &i2c_init_ended_mutex), "pthread_cond_wait");
	
	while (pthread_mutex_trylock(&i2c_data_available_mutex) != 0) {
		printf("mod_i2c_create: esperando inicialização da thread...\n");
		if (nanosleep(&sleep_time, NULL) == -1)
			perror("nanosleep");
	}
	status_abort(pthread_mutex_unlock(&i2c_data_available_mutex), "pthread_mutex_unlock");
	
	/* O trecho abaixo foi substituído pelo spinloop acima */
	// Esperar um pouco (para não ocorrer o caso muito improvável de retornarmos e outra função chamar mod_i2c_write_*
	// antes de mod_i2c_thread chamar pthread_cond_wait)
	/*if (nanosleep(&sleep_time, NULL) == -1)
		perror("nanosleep");*/
}

static void set_slave(uint8_t slave) {
	int i;
	for (i = 0; i < MAX_TRIES; i++)
		if (i2c_slave(fd, slave) == -1)
			perror("set_slave - ioctl");
		else
			return;
	fprintf(stderr, "i2c - set_slave %x falhou %d vezes, abortando\n", (unsigned int) slave, MAX_TRIES);
	abort();
}

void mod_i2c_write_force(int reg, uint8_t value) {
	if (reg >= len(targets)) {
		fprintf(stderr, "mod_i2c_write_force: registrador %d inválido\n", reg);
		abort();
	}
	// pega o lock
	status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
	// seta byte no buffer
	targets[reg].value.byte = value;
	targets[reg].type = BYTE_DATA;
	
	// se não estiver na fila de eventos para enviar, adiciona
	// (estamos usando uma fila circular)
	if (targets[reg].next == NOT_QUEUED) {
		if (queue_last == NOT_QUEUED) {
			targets[reg].next = reg;
		} else {
			targets[reg].next = targets[queue_last].next;
			targets[queue_last].next = reg;
		}
		queue_last = reg;
	}
	
	status_abort(pthread_mutex_unlock(&i2c_lock), "pthread_mutex_unlock");
	// sinaliza que temos dados para mod_i2c_thread
	status_abort(pthread_cond_signal(&i2c_data_available), "pthread_cond_signal");
}

/***********************************************************************
 * mod_i2c_write_quick
 * Para dispositivos muito simples, envia apenas um bit no lugar de Rd/Wr.
 * Não implementei porque nada que temos usa write quick.
***********************************************************************/
/*
void mod_i2c_write_quick(int dev) {
	
	
}
*/

void mod_i2c_write(int reg, uint8_t value) {
	// se estamos escrevendo os mesmos dados que estão no buffer, ignorar
	if (targets[reg].value.byte != value)
		mod_i2c_write_force(reg, value);
}

void mod_i2c_write_now(int reg, uint8_t value) {
	int i;
	status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
	set_slave(targets[reg].dev);
	for (i = 0; i < MAX_TRIES; i++)
		if (i2c_smbus_write_byte_data(fd, targets[reg].reg, value) == -1)
			perror("i2c_smbus_write_byte_data");
		else {
			status_abort(pthread_mutex_unlock(&i2c_lock), "pthread_mutex_unlock");
			return;
		}
	
	fprintf(stderr, "i2c - mod_i2c_write_now %x.%x falhou %d vezes, abortando\n", targets[reg].dev, targets[reg].reg, MAX_TRIES);
	abort();
}

void mod_i2c_write_word_now(int reg, uint16_t value) {
	int i;
	status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
	set_slave(targets[reg].dev);
	for (i = 0; i < MAX_TRIES; i++)
		if (i2c_smbus_write_word_data(fd, targets[reg].reg, value) == -1)
			perror("i2c_smbus_write_word_data");
		else {
			status_abort(pthread_mutex_unlock(&i2c_lock), "pthread_mutex_unlock");
			return;
		}
	
	fprintf(stderr, "i2c - mod_i2c_write_word_now %x.%x falhou %d vezes, abortando\n", targets[reg].dev, targets[reg].reg, MAX_TRIES);
	abort();
}

uint8_t mod_i2c_read_direct(uint8_t dev, uint8_t reg) {
	int ret;
	status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
	set_slave(dev);
	while ((ret = i2c_smbus_read_byte_data(fd, reg)) == -1)
		status_perror("i2c_smbus_read_byte_data", ret);
	status_abort(pthread_mutex_unlock(&i2c_lock), "pthread_mutex_unlock");
	return (uint8_t) ret;
}

uint8_t mod_i2c_read(int reg) {
	int32_t ret;
	int i;
	status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
	set_slave(targets[reg].dev);
	
	for (i = 0; i < MAX_TRIES; i++)
		if ((ret = i2c_smbus_read_byte_data(fd, targets[reg].reg)) == -1)
			perror("i2c_smbus_read_byte_data");
		else {
			status_abort(pthread_mutex_unlock(&i2c_lock), "pthread_mutex_unlock");
			return (uint8_t) ret;
		}
	
	fprintf(stderr, "i2c - mod_i2c_read %x.%x falhou %d vezes, abortando\n", targets[reg].dev, targets[reg].reg, MAX_TRIES);
	abort();
}

/***********************************************************************
 * mod_i2c_read_byte
 * Isso NÃO FAZ o que parece
 * Essa função não lê um byte do reistrador reg. Ela chama i2c_smbus_read_byte
 * no dispositivo de reg. i2c_smbus_read_byte envia para o dispositivo
 * um comando para leitura de um byte sem especificar nenhum endereço.
 * O comportamento depende do dispositivo. Pela especificação i2c, a
 * função é muito usada em dispositivos que usam apenas um byte para comunicação.
 * Em outros dispositivos, geralmente retorna o byte do último endereço lido.
 * Mas a escolha do valor retornado é do dispositivo apenas. A HMC5883L,
 * por exemplo, possui comportamento diferente; ver hmc5883l.c
***********************************************************************/
uint8_t mod_i2c_read_byte(int reg) {
	int32_t ret;
	int i;
	status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
	set_slave(targets[reg].dev);
	
	for (i = 0; i < MAX_TRIES; i++) {
		if ((ret = i2c_smbus_read_byte(fd)) == -1)
			perror("i2c_smbus_read_byte");
		else {
			status_abort(pthread_mutex_unlock(&i2c_lock), "pthread_mutex_unlock");
			return (uint8_t) ret;
		}
	}
	
	fprintf(stderr, "i2c - mod_i2c_read_byte %x.%x falhou %d vezes, abortando\n", targets[reg].dev, targets[reg].reg, MAX_TRIES);
	abort();
}

uint16_t mod_i2c_read_word(int reg) {
	int32_t ret;
	int i;
	status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
	for (i = 0; i < MAX_TRIES; i++)
		if ((ret = i2c_smbus_read_word_data(fd, targets[reg].reg)) == -1)
			perror("i2c_smbus_read_word_data");
		else {
			status_abort(pthread_mutex_unlock(&i2c_lock), "pthread_mutex_unlock");
			return (uint16_t) ret;
		}
	
	fprintf(stderr, "i2c - mod_i2c_read_word %x.%x falhou %d vezes, abortando\n", targets[reg].dev, targets[reg].reg, MAX_TRIES);
	abort();
}

uint16_t mod_i2c_read_word_inv(int reg) {
	int32_t ret;
	int i;
	status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
	for (i = 0; i < MAX_TRIES; i++)
		if ((ret = i2c_smbus_read_word_data(fd, targets[reg].reg)) == -1)
			perror("i2c_smbus_read_word_data");
		else {
			status_abort(pthread_mutex_unlock(&i2c_lock), "pthread_mutex_unlock");
			return ((uint16_t) ret) >> 8 | ((uint16_t) ret << 8);
		}
	
	fprintf(stderr, "i2c - mod_i2c_read_word %x.%x falhou %d vezes, abortando\n", targets[reg].dev, targets[reg].reg, MAX_TRIES);
	abort();
}

#warning Adicionar checagem de erro (seria bom tocar alarme ou fazer algo se o i2c morrer de vez)
#warning Usar timedwait, escrever qualquer coisa de tempos em tempos e ativar watchdog no uC
#warning i2c block transfer e configurações (timeout...), sync
static void *mod_i2c_thread(__attribute__((unused)) void *ignored) {
	const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 10 * MS};
	int i;
	// mutex para espera de condição. Como essa é a única thread esperando i2c_available, ficamos com o mutex
	// durante toda a execução do programa (ele é pego e solto fora do loop). Se outras threads esperassem a
	// condição que há dados para escrever no i2c, teríamos que soltar o lock a cada iteração.
	status_abort(pthread_mutex_lock(&i2c_data_available_mutex), "pthread_mutex_lock");
	status_abort(pthread_cond_broadcast(&i2c_init_ended), "pthread_cond_broadcast");
	for (;;) {
		status_abort(pthread_cond_wait(&i2c_data_available, &i2c_data_available_mutex), "pthread_cond_wait");
		// se foi sinalizado que há dados, escreve enquanto fila possui itens
		while (queue_last != NOT_QUEUED) {
			status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
			// essa checagem é desnecessária por sermos a única thread retirando itens da fila
			// (se haviam itens antes de adquirir o lock ainda há itens), mas o programa poderia
			// quebrar se alguém o mudasse no futuro.
			if (queue_last == NOT_QUEUED)
				break;
			
			set_slave(targets[targets[queue_last].next].dev);
			if (targets[targets[queue_last].next].type == BYTE_DATA) {
				for (i = 0; i < MAX_TRIES; i++) {
					if (i2c_smbus_write_byte_data(fd, targets[targets[queue_last].next].reg, targets[targets[queue_last].next].value.byte))
						perror("i2c_smbus_write_byte_data");
					else
						break;
				}
			}
			if (i == MAX_TRIES) {
				fprintf(stderr, "i2c - mod_i2c_thread %x.%x falhou %d vezes, abortando\n", targets[targets[queue_last].next].dev, targets[targets[queue_last].next].reg, MAX_TRIES);
				abort();
			}
			
			
			if (queue_last == targets[queue_last].next) {
				targets[queue_last].next = NOT_QUEUED;
				queue_last = NOT_QUEUED;
			} else {
				int lixo = targets[queue_last].next;
				targets[queue_last].next = targets[targets[queue_last].next].next;
				targets[lixo].next = NOT_QUEUED;
			}
			
			// solta o mutex antes de esperar um pouco entre escritas
			status_abort(pthread_mutex_unlock(&i2c_lock), "pthread_mutex_unlock");
			
			if (nanosleep(&sleep_time, NULL) == -1)
				perror("nanosleep");
		}
	}
	// nunca vai chegar aqui, mas se a thread pudesse ser morta e reiniciada, libere o mutex antes
	// (ou a próxima thread vai travar esperando o mutex)
	status_abort(pthread_mutex_unlock(&i2c_data_available_mutex), "pthread_mutex_unlock");
}

// Isso é usado para ver a pilha de comandos (útil para depurar quando houverem problemas)
	/*
	int end = queue_last, now = queue_last;
	printf("mod_i2c_write QUEUE:\n");
	do {
		now = targets[now].next;
		printf("%x\t%x\n", targets[now].dev, targets[now].reg);
	} while (now != end);
	printf("\n");
	*/
