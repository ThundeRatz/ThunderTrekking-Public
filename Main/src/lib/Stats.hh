#pragma once

#include <vector>

namespace Trekking {
	class Stats {
		public:
			Stats();
			void sample(double value);
			int sample_count();
			double sample_variance();
			double sample_std();
			double mean();
			double variance();
			double std();
			double sum();

		private:
			double _mean, samples, q;
	};
}
