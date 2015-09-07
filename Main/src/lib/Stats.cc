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

// https://en.wikipedia.org/wiki/Standard_deviation#Rapid_calculation_methods
// posui muitas informações úteis

#include <cmath>

#include "Stats.hh"

namespace Trekking {
	Stats::Stats() : _mean(0.), samples(0), q(0.) {}

	void Stats::sample(double value) {
		double old_mean = _mean;
		_mean += (value - _mean) / (++samples);
		q += (value - old_mean) * (value - _mean);
	}

	int Stats::sample_count() const {
		return samples;
	}

	double Stats::sample_variance() const {
		return q / (samples - 1);
	}

	double Stats::sample_std() const {
		return std::sqrt(sample_variance());
	}

	double Stats::mean() const {
		return _mean;
	}

	double Stats::variance() const {
		return q / samples;
	}

	double Stats::std() const {
		return std::sqrt(variance());
	}

	double Stats::sum() const {
		return _mean * samples;
	}
}
