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
#include <cstring>
#include <stdexcept>

#include <arpa/inet.h>

#include "UDP.hh"

#include "ports.h"
#include "compass.h"

double direcao_atual = 0;

void __attribute__((noreturn)) hmc5883l_thread() {
	char error[32];
	int16_t data[3];


	try {
		Trekking::UDPReceiver hmc(UDP_HMC5883L, sizeof(data));

		for (;;) {
			switch (hmc.receive(data, sizeof data)) {
				case sizeof(data):
				direcao_atual = compass_orientation(data[0], data[2]);
				break;

				case -1:
				std::cerr << "Recvfrom: " << strerror_r(errno, error, sizeof error) << std::endl;
				break;

				default:
				std::cout << "Unexpected message size\n";
				break;
			}
		}
	} catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		exit(-1);
	}
}
