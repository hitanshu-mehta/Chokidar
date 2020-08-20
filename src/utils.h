
#ifndef UTILS_H
#define UTILS_H

#include <vector>

namespace utils {

class id_generator
{
private:
	long id = 0;
	id_generator() { this->id = 0L; }
	id_generator(long id) { this->id = id; }
	long next_id() {
		this->id++;
		return this->id;
	}
};

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
};

} // namespace utils

#endif