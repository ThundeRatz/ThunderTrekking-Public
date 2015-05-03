// Ver http://www.cs.cmu.edu/afs/cs/Web/People/justinca/code/tsip_manual_rev_c.pdf
// https://code.google.com/p/doubango/source/browse/branches/2.0/doubango/tinySIP/src/tsip.c?spec=svn845&r=845
// https://ccrma.stanford.edu/courses/250a-fall-2003/docs/avrlib-new/tsip_8c-source.html
// https://github.com/cswaim/gps/blob/master/src/lib/tsip.cpp
// https://github.com/cswaim/gps/blob/master/src/lib/gps_main.cpp
// https://github.com/cswaim/gps
// O protocolo TSIP é muito flexível e configurável. Podemos mexer com o receptor se
// tivermos problemas, mas as configurações padrões já são muito boas.
// Configurações atuais:
// DGPS Position Fix Mode - padrão (auto)
// Elevation Mask/altitude mínima padrões - Elevation Mask de 15º ao invés de 10º é recomendado se precisão é necessária
// Dynamics Code - Land
// SNR Mask (Noise) - padrão
// PDOP (Position Dilution of Precision) Mask/Switch - Padrão
// Minimum Projection Flag - OFF (Mudar isso)

// Pág. 56 - Command Packets


// Pág. 198 - Report Packets
#define GPS_TIME_REPORT					0x41
#define SINGLE_PRECISION_POSITION_FIX	0x42
#define FIRMWARE_INFORMATION_REPORT		0x45
#define RECEIVER_HEALTH_REPORT			0x46
#define SINGLE_PRECISION_LLA_POSITION_FIX_REPORT	0x4a
#define MANUAL_2D_REFERENCE_ALTITUDE_REPORT	0x4a
#define ADDITIONAL_STATUS_REPORT		0x4b
#define NO_FAST_RATE_NOTIFICATION		0X7d	// sid = 0x7f
#define DIFFERENTIAL_POSITION_FIX_MODE_REPORT	0x82
#define DOUBLE_PRECISION_XYZ_POSITION_FIX	0x83
#define DOUBLE_PRECISION_LLA_POSITION_FIX	0x84
#define VERSION_2_RTCM_REPORT_NOTIFICATION	0x87	// sid = 0x08
#define RECEIVER_IDENTITY_REPORT		0x8f						



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
#define try(cmd, msg)		do {if ((cmd) == -1) {perror(msg); exit(-1);}} while (0)

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

inline static int baud_bsearch(unsigned int *baud) {
	unsigned int *found = NULL;
	
	found = bsearch(baud, &baud_list, nitens(baud_list), sizeof(baud_list[0]), cmp_uints);
	if (!found)
		return -1;
	return found - baud_list;
}

int serial_init(char *dev) {
	struct termios tty;
	int fd;
	// https://github.com/cswaim/gps/blob/master/src/lib/tsip.cpp
	try(fd = open(dev, O_RDWR| O_NOCTTY), "open");
	
	memset(&tty, 0, sizeof(tty));
	
	// Baud de entrada/saída
	cfsetospeed(&tty, B9600);
	cfsetispeed(&tty, B9600);
	
#warning Ativar paridade
	tty.c_iflag = IGNPAR;
	
	// Sem processamento adicional da saída
	// (ver manual, há flags para conversão entre convenções como NL -> CRNL automáticas)
	tty.c_oflag = 0;
	
	// Ver http://www.delorie.com/gnu/docs/glibc/libc_364.html - conexão local
	// e entrada de dados
	tty.c_cflag |= (CS8 | CLOCAL | CREAD);
	
	// VMIN e VTIME setam número mínimo de caracteres para ler antes de retornar read
	// e timeout
	
	// Sem modo canônico e alguns caracteres especiais
	tty.c_lflag = 0;
	
	try(tcsetattr(fd, TCSANOW, &tty), "tcsetattr");
	return fd;
}

int nmea_init(char *dev) {
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

#define DLE	0x10	// DLE ID = Start packet
#define ETX	0x03	// DLE ETX = End packet
// Para não causar conflito com DLE ETX, todos os bytes 0x10 enviados na série de dados
// são duplicados para 0x10 0x10 e devem ser filtrados pelo receptor dos dados.
// End packet é definido como ETX precedido por um número ímpar de DLE

typedef struct {
	uint8_t start;
	uint8_t id;
	uint8_t data[1];
} tsip_packet_t;

typedef struct {
	uint8_t start;
	uint8_t id, sid;
	uint8_t data[1];
} tsip_subpacket_t;

typedef struct {
	uint8_t *data;
	size_t end, msg_end, len, maxlen;
	// end = fim dos dados úteis (sem DLE ETX)
	// msg_end = fim dos dados (incluindo DLE ETX)
	// len = quantidade de bytes no buffer
	// maxlen = tamanho máximo do buffer
} tsip_data_t;

void tsip_read(int fd, tsip_data_t *tsip) {
	int i, dle_count = 0, inicio = 0;
	ssize_t status;
	
	start:
	memmove(tsip->data, tsip->data + tsip->msg_end, tsip->len - tsip->msg_end);
	tsip->msg_end = 0;
	tsip->len -= tsip->msg_end;
	
	for (;;) {
		for (i = 0; i < tsip->len; i++) {
			if (tsip->data[i] == DLE)
				dle_count++;
			else  {
				if (dle_count & 1) {		// número ímpar de DLE
					if (tsip->data[i] == ETX) {
						// Fim do pacote
						tsip->msg_end = i;
						return;
					} else {
						printf("Inicio\n");
						tsip->msg_end = i;
						if (inicio)
							return;
						// Início do pacote
						inicio = 1;
						goto start;
					}
				}
				dle_count = 0;
			}
		}
		try(status = read(fd, tsip->data + tsip->len, tsip->maxlen - tsip->len), "read");
		if (!status) {
			if (tsip->len == tsip->maxlen) {
				fprintf(stderr, ">>>>>TSIP: Buffer cheio<<<<<\n");
				for (i = 0; i < tsip->len; i++) {
					printf("%.2x ", tsip->data[i]);
				}
				return;
			} else {
				// Sem dados. Setar c_cc[VMIN] em termios faz read() esperar dados antes de retornar
				return;
				//fprintf(stderr, "TSIP: read() retornou 0 bytes, tentando novamente...\n");
			}
		} else {
			tsip->len += status;
		}
	}
}

int main() {
	int fd = serial_init("/dev/ttyAMA0"), i;
	tsip_data_t tsip;
	tsip_packet_t *packet;
	
	int dle_count=0;
	for (;;) {
		char data;
		if (read(fd, &data, 1) == 1) {
			putchar(data);
			//printf("%.2x\t", data);
			if (data == DLE)
				dle_count++;
			else if (dle_count & 1) {
				if (data == ETX)
					printf("End\n");
				else
					printf("Start\n");
			}
		}
	}
	
	
	tsip.data = malloc(9000);
	tsip.maxlen = 9000;
	tsip.len = 0;
	tsip.msg_end = 0;
	for (;;) {
		char buf[1000];
		
		tsip_read(fd, &tsip);
		
		packet = (tsip_packet_t *) tsip.data;
		
		switch (packet->id) {
			case GPS_TIME_REPORT:
			printf("GPS_TIME_REPORT\n");
			break;
			case SINGLE_PRECISION_POSITION_FIX:
			printf("SINGLE_PRECISION_POSITION_FIX\n");
			break;
			case FIRMWARE_INFORMATION_REPORT:
			printf("FIRMWARE_INFORMATION_REPORT\n");
			break;
			case RECEIVER_HEALTH_REPORT:
			printf("RECEIVER_HEALTH_REPORT\n");
			break;
			case SINGLE_PRECISION_LLA_POSITION_FIX_REPORT:
			printf("SINGLE_PRECISION_LLA_POSITION_FIX_REPORT or\nMANUAL_2D_REFERENCE_ALTITUDE_REPORT");
			break;
			case ADDITIONAL_STATUS_REPORT:
			printf("ADDITIONAL_STATUS_REPORT\n");
			break;
			case NO_FAST_RATE_NOTIFICATION:
			printf("NO_FAST_RATE_NOTIFICATION\n");
			break;
			case DIFFERENTIAL_POSITION_FIX_MODE_REPORT:
			printf("DIFFERENTIAL_POSITION_FIX_MODE_REPORT\n");
			break;
			case DOUBLE_PRECISION_XYZ_POSITION_FIX:
			printf("DOUBLE_PRECISION_XYZ_POSITION_FIX\n");
			break;
			case DOUBLE_PRECISION_LLA_POSITION_FIX:
			printf("DOUBLE_PRECISION_LLA_POSITION_FIX\n");
			break;
			case VERSION_2_RTCM_REPORT_NOTIFICATION:
			printf("VERSION_2_RTCM_REPORT_NOTIFICATION\n");
			break;
			case RECEIVER_IDENTITY_REPORT:
			printf("RECEIVER_IDENTITY_REPORT\n");
			break;
			default:
			printf("Packet id %u\n", packet->id);
			break;
			
		}
		break;
		/*
		for (i = 0; i < len; i++) {
			printf("%.2x\t", buf[i]);
		}
		*/
		
	}
	free(tsip.data);
}
