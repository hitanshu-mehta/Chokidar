#include "basic_packet_info.hpp"
#include "flow_generator_engine.hpp"
#include "packet_capture_engine.hpp"

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <string.h>
#include <vector>

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "mainwindow.h"

#ifdef WINDOWS
	#include <direct.h>
	#define GetCurrentDir _getcwd
#else
	#include <unistd.h>
	#define GetCurrentDir getcwd
#endif

std::string get_curr_working_dir() {
	char buff[FILENAME_MAX]; //create string buffer to hold path
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	main_window main_win;
	main_win.show();

	// pybind11::scoped_interpreter guard{};
	// std::vector<basic_packet_info> buffer;
	// // auto sys = pybind11::module::import("sys");

	// std::string curr_path = get_curr_working_dir();
	// curr_path = curr_path;

	// // std::cout << curr_path << std::endl;

	// auto sys = pybind11::module::import("sys");
	// sys.attr("path").attr("append")(curr_path + "/model"); // add python modules to PATH
	// sys.attr("path").attr("append")(
	// 	"../venv/lib/python3.8/site-packages"); // add python modules(virtual environment) to PATH

	// auto inference = pybind11::module::import("inference");

	// char* filter = strdup(
	// 	"ip and not src host 127.0.0.1 and not port 27017"); // mongodb client runs on port 27017
	// database* const db = database::get_instance();
	// packet_capture_engine(buffer, filter, 100, 1000, true);
	// flow_generator_engine(120000000L, 5000000L, "/home/hitanshu/output.txt", buffer);
	// flow_generator_engine(120000000L, 5000000L, db, buffer);

	// pybind11::object obj = inference.attr("load_model_and_infer")();

	// std::pair<std::vector<int>, std::vector<int>> res =
	// 	obj.cast<std::pair<std::vector<int>, std::vector<int>>>();
	// // for(auto i : res.first) { std::cout << i << "\n"; }
	// // for(auto i : res.second) { std::cout << i << "\n"; }
	return app.exec();
}
