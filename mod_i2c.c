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

#warning TODO: função que escreve no i2c indepentendemente do valor antigo
#warning (para watchdog, por exemplo)

#define _POSIX_C_SOURCE 	199309L
#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "i2c.h"
#include "simple_queue.h"
#define I2C_DEV			"/dev/i2c-1"
#define I2C_DEV_COUNT	2

#define MOTORS_I2C_ADDR			0x69
#define LEDS_I2C_ADDR			0x24

#define MS						1000000

// Funções i2c e pthread retornam o código de erro ao invés de setar errno. As macros são para tornar o código mais curto
// Mostra __msg e o a string de erro __errno
#define status_perror(__msg, __errno)	do {if (__errno) {char __errmsg[50]; fprintf(stderr, "%s: %s\n", __msg, strerror_r(__errno, __errmsg, sizeof(__errmsg)));}} while (0)
// Executa cmd. Se retornar erro, chama status_perror e retorna da função
#define status_try(cmd, errmsg)			do {int __errno = (cmd); if (__errno) {pthread_perror(errmsg, __errno); return;}} while (0)
// Executa cmd. Se houver erro, chama abort()
#define status_abort(cmd, errmsg)		do {int __errno = (cmd); if (__errno) {pthread_perror(errmsg, __errno); abort();}} while (0)

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
	{.dev = LEDS_I2C_ADDR, .reg = 6, .next = NOT_QUEUED}		// TIMESTEP
};

static uint8_t devices[I2C_DEV_COUNT] = {MOTORS_I2C_ADDR, LEDS_I2C_ADDR};
static int fd, queue_last = -1;
static pthread_mutex_t i2c_lock = PTHREAD_MUTEX_INITIALIZER, i2c_available_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t i2c_available = PTHREAD_COND_INITIALIZER;
static const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 5 * MS};
void mod_i2c_create() {
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
}

void mod_i2c_write(int reg, uint8_t value) {
	// se estamos escrevendo os mesmos dados que estão no buffer, ignorar
	if (targets[reg].value.byte == value)
		return;
	// pega o lock
	status_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
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
}

#warning Adicionar checagem de erro (seria bom tocar alarme ou fazer algo se o i2c morrer de vez)
#warning Usar timedwait, escrever qualquer coisa de tempos em tempos e ativar watchdog no uC
#warning i2c block transfer
static void *mod_i2c_thread(__attribute__((unused)) void *ignored) {
	// mutex para espera de condição. Como essa é a única thread esperando i2c_available, ficamos com o mutex
	// durante toda a execução do programa (ele é pego e solto fora do loop). Se outras threads esperassem a
	// condição que há dados para escrever no i2c, teríamos que soltar o lock a cada iteração.
	status_abort(pthread_mutex_lock(&i2c_available_mutex), "pthread_mutex_lock");
	for (;;) {
		status_abort(pthread_cond_wait(&i2c_available, &i2c_available_mutex), "pthread_cond_wait");
		
		// se foi sinalizado que há dados, escreve enquanto fila possui itens
		while (queue_last != NOT_QUEUED) {
			struct i2c_packet *next;
			int i2c_status;
			pthread_abort(pthread_mutex_lock(&i2c_lock), "pthread_mutex_lock");
			// essa checagem é desnecessária por sermos a única thread retirando itens da fila
			// (se haviam itens antes de adquirir o lock ainda há itens), mas o programa poderia
			// quebrar se alguém o mudasse no futuro.
			if (queue_last == NOT_QUEUED)
				continue;
			next = &targets[targets[queue_last].next];
			
			do {
				while (i2c_slave(fd, next->dev) == -1)
					perror("ioctl");
				i2c_status = i2c_smbus_write_byte_data(fd, next->reg, next->value.byte);
				status_perror("i2c_smbus_write_byte_data", i2c_status);
			} while (i2c_status);
			
			if (queue_last == targets[queue_last].next)
				queue_last = -1;
			else
				queue_last = targets[targets[queue_last].next].next;
			targets[targets[queue_last].next].next = -1;
			// solta o mutex antes de esperar um pouco entre escritas
			pthread_abort(pthread_mutex_unlock(&i2c_lock), "pthread_mutex_unlock");
			
			while (nanosleep(&sleep_time, NULL) == -1)
				perror("nanosleep");
		}
	}
	// nunca vai chegar aqui, mas se a thread puder ser morta e reiniciada, libere o mutex antes
	// (ou a próxima thread vai travar esperando o mutex)
	status_abort(pthread_mutex_unlock(&i2c_available_mutex), "pthread_mutex_unlock");
}
