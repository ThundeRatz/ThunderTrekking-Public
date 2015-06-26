#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "udp_receiver.h"
#include "ports.h"
#include "pid.h"
#include "compass.h"

double direcao_atual = 0;

void __attribute__((noreturn)) *hmc5883l_thread(__attribute__((unused)) void *ignored) {
	int udp_socket;
	int16_t data[3];
	pid_correction_t pid;
	
	pid_init(&pid, 5, 2, 1);
	
	if ((udp_socket = udp_receiver_init(UDP_HMC5883L, sizeof(data))) == -1) {
		perror("hmc5883l_thread - udp_sender_init");
		exit(-1);
	}
	
	for (;;) {
		switch (udp_receiver_recv(udp_socket, data, sizeof(data))) {
			case sizeof(data):
			direcao_atual = pass_filter(direcao_atual, compass_orientation(data[0], data[2]), 0.8);
			break;
			
			case -1:
			perror("recvfrom");
			break;
			
			default:
			printf("Unexpected message size\n");
			break;
		}
	}
}
