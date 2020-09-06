#include "session_stats.hpp"

session_stats* session_stats::instance;
std::atomic<session_stats*> session_stats::s_instance;
std::mutex session_stats::s_mutex;

void session_stats::add_no_pkts_captured(long pkts) { no_pkts_captured += pkts; }

void session_stats::add_no_pkts_discarded(long pkts) { no_pkts_discarded += pkts; }

void session_stats::add_to_total_bytes(long bytes) { total_bytes += bytes; }

long const session_stats::get_total_bytes() { return total_bytes; }

long const session_stats::get_no_pkts_captured() { return no_pkts_captured; }

long const session_stats::get_no_pkts_discarded() { return no_pkts_discarded; }

session_stats::session_stats() { }

session_stats* session_stats::get_instance() {
	session_stats* p = s_instance.load(std::memory_order_acquire);
	if(p == nullptr) {
		std::lock_guard<std::mutex> lock(s_mutex);
		p = s_instance.load(std::memory_order_relaxed);
		if(p == nullptr) {
			p = new session_stats();
			s_instance.store(p, std::memory_order_release);
		}
	}
	return p;
}