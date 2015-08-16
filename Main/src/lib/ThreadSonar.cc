#include <iostream>
#include <cstring>
#include <cstdint>

#include "udp_receiver.h"
#include "ports.h"
#include "udp_packets.h"

using namespace std;

uint64_t sonar_l = 600 * 58000, sonar_r = 600 * 58000;

void __attribute__((noreturn)) sonar_thread() {
	int udp_socket;
	hc_sr04_udp_packet_t data;

	if ((udp_socket = udp_receiver_init(UDP_HC_SR04, sizeof(data))) == -1) {
		cerr << "hmc5883l_thread - udp_sender_init: " << strerror(errno) << endl;
		exit(-1);
	}

	for (;;) {
		switch (udp_receiver_recv(udp_socket, &data, sizeof(data))) {
			case sizeof(data):
			if (data.id == 0)
				sonar_l = data.distance;
			else
				sonar_r = data.distance;
			break;

			case -1:
			cerr << "recvfrom: " << strerror(errno) << endl;
			break;

			default:
			cout << "Unexpected message size\n";
			break;
		}
	}
}
