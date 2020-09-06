#include "mainwindow.h"
#include "ui_mainwindow.h"

main_window::main_window(QMainWindow* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow) {
	ui->setupUi(this);
	filter = strdup(
		"ip and not src host 127.0.0.1 and not port 27017"); // mongodb client runs on port 27017

	QObject::connect(ui->capture_button, &QPushButton::toggled, this, &main_window::handle_button);
}

main_window::~main_window() { delete ui; }

void main_window::handle_button() {

	if(ui->capture_button->isChecked()) {
		ui->statusbar->showMessage(tr("Sniffing..."));
		ui->capture_button->setText("Stop Capturing");

		// Start packet capture engine on a thread
		p_worker = new packet_capture_worker(buffer, filter, 100, 1000, true);
		p_worker->start();
	}
	else {
		printf("Stop capturing\n");
		ui->capture_button->setText("Start Capturing");
		p_worker->requestInterruption();
		buffer.clear();
		ui->statusbar->clearMessage();
	}
	printf("button pressed\n");
}