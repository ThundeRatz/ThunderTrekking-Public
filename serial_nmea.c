#define _BSD_SOURCE
#define _GNU_SOURCE
// Tem uns includes a mais aqui
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

#include "colors.h"
#include "serial.h"

static char *commands[] = {"GGA", "GSA", "GSV", "RMC"};
enum                      { GGA,   GSA,   GSV,   RMC};

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

inline void print_err(char *msg) {
	fprintf(stderr, CRED ">>>>>>>>>>>>>>>>>>>> ERRO: %s <<<<<<<<<<<<<<<<<<<<\n" CRESET, msg);
}

// Pela especificação, mensagens têm até 80 bytes + '$' + <CR><LF>
char nmea_buf[83];

int nmea_read(int fd) {
	ssize_t len;
	try(len = read(fd, nmea_buf, sizeof(nmea_buf)),"read");
	
	if (len < 7 || nmea_buf[len - 1] != '\n' || nmea_buf[len - 2] != '\r' || nmea_buf[0] != '$') {
		print_err("NMEA - Mensagem inválida");
		return -1;
	}
	
	if (nmea_buf[len - 5] == '*') {
		// Possui checksum
		int i, sum = 0;
		
		for (i = 1; i < len - 5; i++)
			sum ^= nmea_buf[i];
		
		if (nmea_buf[len - 4] <= '9')
			nmea_buf[len - 4] -= '0';
		else
			nmea_buf[len - 4] -= 'A' - 0xa;
		
		if (nmea_buf[len - 3] <= '9')
			nmea_buf[len - 3] -= '0';
		else
			nmea_buf[len - 3] -= 'A' - 0xa;
		
		if ((sum & 0xf) != nmea_buf[len - 3] && ((sum >> 8) & 0xf) != nmea_buf[len - 4]) {
			print_err("NMEA - Checksum inválida");
			return -1;
		}
		nmea_buf[len - 5] = 0;
	} else {
		nmea_buf[len - 2] = 0;
	}
	return 0;
}

typedef struct {
	float utc;
	float latitude;
	float longitude;
	int quality;
	int satellites;
	int hor_dof;
	int antenna_altitude;
	int geoidal_separation;
	int dgps_age;
	int differential_reference;
	char antenna_altitude_unit;
	char geoidal_separation_unit;
} nmea_t;

int main() {
	const int baud = 4800;
	int fd = serial_open("/dev/ttyUSB0", &baud, O_RDWR);
	nmea_t nmea;
	
	for (;;) {
		if (nmea_read(fd)== -1)
			continue;
		if (nmea_buf[1] != 'G' || nmea_buf[2] != 'P') {
			print_err("Mensagem de dispositivo não GPS");
		} else {
			char *pos;
			
			pos = nmea_buf + 7;
			switch (command(nmea_buf + 3)) {
				char *end;
				float data_f;
				long int data_d;
				
				/**************************************** GGA ****************************************/
				case GGA:
				printf("GGA\n");
				printf("\t%s\n", nmea_buf);
				
				data_f = strtof(pos, &end);
				if (end != pos) {
					// Houve dados válidos
					printf("UTC %f\n", data_f);
					nmea.utc = data_f;
				} else
					printf("UTC vazio\n");
				pos = end + 1;
				
				data_f = strtof(pos, &end);
				if (end != pos) {
					printf("Lat %f\n", data_f);
					nmea.latitude = data_f;
				} else
					printf("Lat vazio\n");
				pos = end + 1;
				
				switch (*pos) {
					case 'S':
					nmea.latitude = -nmea.latitude;
					pos += 2;
					printf("Lat N\n");
					break;
					case 'N':
					pos += 2;
					printf("Lat S\n");
					break;
				}
				
				data_f = strtof(pos, &end);
				if (end != pos) {
					// Houve dados válidos
					printf("Long %f\n", data_f);
					nmea.longitude = data_f;
				} else
					printf("Long vazio\n");
				pos = end + 1;
				
				switch (*pos) {
					case 'E':
					nmea.longitude = -nmea.longitude;
					pos += 2;
					printf("Lat N\n");
					break;
					case 'W':
					pos += 2;
					printf("Lat S\n");
					break;
				}
				
				switch (*pos) {
					case '0':
					pos += 2;
					printf("No fix\n");
					break;
					case '1':
					pos += 2;
					printf("GPS fix\n");
					break;
					case '2':
					pos += 2;
					printf("Diff GPS fix\n");
					break;
				}
				
				data_d = strtol(pos, &end, 10);
				if (end != pos) {
					// Houve dados válidos
					printf("Satélites: %ld\n", data_d);
					nmea.satellites = data_d;
				} else
					printf("Sem satélites\n");
				pos = end + 1;
				
				data_f = strtof(pos, &end);
				if (end != pos) {
					// Houve dados válidos
					printf("Hor DOP %f\n", data_f);
					nmea.hor_dof = data_f;
				} else
					printf("Hor DOF vazio\n");
				pos = end + 1;
				break;
				
				/**************************************** GSA ****************************************/
				case GSA:
				printf("GSA\n");
				break;
				
				/**************************************** GSV ****************************************/
				case GSV:
				printf("GSV\n");
				break;
				
				/**************************************** RMC ****************************************/
				case RMC:
				printf("RMC\n");
				break;
				
				default:
				printf("Tipo desconhecido:\n%s\n", nmea_buf);
				
			}
		}
	}
}
