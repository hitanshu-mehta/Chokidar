#ifndef SESSION_STATS_HPP
#define SESSION_STATS_HPP

#include <atomic>
#include <mutex>

/* singleton class
    We have to make this class thread safe, beacause packet_capture_thread
    ui_reload_thread can try to access this class at the sametime.
 */
class session_stats
{
private:
	long no_pkts_captured = 0;
	long no_pkts_discarded = 0;
	long total_bytes = 0;
	long tcp_count = 0;
	long udp_count = 0;
	long ip_count = 0;
	long icmp_count = 0;
	long dccp_count = 0;
	long arp_count = 0;
	long ipv6_count = 0;

	static session_stats* instance;
	session_stats();
	static std::atomic<session_stats*> s_instance;
	static std::mutex s_mutex;

public:
	void add_no_pkts_captured(long);
	void add_no_pkts_discarded(long);
	void add_to_total_bytes(long);

	void add_to_tcp_count(long);
	void add_to_udp_count(long);
	void add_to_ip_count(long);
	void add_to_icmp_count(long);
	void add_to_dccp_count(long);
	void add_to_arp_count(long);
	void add_to_ipv6_count(long);

	long const get_no_pkts_captured();
	long const get_no_pkts_discarded();
	long const get_total_bytes();

	long const get_ip_count();
	long const get_udp_count();
	long const get_tcp_count();
	long const get_icmp_count();
	long const get_dccp_count();
	long const get_arp_count();
	long const get_ipv6_count();

	static session_stats* get_instance();
};

#endif