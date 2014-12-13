/***********************************************************************
 * init.c
 * Variáveis globais para o programa e função que chama funções de
 * inicialização
***********************************************************************/

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "file_lock.h"
#include "hook.h"
#include "mod_i2c.h"
#include "joystick.h"

#define MS	1000000

int js;

void init() {
	const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 500 * MS};
	if (file_lock("/tmp/trekking") == -1)
		exit(-1);
	hook_init();
	mod_i2c_create();
	
	while ((js = joystick_open("/dev/input/js0")) == -1) {
		printf("Joystick não conectado\n");
		if (nanosleep(&sleep_time, NULL) == -1)
			perror("nanosleep");
	}
}
