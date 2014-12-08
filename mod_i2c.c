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
#define I2C_DEV			"/dev/i2c-1"

#define MOTORS_I2C_ADDR			0x69
#define LEDS_I2C_ADDR			0x24
#define COMPASS_I2C_ADDR		0x1e

#define MS						1000000

// Funções i2c e pthread retornam o código de erro ao invés de setar errno. As macros são para tornar o código mais curto
// Mostra __msg e o a string de erro __errno
#define status_perror(__msg, __errno)	do {if (__errno) {char __errmsg[50]; fprintf(stderr, "%s: %s\n", __msg, strerror_r(__errno, __errmsg, sizeof(__errmsg)));}} while (0)
// Executa cmd. Se retornar erro, chama status_perror e retorna da função
#define status_try(cmd, errmsg)			do {int __errno = (cmd); if (__errno) {status_perror(errmsg, __errno); return;}} while (0)
// Executa cmd. Se houver erro, chama abort()
#define status_abort(cmd, errmsg)		do {int __errno = (cmd); if (__errno) {status_perror(errmsg, __errno); abort();}} while (0)

static void *mod_i2c_thread(__attribute__((unused)) void *ignored);

union i2c_data {
	uint8_t byte;
	uint8_t *block;
};

// Por enquanto, apenas transferência de bytes individuais é suportada.
// A especificação possui extensões para envio de blocos. É uma coisa a ser feita.
enum i2c_type {
	BYTE, BLOCK
};

struct i2c_packet {
	int next;
	const uint8_t dev, reg;
	// i2c_type type;
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
static pthread_mutex_t i2c_lock = PTHREAD_MUTEX_INITIALIZER, i2c_available_mutex = PTHREAD_MUTEX_INITIALIZER, i2c_init_ended_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t i2c_available = PTHREAD_COND_INITIALIZER, i2c_init_ended = PTHREAD_COND_INITIALIZER;

void mod_i2c_create() {
	const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 500 * MS};
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
	
	fd = i2c_open(I2C_DEV);
	status_abort(pthread_attr_init(&attr), "pthread_attr_init");
	status_abort(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED), "pthread_attr_setdetachstate");
	status_abort(pthread_create(&thread, NULL, mod_i2c_thread, NULL), "pthread_create");
	
	// esperamos a thread inicializar antes de retornar. Sem a espera, os primeiros comandos i2c eram
	// adicionados na pilha mas só enviados quando houvesse mais uma escrita após a inicialização
	// que gerasse o sinal para a thread. No programa, a inicialização da bússola só era enviada
	// após o próximo comando (para mover o robô) e até lá as leituras davam erro.
	status_abort(pthread_cond_wait(&i2c_init_ended, &i2c_init_ended_mutex), "pthread_cond_wait");
	
	if (nanosleep(&sleep_time, NULL) == -1)
		perror("nanosleep");
}

static void set_slave(uint8_t slave) {
	int i;
	for (i = 0; i < 10; i++)
		if (i2c_slave(fd, targets[targets[queue_last].next].dev) == -1)
			perror("set_slave - ioctl");
		else
			return;
	fprintf(stderr, "i2c - set_slave %x falhou 10 vezes, abortando\n", (unsigned int) slave);
	abort();
}

void mod_i2c_write_force(int reg, uint8_t value) {
#ifdef DEBUG
	printf("mod_i2c_write: %d = %hhu\n", reg, value);
#endif
	// pega o lock
	status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
#ifdef DEBUG
	printf("mod_i2c_write lock\n");
#endif
	// seta byte no buffer
	targets[reg].value.byte = value;
	
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
#ifdef DEBUG
	printf("mod_i2c_write unlock\n");
#endif
	// sinaliza que temos dados para mod_i2c_thread
	status_abort(pthread_cond_signal(&i2c_available), "pthread_cond_signal");
	/*
	int end = queue_last, now = queue_last;
	printf("mod_i2c_write QUEUE:\n");
	do {
		now = targets[now].next;
		printf("%x\t%x\n", targets[now].dev, targets[now].reg);
	} while (now != end);
	printf("\n");
	*/
	/*
	struct i2c_packet *next = &targets[targets[queue_last].next];
	while (i2c_slave(fd, next->dev) == -1)
	perror("ioctl");
	int i2c_status;
	do {
#ifdef DEBUG
		printf("mod_i2c_thread: i2c_slave %x: %hhu = %hhu\n", next->dev, next->reg, next->value.byte);
#endif
		i2c_status = i2c_smbus_write_byte_data(fd, next->reg, next->value.byte);
		status_perror("i2c_smbus_write_byte_data", i2c_status);
	} while (i2c_status);
	
	if (queue_last == targets[queue_last].next) {
		targets[queue_last].next = NOT_QUEUED;
		queue_last = -1;
	} else {
		queue_last = targets[targets[queue_last].next].next;
		targets[targets[queue_last].next].next = NOT_QUEUED;
	}
	*/
}

void mod_i2c_write(int reg, uint8_t value) {
	// se estamos escrevendo os mesmos dados que estão no buffer, ignorar
	if (targets[reg].value.byte != value)
		mod_i2c_write_force(reg, value);
}

uint8_t mod_i2c_read_direct(uint8_t dev, uint8_t reg) {
	int ret;
	status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
#ifdef DEBUG
	printf("mod_i2c_read_direct: peguei lock - %x - %hhu\n", dev, reg);
#endif
	set_slave(dev);
	while ((ret = i2c_smbus_read_byte_data(fd, reg)) == -1)
		status_perror("i2c_smbus_read_byte_data", ret);
	status_abort(pthread_mutex_unlock(&i2c_lock), "pthread_mutex_unlock");
	return (uint8_t) ret;
}

uint8_t mod_i2c_read(int reg) {
	int ret;
	status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
#ifdef DEBUG
	printf("mod_i2c_read: peguei lock - %x - %hhu\n", targets[reg].dev, targets[reg].reg);
#endif
	set_slave(targets[reg].dev);
	while ((ret = i2c_smbus_read_byte_data(fd, targets[reg].reg)) == -1)
		status_perror("i2c_smbus_read_byte_data", ret);
	status_abort(pthread_mutex_unlock(&i2c_lock), "pthread_mutex_unlock");
	return (uint8_t) ret;
}

#warning read_word aqui
uint16_t mod_i2c_read_word(int reg) {
	return (mod_i2c_read_direct(targets[reg].dev, targets[reg].reg) << 8) + mod_i2c_read_direct(targets[reg].dev, targets[reg].reg + 1);
}

#warning Adicionar checagem de erro (seria bom tocar alarme ou fazer algo se o i2c morrer de vez)
#warning Usar timedwait, escrever qualquer coisa de tempos em tempos e ativar watchdog no uC
#warning i2c block transfer e configurações (timeout...), sync
static void *mod_i2c_thread(__attribute__((unused)) void *ignored) {
	const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 5 * MS};
	// mutex para espera de condição. Como essa é a única thread esperando i2c_available, ficamos com o mutex
	// durante toda a execução do programa (ele é pego e solto fora do loop). Se outras threads esperassem a
	// condição que há dados para escrever no i2c, teríamos que soltar o lock a cada iteração.
	status_abort(pthread_mutex_lock(&i2c_available_mutex), "pthread_mutex_lock");
#ifdef DEBUG
	printf("mod_i2c_thread: iniciado\n");
#endif
	status_abort(pthread_cond_broadcast(&i2c_init_ended), "pthread_cond_broadcast");
	for (;;) {
		status_abort(pthread_cond_wait(&i2c_available, &i2c_available_mutex), "pthread_cond_wait");
#ifdef DEBUG
		printf("mod_i2c_thread: sinalizado\n");
#endif
		// se foi sinalizado que há dados, escreve enquanto fila possui itens
		while (queue_last != NOT_QUEUED) {
			int i2c_status;
			status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
			// essa checagem é desnecessária por sermos a única thread retirando itens da fila
			// (se haviam itens antes de adquirir o lock ainda há itens), mas o programa poderia
			// quebrar se alguém o mudasse no futuro.
			if (queue_last == NOT_QUEUED)
				break;
			
#ifdef DEBUG
			int end = queue_last, now = queue_last;
			printf("mod_i2c_thread QUEUE:\n");
			do {
				now = targets[now].next;
				printf("%x\t%x\n", targets[now].dev, targets[now].reg);
			} while (now != end);
			printf("\n");
#endif
			
			set_slave(targets[targets[queue_last].next].dev);
			do {
#ifdef DEBUG
				printf("mod_i2c_thread: i2c_slave %x: %hhu = %hhu\n", targets[targets[queue_last].next].dev, targets[targets[queue_last].next].reg, targets[targets[queue_last].next].value.byte);
#endif
				i2c_status = i2c_smbus_write_byte_data(fd, targets[targets[queue_last].next].reg, targets[targets[queue_last].next].value.byte);
				status_perror("i2c_smbus_write_byte_data", i2c_status);
			} while (i2c_status);
			
			
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
	// nunca vai chegar aqui, mas se a thread puder ser morta e reiniciada, libere o mutex antes
	// (ou a próxima thread vai travar esperando o mutex)
	status_abort(pthread_mutex_unlock(&i2c_available_mutex), "pthread_mutex_unlock");
}
