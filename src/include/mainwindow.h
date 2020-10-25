#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/include/basic_packet_info.hpp"

#include <QCloseEvent>
#include <QMainWindow>
#include <QThread>

#include "database.hpp"

#include "inference_worker.hpp"
#include "packet_capture_worker.h"
#include "ui_reload_worker.hpp"

#include <QtCharts/QChart>

#include <string>

#include "Chart.h"

namespace Ui {
class MainWindow;
}

class main_window : public QMainWindow
{
	Q_OBJECT
	packet_capture_worker* p_worker;
	std::vector<basic_packet_info> buffer;
	char* filter;
	Ui::MainWindow* ui;
	bool start;
	bool is_sniffing;

	std::string curr_path;

	ui_reload* ui_r;
	inference_worker* infer_w;

	Chart* chart;

public:
	explicit main_window(std::string, QMainWindow* parent = nullptr);
	void create_protocol_chart();
	void create_ip_chart();

public slots:
	void handle_capture_button();
	void handle_done_button();
	void stop_packet_capture_worker();
	void start_packet_capture_worker();
};

#endif