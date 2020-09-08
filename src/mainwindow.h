#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basic_packet_info.hpp"

#include <QCloseEvent>
#include <QMainWindow>
#include <QThread>

#include "database.hpp"
#include "worker/packet_capture_worker.h"
#include "worker/ui_reload_worker.hpp"

#include <QtCharts/QChart>

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
	ui_reload* ui_r;

	Chart* chart;

	// graphs_reload* g_r;
	// void closeEvent(QCloseEvent* close) {
	// 	database* db = database::get_instance();
	// 	ui->accept();
	// }

public:
	explicit main_window(QMainWindow* parent = nullptr);
	void create_protocol_chart();
	void create_ip_chart();

public slots:
	void handle_capture_button();
	void handle_done_button();
	void stop_packet_capture_worker();
	void start_packet_capture_worker();
};

#endif