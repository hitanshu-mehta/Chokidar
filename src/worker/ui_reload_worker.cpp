#include "ui_reload_worker.hpp"
#include <QString>
#include <string>

ui_reload::ui_reload(Ui::MainWindow* Ui)
	: timer(new callback_timer())
	, ui(Ui) {
	timer->start(1000, std::bind(&ui_reload::update, this));
}

void ui_reload::update() {
	session_stats* stats = session_stats::get_instance();
	ui->packet_captured_cnt_label->setText(
		QString::fromStdString(std::to_string(stats->get_no_pkts_captured())));
	ui->packet_discarded_cnt_label->setText(
		QString::fromStdString(std::to_string(stats->get_no_pkts_discarded())));
}