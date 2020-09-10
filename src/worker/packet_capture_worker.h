#ifndef PACKET_CAPTURE_WORKER_HPP
#define PACKET_CAPTURE_WORKER_HPP

#include "../database.hpp"
#include "../packet_capture_engine.hpp"

#include <QObject>
#include <QString>
#include <QThread>

class packet_capture_worker : public QThread
{
	Q_OBJECT

	packet_capture_engine* p_engine;

	std::vector<basic_packet_info>* buffer;
	database* const db = database::get_instance();

	void run() override {
		sniff();
		emit result_ready();
	}

signals:
	void result_ready();

public:
	explicit packet_capture_worker(std::vector<basic_packet_info>&, char*, int, int, bool);
	void sniff();
	explicit packet_capture_worker(){};
	~packet_capture_worker(){};
	packet_capture_engine* get_engine();
};

#endif