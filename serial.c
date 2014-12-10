#define _BSD_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <getopt.h>
#include <unistd.h>
#include <stdint.h>
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

static char *commands[] = {"GSV"};
enum                      {GSV};

static int compare_command(const void *cmp1, const void *cmp2) {
	return strncmp((const char *) cmp1, *(const char **) cmp2, 3);
}

int command(char *cmd) {
	char **found = NULL;
	
	if (!(cmd)[0] || !(cmd)[1] || !(cmd)[2])
		return -1;
	
	found = bsearch(cmd, &commands, nitens(commands), sizeof(commands[0]), compare_command);
	if (!found)
		return -1;
	return found - commands;
}

static int cmp_uints(const void *cmp1, const void *cmp2) {
	unsigned int n1 = *(int *) cmp1, n2 = *(int *) cmp2;
	return n1 - n2;
}

inline static int baud_bsearch(unsigned int *baud) {
	unsigned int *found = NULL;
	
	found = bsearch(baud, &baud_list, nitens(baud_list), sizeof(baud_list[0]), cmp_uints);
	if (!found)
		return -1;
	return found - baud_list;
}

int nmea_open(char *dev) {
	struct termios tty;
	int fd;
	// https://github.com/cswaim/gps/blob/master/src/lib/tsip.cpp
	try(fd = open(dev, O_RDWR| O_NOCTTY), "open");
	
	memset(&tty, 0, sizeof(tty));
	
	// Baud de entrada/saída
	cfsetospeed(&tty, B4800);
	cfsetispeed(&tty, B4800);
	
	// Não há paridade em NMEA
	tty.c_iflag = IGNPAR;
	
	// Sem processamento adicional da saída
	// (ver manual, há flags para conversão entre convenções como NL -> CRNL automáticas)
	tty.c_oflag = 0;
	
	// 8 bits de dados, mas a especificação diz que o bit 7 é sempre 0
	tty.c_cflag |= (CS8 | CLOCAL | CREAD);
	
	// VMIN e VTIME setam número mínimo de caracteres para ler antes de retornar read
	// e timeout
	
	// Sem modo canônico e alguns caracteres especiais
	tty.c_lflag = 0;
	
	try(tcsetattr(fd, TCSANOW, &tty), "tcsetattr");
	return fd;
}

// Pela especificação, mensagens têm até 80 bytes + '$' + <CR><LF>
char nmea_buf[83];

// Pode ser otimizado
char nmea_byte(int fd) {
	char data;
	ssize_t status;
	do {
		status = read(fd, &data, 1);
		if (status == -1){
			perror("NMEA - read");
			exit(1);
		}
	} while (!status);
	return data;
}

void nmea_read(int fd) {
	int i;
	char c;
	
	while (nmea_byte(fd) != '$');
	
	for (i = 0; i < (int) sizeof(nmea_buf); i++) {
		c = nmea_byte(fd);
		if (c == '\r') {
			if (nmea_byte(fd) != '\n') {
				fprintf(stderr, "NMEA - Invalid Message\n");
				exit(1);
			}
			nmea_buf[i] = 0;
			return;
		}
		nmea_buf[i] = c;
	}
	
	fprintf(stderr, "NMEA - Too long message\n");
	exit(1);
}

int nmea_init(int fd) {
	char data;
	ssize_t status;
	
	for (;;) {
		status = read(fd, &data, 1);
		if (status == -1) {
			perror("NMEA - read");
			return -1;
		}
		if (status && data == '$')
			return 0;
	}
}

int main() {
	int fd = nmea_open("/dev/ttyAMA0");
	
	if (nmea_init(fd) == -1)
		return -1;
	
	for (;;) {
		nmea_read(fd);
		printf("NMEA\t%s\n", nmea_buf);
		
		if (nmea_buf[0] != 'G' || nmea_buf[1] != 'P') {
			printf("Dispositivo não é GPS\n");
		} else {
			switch (command(nmea_buf + 2)) {
				case GSV:
				printf("GSV\n");
				break;
				
			}
		}
	}
}
