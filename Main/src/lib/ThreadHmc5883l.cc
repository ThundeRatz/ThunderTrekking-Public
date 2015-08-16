#include <iostream>
#include <cstring>
#include <stdint.h>

#include "udp_receiver.h"
#include "ports.h"
#include "compass.h"

double direcao_atual = 0;

void __attribute__((noreturn)) hmc5883l_thread() {
	int udp_socket;
	int16_t data[3];

	if ((udp_socket = udp_receiver_init(UDP_HMC5883L, sizeof(data))) == -1) {
		std::cerr << "hmc5883l_thread - udp_sender_init: " << strerror(errno) << std::endl;
		exit(-1);
	}

	for (;;) {
		switch (udp_receiver_recv(udp_socket, data, sizeof(data))) {
			case sizeof(data):
			direcao_atual = compass_orientation(data[0], data[2]);
			break;

			case -1:
			std::cerr << "recvfrom: " << strerror(errno) << std::endl;
			break;

			default:
			std::cout << "Unexpected message size\n";
			break;
		}
	}
}
