#include "src/include/packet_capture_worker.h"

#include "src/include/flow_generator_engine.hpp"
#include "src/include/ui_reload_worker.hpp"

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
		if(count >= 100) {
			flow_generator_engine(120000000L, 5000000L, db, *buffer);
			buffer->clear();
			count = 0;
		}
	}
}

packet_capture_engine* packet_capture_worker::get_engine() { return p_engine; }
