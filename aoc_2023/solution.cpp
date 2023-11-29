#include "solution.hpp"

double variance_from(const std::vector<double>& samples, double mean, int size) {
	// unbiased sample variance
	auto variance_func = [&mean](double accumulator, const double val) {
		return accumulator + ((val - mean) * (val - mean));
	};

	return std::accumulate(samples.begin(), samples.end(), 0.0, variance_func) / (size - 1);
}