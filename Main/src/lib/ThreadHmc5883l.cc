#include <iostream>
#include <cstring>
#include <stdint.h>

#include "udp_receiver.h"
#include "ports.h"
#include "compass.h"

double direcao_atual = 0;

void __attribute__((noreturn)) hmc5883l_thread() {
	char error[32];
	int udp_socket;
	int16_t data[3];

	if ((udp_socket = udp_receiver_init(UDP_HMC5883L, sizeof(data))) == -1) {
		std::cerr << "hmc5883l_thread - udp_sender_init: " <<strerror_r(errno, error, sizeof error) << std::endl;
		exit(-1);
	}

	for (;;) {
		switch (udp_receiver_recv(udp_socket, data, sizeof(data))) {
			case sizeof(data):
			direcao_atual = compass_orientation(data[0], data[2]);
			break;

			case -1:
			std::cerr << "recvfrom: " << strerror_r(errno, error, sizeof error) << std::endl;
			break;

			default:
			std::cout << "Unexpected message size\n";
			break;
		}
	}
}
