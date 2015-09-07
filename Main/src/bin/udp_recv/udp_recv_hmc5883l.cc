/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 ThundeRatz

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include <iostream>
#include <cstdint>
#include <stdexcept>

#include <arpa/inet.h>

#include "UDP.hh"
#include "errno_string.hh"

#include "ports.h"
#include "compass.h"

using namespace std;
using namespace Trekking;

int main() {
	struct sockaddr_in remote;
	char ip[41];
	int16_t data[3];

	try {
		UDPReceiver hmc(UDP_HMC5883L, sizeof(data));

		for (;;) {
			switch (hmc.receive(data, sizeof data)) {
				case sizeof(data):
				if (inet_ntop(AF_INET, &remote.sin_addr, ip, sizeof(ip) - 1) == NULL) {
					cerr << "Inet_ntop: " << errno_string() << endl;
					cout << data[0] << " " << data[1] << " " << data[2] << " " << compass_orientation(data[0], data[2]) << endl;
				} else
					cout << ip << ": " << data[0] << " " << data[1] << " " << data[2] << " " << compass_orientation(data[0], data[2]) << endl;
				break;

				case -1:
				cerr << "Recvfrom: " << errno_string() << endl;
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
