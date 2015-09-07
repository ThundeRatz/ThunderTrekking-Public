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

#include "UDP.hh"

#include "ports.h"
#include "camera.h"

using namespace std;
using namespace Trekking;

int main() {
	camera_data data;
	char error[32];

	try {
		UDPReceiver camera(UDP_CAMERA, sizeof data);

		for (;;) {
			switch (camera.receive(&data, sizeof data)) {
				case sizeof(data):
				cout << data.top_left.x << " " << data.top_left.y
					<< " " << data.bottom_right.x << " " << data.bottom_right.y << endl;
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
		cout << e.what() << endl;
	}
}
