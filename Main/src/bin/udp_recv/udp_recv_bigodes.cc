#include <stdexcept>
#include <iostream>
#include <cstdint>
#include <cstring>

#include <arpa/inet.h>

#include "ports.h"

#include "UDP.hh"

using namespace std;
using namespace Trekking;

int main() {
	struct sockaddr_in remote;
	char ip[41], error[32];
	uint8_t data;

	try {
		UDPReceiver bigode(UDP_BIGODE, sizeof data);

		for (;;) {
			switch (bigode.receive(&data, sizeof data)) {
				case sizeof(data):
				if (inet_ntop(AF_INET, &remote.sin_addr, ip, sizeof(ip) - 1) == NULL) {
					cerr << "Inet_ntop: " << strerror_r(errno, error, sizeof error) << endl;
					cout << data << endl;
				} else
					cout << ip << ": " << data << endl;
				break;

				case -1:
				cerr << "Recvfrom: " << strerror_r(errno, error, sizeof error) << endl;
				return -1;

				default:
				cout << "Unexpected message size\n";
				break;
			}
		}
	} catch (runtime_error& e) {
		cerr << e.what() << endl;
	}
}
