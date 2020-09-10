#include "ui_reload_worker.hpp"
#include "../database.hpp"
#include "../graph/bar_chart.hpp"

#include <bsoncxx/builder/basic/document.hpp>

#include <QtCore/QString>
#include <QtCore/QVector>

#include <algorithm>
#include <string>
#include <vector>

#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QValueAxis>

ui_reload::ui_reload(Ui::MainWindow* Ui)
	: ui(Ui)
	, no_pkts(0)
	, total_bytes(0) {
	QObject::connect(&m_timer, &QTimer::timeout, this, &ui_reload::update);
	m_timer.setInterval(1000);
	// timer->start(1000, std::bind(&ui_reload::update, this));
	m_timer.start();
}

ui_reload::~ui_reload() { }

void ui_reload::update() {
	++g_timer;

	database* db_c = database::get_instance();

	session_stats* stats = session_stats::get_instance();

	ui->packet_captured_cnt_label->setText(
		QString::fromStdString(std::to_string(stats->get_no_pkts_captured())));
	ui->packet_discarded_cnt_label->setText(
		QString::fromStdString(std::to_string(stats->get_no_pkts_discarded())));

	int bytes_s = (stats->get_total_bytes() - total_bytes);
	ui->network_util_lcd->display((bytes_s * 8) / 1000000.0);
	total_bytes = stats->get_total_bytes();

	ui->packet_s_lcd->display(
		(int)(stats->get_no_pkts_captured() + stats->get_no_pkts_discarded() - no_pkts));
	no_pkts = stats->get_no_pkts_captured() + stats->get_no_pkts_discarded();

	auto filter = bsoncxx::builder::basic::document{};
	int buff_size = (int)db_c->get_db()->collection("flows").count_documents(filter.view());
	int per = (buff_size * 100) / 5000;
	ui->buffer_usage_bar->setValue(per);

	// reload graphs at every 3 secs
	if(g_timer == 3) {

		reload_protocol_graphs(stats);
		reload_ip_graphs(stats);
		g_timer = 0;
	}
}

void ui_reload::reload_ip_graphs(session_stats* stats) {

	std::map<std::string, int> mp;
	mp = stats->get_map();
	std::vector<std::pair<int, std::string>> vec;

	for(auto el : mp) vec.push_back({el.second, el.first});

	sort(vec.begin(), vec.end(), [&](std::pair<int, std::string> a, std::pair<int, std::string> b) {
		return a.first > b.first;
	});

	int limit = std::min(6, (int)vec.size());

	long max_y = mp.begin()->second;

	std::vector<QBarSet*> bars;
	std::vector<QBarSeries*> series;

	QChart* top_ip_chart = new QChart();

	QStringList categories;
	categories << "Top IPs";
	QBarCategoryAxis* axisX = new QBarCategoryAxis();
	axisX->append(categories);
	top_ip_chart->addAxis(axisX, Qt::AlignBottom);

	QValueAxis* axisY = new QValueAxis();
	axisY->setRange(0, max_y + 100);
	top_ip_chart->addAxis(axisY, Qt::AlignLeft);

	for(int i = 0; i < limit; ++i) {
		bars.push_back(new QBarSet(QString::fromStdString(vec[i].second)));
		*bars[i] << vec[i].first;
		series.push_back(new QBarSeries());
		series[i]->append(bars[i]);
		top_ip_chart->addSeries(series[i]);
		series[i]->attachAxis(axisX);
		series[i]->attachAxis(axisY);
	}

	top_ip_chart->legend()->setVisible(true);
	top_ip_chart->legend()->setAlignment(Qt::AlignBottom);
	top_ip_chart->setMargins(QMargins(0, 0, 0, 0));
	ui->top_ip_chart->setChart(top_ip_chart);
}

void ui_reload::reload_protocol_graphs(session_stats* stats) {

	long max_y = std::max({stats->get_ip_count(),
						   stats->get_tcp_count(),
						   stats->get_udp_count(),
						   stats->get_icmp_count(),
						   stats->get_dccp_count(),
						   stats->get_ipv6_count(),
						   stats->get_arp_count()});

	QBarSet* set0 = new QBarSet("IP");
	QBarSet* set1 = new QBarSet("TCP");
	QBarSet* set2 = new QBarSet("UDP");
	QBarSet* set3 = new QBarSet("ICMP");
	QBarSet* set4 = new QBarSet("DCCP");
	QBarSet* set5 = new QBarSet("IPv6");
	QBarSet* set6 = new QBarSet("ARP");

	*set0 << stats->get_ip_count();
	*set1 << stats->get_tcp_count();
	*set2 << stats->get_udp_count();
	*set3 << stats->get_icmp_count();
	*set4 << stats->get_dccp_count();
	*set5 << stats->get_ipv6_count();
	*set6 << stats->get_arp_count();

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
	// protocol_chart->setAnimationOptions(QChart::SeriesAnimations);

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
	axisY->setRange(0, max_y + 100);
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