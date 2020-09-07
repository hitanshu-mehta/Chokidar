#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basic_packet_info.hpp"

#include <QCloseEvent>
#include <QMainWindow>
#include <QThread>

#include "database.hpp"
#include "worker/packet_capture_worker.h"

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
	// void closeEvent(QCloseEvent* close) {
	// 	database* db = database::get_instance();
	// 	ui->accept();
	// }

public:
	explicit main_window(QMainWindow* parent = nullptr);
	~main_window();

public slots:
	void handle_capture_button();
	void handle_done_button();
	void stop_packet_capture_worker();
	void start_packet_capture_worker();
};

#endif