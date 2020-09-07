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

	static session_stats* instance;
	session_stats();
	static std::atomic<session_stats*> s_instance;
	static std::mutex s_mutex;

public:
	void add_no_pkts_captured(long);
	void add_no_pkts_discarded(long);
	void add_to_total_bytes(long);

	long const get_no_pkts_captured();
	long const get_no_pkts_discarded();
	long const get_total_bytes();

	static session_stats* get_instance();
};

#endif