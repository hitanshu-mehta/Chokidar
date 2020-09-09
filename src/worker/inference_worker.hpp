#ifndef INFERENCE_WORKER_HPP
#define INFERENCE_WORKER_HPP

#include <string>

#include "callback_timer.hpp"

class inference_worker
{
	std::string curr_path;
	callback_timer* timer;
	void infer();
	bool done = true;

public:
	inference_worker(std::string);
};

#endif