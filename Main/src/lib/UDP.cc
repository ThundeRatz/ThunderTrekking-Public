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
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "ports.h"

#include "UDP.hh"

namespace Trekking {
	UDPReceiver::UDPReceiver(uint16_t port, int packet_size) {
		struct sockaddr_in local;
		char error[32];

		this->port = port;

		if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
			throw std::runtime_error("Socket: " + std::string(strerror_r(errno, error, sizeof error)));

		memset((char *) &local, 0, sizeof local);
		local.sin_family = AF_INET;	        //IPv4
		local.sin_addr.s_addr = INADDR_ANY;
		local.sin_port = htons(this->port);

		if (bind(socketfd, (struct sockaddr *) &local, sizeof local) == -1)
			throw std::runtime_error("Bind: " + std::string(strerror_r(errno, error, sizeof error)));

		if (setsockopt(socketfd, SOL_SOCKET, SO_RCVBUF, &packet_size, sizeof(packet_size)) == -1)
			std::cerr << "Setsockopt: " << strerror_r(errno, error, sizeof error) << std::endl;
	}

	ssize_t UDPReceiver::receive(void* data, size_t size) {
		return ::recv(this->socketfd, data, size, 0);
	}

	UDPSender::UDPSender(uint16_t port) {
		struct sockaddr_in remote;
		char error[32];

		this->port = port;

		if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
			throw std::runtime_error("Socket: " + std::string(strerror_r(errno, error, sizeof error)));

		memset((char *) &remote, 0, sizeof remote);
		remote.sin_family = AF_INET;	//IPv4
		if (!inet_pton(AF_INET, IP, &remote.sin_addr.s_addr))
			throw std::runtime_error("Inet_pton: " + std::string(strerror_r(errno, error, sizeof error)));
		remote.sin_port = htons(this->port);

		if (connect(socketfd, (struct sockaddr *) &remote, sizeof(remote)) == -1)
			throw std::runtime_error("Connect: " + std::string(strerror_r(errno, error, sizeof error)));
	}

	ssize_t UDPSender::send(void* data, size_t size) {
		return ::send(socketfd, data, size, 0);
	}
}
