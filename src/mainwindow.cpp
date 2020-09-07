#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worker/ui_reload_worker.hpp"

#include <pcap.h>

#include <QString>
#include <string>

main_window::main_window(QMainWindow* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, start(false) {
	ui->setupUi(this);
	filter = strdup(
		"ip and not src host 127.0.0.1 and not port 27017"); // mongodb client runs on port 27017

	QObject::connect(
		ui->capture_button, &QPushButton::toggled, this, &main_window::handle_capture_button);
	QObject::connect(
		ui->done_button, &QPushButton::pressed, this, &main_window::handle_done_button);
}

main_window::~main_window() { delete ui; }

void main_window::handle_capture_button() {

	if(ui->capture_button->isChecked()) start_packet_capture_worker();
	else
		stop_packet_capture_worker();

	printf("button pressed\n");
}

void main_window::stop_packet_capture_worker() {

	printf("Stop capturing\n");
	ui->capture_button->setText("Start Capturing");

	if(p_worker->isRunning()) p_worker->requestInterruption();
	p_worker->exit();

	buffer.clear();

	ui->statusbar->clearMessage();
}

void main_window::start_packet_capture_worker() {
	if(!start) {
		start = true;
		ui_reload* ui_r = new ui_reload(this->ui);
	}
	ui->statusbar->showMessage(tr("Sniffing..."));
	ui->capture_button->setText("Stop Capturing");

	// Start packet capture engine on a thread
	p_worker = new packet_capture_worker(buffer, filter, 100, 1000, true);
	p_worker->start();
}

void main_window::handle_done_button() {

	QString f = ui->filter_textedit->toPlainText();
	char* _filter = strdup(f.toStdString().c_str());

	if(p_worker->get_engine()->get_packet_capture()->compile_filter_expression(_filter) == -1) {
		// invalid filter expression
		return;
	}

	filter = _filter;

	// stop packet_capture_worker if running
	stop_packet_capture_worker();
	start_packet_capture_worker();
}