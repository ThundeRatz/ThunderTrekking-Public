// https://en.wikipedia.org/wiki/Standard_deviation#Rapid_calculation_methods
// posui muitas informações úteis

#include <cmath>

#include "Stats.h"

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
