#include "packet_capture_worker.h"
#include "../flow_generator_engine.hpp"

packet_capture_worker::packet_capture_worker(std::vector<basic_packet_info>& buff,
											 char* filter,
											 int no_pkts,
											 int timeout,
											 bool is_promiscious)
	: p_engine(new packet_capture_engine(buff, filter, no_pkts, timeout, is_promiscious))
	, buffer(&buff) { }

void packet_capture_worker::sniff() {
	int count = 0;
	while(true) {
		if(QThread::currentThread()->isInterruptionRequested()) {
			flow_generator_engine(120000000L, 5000000L, db, *buffer);
			buffer->clear();
			return;
		}
		p_engine->sniff();
		++count;
		if(count >= 1000) {
			flow_generator_engine(120000000L, 5000000L, db, *buffer);
			buffer->clear();
		}
	}
}