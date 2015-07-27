#pragma once

#include <vector>

namespace Trekking {
	class Stats {
		public:
			Stats();
			void sample(double value);
			int sample_count() const;
			double sample_variance() const;
			double sample_std() const;
			double mean() const;
			double variance() const;
			double std() const;
			double sum() const;

		private:
			double _mean, samples, q;
	};
}
