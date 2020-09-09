#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <pcap.h>

#include <QMessageBox>
#include <QString>

#include <string>

#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QValueAxis>

main_window::main_window(std::string curr_p, QMainWindow* parent)
	: QMainWindow(parent)
	, curr_path(curr_p)
	, ui(new Ui::MainWindow)
	, chart(new Chart)
	, start(false)
	, is_sniffing(false) {

	ui->setupUi(this);

	// create charts
	create_protocol_chart();
	create_ip_chart();
	chart->setTitle("Network Utilization - 1 Hour Window (1 sec average)");
	chart->legend()->hide();
	chart->setAnimationOptions(QChart::AllAnimations);
	ui->Mbits_graph->setChart(chart);

	filter =
		strdup("not ip src host 127.0.0.1 and not port 27017"); // mongodb client runs on port 27017

	QObject::connect(
		ui->capture_button, &QPushButton::toggled, this, &main_window::handle_capture_button);
	QObject::connect(
		ui->done_button, &QPushButton::pressed, this, &main_window::handle_done_button);

	// setup_python();

	infer_w = new inference_worker(curr_path);
}

// void main_window::setup_python() { }

void main_window::handle_capture_button() {

	if(ui->capture_button->isChecked()) start_packet_capture_worker();
	else
		stop_packet_capture_worker();

	printf("button pressed\n");
}

void main_window::stop_packet_capture_worker() {

	printf("Stop capturing\n");
	p_worker->requestInterruption();

	while(p_worker->isRunning()) { };
	p_worker->quit();

	buffer.clear();

	ui->statusbar->clearMessage();
	ui->capture_button->setText("Start Capturing");
	ui->capture_button->setStyleSheet(
		QString::fromUtf8("QPushButton{\n"
						  "	font: 200 11pt \"Noto Sans\";\n"
						  "	\n"
						  "	background-color: qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:0, "
						  "stop:0 rgba(4, 224, 0, 85), stop:1 rgba(255, 255, 255, 255));\n"
						  "}"));
	ui->capture_button->setDown(false);

	is_sniffing = false;
}

void main_window::start_packet_capture_worker() {

	ui->statusbar->showMessage(tr("Sniffing..."));
	ui->capture_button->setText("Stop Capturing");
	ui->capture_button->setDown(true);
	ui->capture_button->setStyleSheet(
		QString::fromUtf8("QPushButton{\n"
						  "	font: 200 11pt \"Noto Sans\";\n"
						  "	colotr:white;\n"
						  "background-color: qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:0, "
						  "stop:0 rgba(255, 0, 0, 116), stop:1 rgba(255, 255, 255, 255));\n"
						  "}"));
	if(!start) {
		start = true;
		ui_r = new ui_reload(this->ui);
		// g_r = new graphs_reload(this->ui);

		p_worker = new packet_capture_worker(buffer, filter, 100, 1000, true);
		p_worker->start();
		is_sniffing = true;
		return;
	}
	while(!p_worker->isFinished()) { };
	delete p_worker;

	// Start packet capture engine on a thread
	p_worker = new packet_capture_worker(buffer, filter, 100, 1000, true);
	p_worker->start();
	is_sniffing = true;
}

void main_window::handle_done_button() {

	QString f = ui->filter_textedit->toPlainText();
	char* _filter = strdup(f.toStdString().c_str());
	bool here = false;

	if(!start) {
		start_packet_capture_worker();
		start = true;
		here = true;
	}

	if(p_worker->get_engine()->get_packet_capture()->compile_filter_expression(_filter) == -1) {

		int ret = QMessageBox::warning(
			this,
			tr("Invalid filter!"),
			tr("See this page-> (https://www.tcpdump.org/manpages/pcap-filter.7.html) for packet "
			   "filter syntax."));
		if(here) stop_packet_capture_worker();
		return;
	}

	filter = _filter;

	if(here) stop_packet_capture_worker();

	// stop packet_capture_worker if running
	if(is_sniffing) {
		stop_packet_capture_worker();
		start_packet_capture_worker();
	}
}

void main_window::create_protocol_chart() {
	QBarSet* set0 = new QBarSet("IP");
	QBarSet* set1 = new QBarSet("TCP");
	QBarSet* set2 = new QBarSet("UDP");
	QBarSet* set3 = new QBarSet("ICMP");
	QBarSet* set4 = new QBarSet("DCCP");
	QBarSet* set5 = new QBarSet("IPv6");
	QBarSet* set6 = new QBarSet("ARP");

	*set0 << 0;
	*set1 << 0;
	*set2 << 0;
	*set3 << 0;
	*set4 << 0;
	*set5 << 0;
	*set6 << 0;

	QBarSeries* series0 = new QBarSeries();
	QBarSeries* series1 = new QBarSeries();
	QBarSeries* series2 = new QBarSeries();
	QBarSeries* series3 = new QBarSeries();
	QBarSeries* series4 = new QBarSeries();
	QBarSeries* series5 = new QBarSeries();
	QBarSeries* series6 = new QBarSeries();
	series0->append(set0);
	series1->append(set1);
	series2->append(set2);
	series3->append(set3);
	series4->append(set4);
	series5->append(set5);
	series6->append(set6);

	QChart* protocol_chart = new QChart();
	protocol_chart->addSeries(series0);
	protocol_chart->addSeries(series1);
	protocol_chart->addSeries(series2);
	protocol_chart->addSeries(series3);
	protocol_chart->addSeries(series4);
	protocol_chart->addSeries(series5);
	protocol_chart->addSeries(series6);
	protocol_chart->setAnimationOptions(QChart::SeriesAnimations);

	QStringList categories;
	categories << "Protocols";
	QBarCategoryAxis* axisX = new QBarCategoryAxis();

	axisX->append(categories);
	protocol_chart->addAxis(axisX, Qt::AlignBottom);
	series0->attachAxis(axisX);
	series1->attachAxis(axisX);
	series2->attachAxis(axisX);
	series3->attachAxis(axisX);
	series4->attachAxis(axisX);
	series5->attachAxis(axisX);
	series6->attachAxis(axisX);

	QValueAxis* axisY = new QValueAxis();
	axisY->setRange(0, 15);
	protocol_chart->addAxis(axisY, Qt::AlignLeft);
	series0->attachAxis(axisY);
	series1->attachAxis(axisY);
	series2->attachAxis(axisY);
	series3->attachAxis(axisY);
	series4->attachAxis(axisY);
	series5->attachAxis(axisY);
	series6->attachAxis(axisY);

	protocol_chart->legend()->setVisible(true);
	protocol_chart->legend()->setAlignment(Qt::AlignBottom);
	protocol_chart->setMargins(QMargins(0, 0, 0, 0));

	ui->protocol_bar_chart->setChart(protocol_chart);
}

void main_window::create_ip_chart() {

	QChart* ip_chart = new QChart();

	ip_chart->setAnimationOptions(QChart::SeriesAnimations);

	QStringList categories;
	categories << "Protocols";
	QBarCategoryAxis* axisX = new QBarCategoryAxis();

	axisX->append(categories);
	ip_chart->addAxis(axisX, Qt::AlignBottom);

	QValueAxis* axisY = new QValueAxis();
	axisY->setRange(0, 15);
	ip_chart->addAxis(axisY, Qt::AlignLeft);

	ip_chart->legend()->setVisible(true);
	ip_chart->legend()->setAlignment(Qt::AlignBottom);
	ip_chart->setMargins(QMargins(0, 0, 0, 0));

	ui->top_ip_chart->setChart(ip_chart);
}