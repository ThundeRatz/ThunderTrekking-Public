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
#include <ctime>

#include "errno_string.hh"
#include "sleep.hh"
#include "ThreadSpawn.hh"

#define MS			1000000

#define INIT_MAX_SPEED	255

static int max_speed = INIT_MAX_SPEED;
static int speed_right = 0, speed_left = 0;

using namespace std;

#ifdef MOTORS_I2C

#include <cstdlib>

#include "i2c.h"

void __attribute__((noreturn)) motors_thread() {
	char error[32];
	static const unsigned int sleep_time = 50;
	static int i2c;

	i2c = i2c_open(1, "i915 gmbus vga");
	if (i2c == -1) {
		cerr << "i2c_open: " << errno_string() << endl;;
		exit(-1);
	}

	if (i2c_slave(i2c, 0x69)) {
		cerr << "i2c_slave: " << errno_string() << endl;
		exit(-1);
	}

	for (;;) {
		Trekking::sleep_ms(sleep_time);
		if (i2c_smbus_write_byte_data(i2c, 0, speed_left < 0))
			cerr << "i2c_smbus_write_byte_data: " << errno_string() << endl;
		if (i2c_smbus_write_byte_data(i2c, 1, speed_left >= 0 ? speed_left * max_speed / 255 : -speed_left * max_speed / 255))
			cerr << "i2c_smbus_write_byte_data: " << errno_string() << endl;
		if (i2c_smbus_write_byte_data(i2c, 2, speed_right < 0))
			cerr << "i2c_smbus_write_byte_data: " << errno_string() << endl;
		if (i2c_smbus_write_byte_data(i2c, 3, speed_right >= 0 ? speed_right * max_speed / 255 : -speed_right * max_speed / 255))
			cerr << "i2c_smbus_write_byte_data: " << errno_string() << endl;
	}
}

#else

#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "serial.h"

void motors_thread() {
	static const int baudrate = 9600;
	static const unsigned long sleep_time = 10;
	int fd = serial_open("/dev/ttyUSB0", &baudrate, O_WRONLY);
	while (fd == -1) {
		fd = serial_open("/dev/ttyUSB0", &baudrate, O_WRONLY);
		Trekking::sleep_ms(sleep_time);
	}
	while (running) {
		uint8_t message;
		Trekking::sleep_ms(sleep_time);
		message = (1<< 7) | (speed_left < 0 ? 1 << 6 | (-(speed_left / 4) & 0x3f) : (speed_left / 4) & 0x3f);
		if (write(fd, &message, 1) == -1) {
			cerr << "write: " << errno_string() << endl;
			close(fd);
			do {
				fd = serial_open("/dev/ttyUSB0", &baudrate, O_WRONLY);
				Trekking::sleep_ms(sleep_time);
				if (fd != -1)
					break;
				fd = serial_open("/dev/ttyUSB1", &baudrate, O_WRONLY);
				Trekking::sleep_ms(sleep_time);
			} while (fd == -1);
		}
		message = 0xAA | __builtin_parity(message);
		//printf("%hhu\n", message);
		if (write(fd, &message, 1) == -1)
			cerr << "write: " << errno_string() << endl;
		message = speed_right < 0 ? 1 << 6 | (-(speed_right / 4) & 0x3f) : ((speed_right / 4) & 0x3f);
		if (write(fd, &message, 1) == -1)
			cerr << "write: " << errno_string() << endl;
		message = 0xAA | __builtin_parity(message);
		//printf("%hhu\n", message);
		if (write(fd, &message, 1) == -1)
			cerr << "write: " << errno_string() << endl;
		fsync(fd);
	}
}

#endif

void motor(int left, int right) {
	cout << "motores: " << left << " " << right << endl;
	if (left >= 255)
		speed_left = max_speed;
	else if (left <= -255)
		speed_left = -max_speed;
	else
		speed_left = left * max_speed / 255;

	if (right >= 255)
		speed_right = max_speed;
	else if (right <= -255)
		speed_right = -max_speed;
	else
		speed_right = right * max_speed / 255;
}

void set_max_speed(int new_max_speed) {
	max_speed = new_max_speed > 255 ? 255 : new_max_speed;
}

void reset_max_speed() {
	max_speed = INIT_MAX_SPEED;
}

int get_max_speed() {
	return max_speed;
}
