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

#include "Compass.hh"

#include <cmath>
#include <cstdint>

namespace Trekking {
	double Compass::angle(double x, double y) {
		return atan2(y, x);
	}

	double Compass::heading(double x, double y) {
		return atan2(x, y);
	}

	double Compass::diff(double target) {
		return diff(read_compass(), target);
	}

	double Compass::read_angle() {
		return -read_compass();
	}

	double Compass::diff(double target, double now) {
		double r;
		r = fmod((target - now), 2 * M_PI);
		if (r < -M_PI) {
			r += 2 * M_PI;
			return r;
		}
		if (r > M_PI)
			r -= 2 * M_PI;
		return r;
	}
}
