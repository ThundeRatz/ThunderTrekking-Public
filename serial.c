#define _BSD_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#define OPTIONS	"b:d:h"
#define nitens(list)	1[&list] - list
#define try(cmd, msg)	do {if ((cmd) == -1) {perror(msg); exit(-1);}} while (0)

// Ver man 3 termios para as flags para terminais

/*
 * Possíveis constantes de baud:
 * B0
 * B50
 * B75
 * B110
 * B134
 * B150
 * B200
 * B300
 * B600
 * B1200
 * B1800
 * B2400
 * B4800
 * B9600
 * B19200
 * B38400
 * B57600
 * B115200
 * B230400
 * Tem valores consecutivos (http://www.delorie.com/djgpp/doc/incs/termios.h). Uma busca binária em uma array com os valores retorna o valor certo da constante.
 */

static const unsigned int baud_list[] = {0, 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400};

static int cmp_uints(const void *cmp1, const void *cmp2) {
	unsigned int n1 = *(int *) cmp1, n2 = *(int *) cmp2;
	return n1 - n2;
}

inline static int baud_bsearch(const int *baud) {
	unsigned int *found = NULL;
	
	found = bsearch(baud, &baud_list, nitens(baud_list), sizeof(baud_list[0]), cmp_uints);
	if (!found)
		return -1;
	return found - baud_list;
}

/***********************************************************************
 * serial_open
 * Abre um dispositivo serial. O driver para terminais do Linux suporta
 * várias flags e configurações, essa função abre:
 * -Com baud de entrada e saída iguais
 * -Sem checagem de paridade
 * -Sem processamento de saída ou geração de sinais
 * -Transmissão de 8 bits
 * -Modo canônico
 **********************************************************************/
int serial_open(char *dev, const int *baud, int flags) {
	struct termios tty;
	int fd, speed;
	
	try(fd = open(dev, flags| O_NOCTTY), "serial_open - open");
	
	memset(&tty, 0, sizeof(tty));
	
	// Baud de entrada/saída
	if ((speed = baud_bsearch(baud)) == -1) {
		fprintf(stderr, "serial_open - Invalid baud rate\n");
	}
	cfsetospeed(&tty, (speed_t) speed);
	cfsetispeed(&tty, (speed_t) speed);
	
	// Sem paridade
	tty.c_iflag = IGNPAR;
	
	// Sem processamento adicional da saída
	// (ver manual, há flags para conversão entre convenções como NL -> CRNL automáticas)
	tty.c_oflag = 0;
	
	// 8 bits de dados, mas a especificação diz que o bit 7 é sempre 0 para NMEA
	tty.c_cflag |= (CS8 | CLOCAL | CREAD);
	
	// VMIN e VTIME setam número mínimo de caracteres para ler antes de retornar read
	// e timeout
	
	// Modo canônico
	tty.c_lflag = ICANON;
	tcflush(fd, TCIFLUSH);
	try(tcsetattr(fd, TCSAFLUSH, &tty), "serial_open - tcsetattr");
	return fd;
}
