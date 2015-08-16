#include <iostream>
#include <cstring>

#include "udp_receiver.h"
#include "ports.h"

using namespace std;

float sensor_contato = 999;

void __attribute__((noreturn)) proximity_thread() {
	int udp_socket;
	udp_socket = udp_receiver_init(UDP_PROXIMITY, sizeof(sensor_contato));
	if (udp_socket == -1) {
		cerr << "udp_receiver_init: " << strerror(errno) << endl;
		exit(-1);
	}

	for (;;) {
		switch (udp_receiver_recv(udp_socket, &sensor_contato, sizeof(sensor_contato))) {
			case sizeof(sensor_contato):
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
