#include "inference_worker.hpp"

#include <iostream>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

#include <chrono>
#include <thread>

inference_worker::inference_worker(std::string curr_p)
	: timer(new callback_timer())
	, curr_path(curr_p) {
	// m_timer.setInterval(4000); // every 5 mins // 18000000
	// m_timer.start();
	timer->start(18000000, std::bind(&inference_worker::infer, this));
}

void inference_worker::infer() {

	using namespace std::this_thread; // sleep_for, sleep_until
	using namespace std::chrono;	  // nanoseconds, system_clock, seconds

	pybind11::scoped_interpreter guard{};
	auto sys = pybind11::module::import("sys");
	sys.attr("path").attr("append")(curr_path + "/model"); // add python modules to PATH
	//sys.attr("path").attr("append")(
	//	"../venv/lib/python3.8/site-packages"); // add python modules(virtual environment) to PATH
	auto inference = pybind11::module::import("inference");
	while(1) {
		pybind11::object obj = inference.attr("load_model_and_infer")();
		std::pair<std::vector<int>, std::vector<int>> res =
			obj.cast<std::pair<std::vector<int>, std::vector<int>>>();

		fprintf(stderr, "INFERENCE RESULTS\n");
		for(auto i : res.first) { std::cout << i << "\n"; }
		for(auto i : res.second) { std::cout << i << "\n"; }
		fprintf(stderr, "--------------------------\n");

		sleep_until(system_clock::now() + seconds(300));
	}
}
