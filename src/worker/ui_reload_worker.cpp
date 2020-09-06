#include "ui_reload_worker.hpp"
#include "../database.hpp"

#include <bsoncxx/builder/basic/document.hpp>

#include <QString>
#include <string>

ui_reload::ui_reload(Ui::MainWindow* Ui)
	: timer(new callback_timer())
	, ui(Ui)
	, no_pkts(0)
	, total_bytes(0) {
	timer->start(1000, std::bind(&ui_reload::update, this));
}

void ui_reload::update() {

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
}