#include "session_stats.hpp"

session_stats* session_stats::instance;
std::atomic<session_stats*> session_stats::s_instance;
std::mutex session_stats::s_mutex;

void session_stats::add_no_pkts_captured(long pkts) { no_pkts_captured += pkts; }

void session_stats::add_no_pkts_discarded(long pkts) { no_pkts_discarded += pkts; }

void session_stats::add_to_total_bytes(long bytes) { total_bytes += bytes; }

void session_stats::add_to_tcp_count(long proto) { tcp_count += proto; }

void session_stats::add_to_udp_count(long proto) { udp_count += proto; }

void session_stats::add_to_ip_count(long proto) { ip_count += proto; }

void session_stats::add_to_icmp_count(long proto) { icmp_count += proto; }

void session_stats::add_to_dccp_count(long proto) { dccp_count += proto; }

void session_stats::add_to_arp_count(long proto) { arp_count += proto; }

void session_stats::add_to_ipv6_count(long proto) { ipv6_count += proto; }

void session_stats::add_uni_ip(std::string ip, int cnt) { top_ip_count[ip] += cnt; }

// getter methods

long const session_stats::get_total_bytes() { return total_bytes; }
long const session_stats::get_no_pkts_captured() { return no_pkts_captured; }
long const session_stats::get_no_pkts_discarded() { return no_pkts_discarded; }

long const session_stats::get_ip_count() { return ip_count; }
long const session_stats::get_tcp_count() { return tcp_count; }
long const session_stats::get_udp_count() { return udp_count; }
long const session_stats::get_icmp_count() { return icmp_count; }
long const session_stats::get_dccp_count() { return dccp_count; }
long const session_stats::get_arp_count() { return arp_count; }
long const session_stats::get_ipv6_count() { return ipv6_count; }

std::map<std::string, int> session_stats::get_map() { return top_ip_count; } // ----------------

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