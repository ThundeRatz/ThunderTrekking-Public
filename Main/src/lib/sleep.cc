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

#include "errno_string.hh"
#include "sleep.hh"

void Trekking::sleep_ms(long ms) {
	struct timespec sleep_time = {
		.tv_sec = ms / 1000,
		.tv_nsec = (ms % 1000) * 1000000
	};
	if(nanosleep(&sleep_time, NULL))
		std::cerr << "sleep_ms: " << errno_string() << std::endl;
}

void Trekking::sleep_us(long us) {
	struct timespec sleep_time = {
		.tv_sec = us / 1000000,
		.tv_nsec = (us % 1000000) * 1000
	};
	if(nanosleep(&sleep_time, NULL))
		std::cerr << "sleep_us: " << errno_string() << std::endl;
}

void Trekking::sleep_ns(long ns) {
	struct timespec sleep_time = {
		.tv_sec = ns / 1000000000,
		.tv_nsec = (ns % 1000000000)
	};
	if(nanosleep(&sleep_time, NULL))
		std::cerr << "sleep_ns: " << errno_string() << std::endl;
}
