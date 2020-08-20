
#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <cmath>
#include <vector>

namespace utils {

static long id = 0;

// class id_generator
// {
// private:
// 	static long id;
// 	id_generator() { this->id = 0L; }
// 	id_generator(long id) { this->id = id; }
// 	long next_id() {
// 		this->id++;
// 		return this->id;
// 	}
// };

class summary_statistics
{
private:
	std::vector<double> values;
	double sum = 0;

public:
	summary_statistics() {
		values = std::vector<double>();
		sum = 0;
	}
	void add_value(double val) {
		values.push_back(val);
		sum += val;
	}
	int const get_count() { return values.size(); }
	double const get_sum() { return this->sum; }
	double const get_avg() { return (get_sum() / (get_count())); }
	double const get_min() { return *std::min_element(values.begin(), values.end()); }
	double const get_max() { return *std::max_element(values.begin(), values.end()); }
	double const get_variance() {
		double var = 0;
		double mean = get_avg();
		for(long i = 0; i < (long)values.size(); i++) {
			var += (values[i] - mean) * (values[i] - mean);
		}
		var = var / (double)values.size();
		return var;
	}
	double const get_standard_deviation() { return std::sqrt(get_variance()); }
};

} // namespace utils

#endif