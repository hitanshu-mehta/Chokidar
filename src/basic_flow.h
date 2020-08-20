#ifndef BASIC_FLOW_H
#define BASIC_FLOW_H

#include "basic_packet_info.h"
#include "constants.h"
#include "utils.h"

#include <map>
#include <netinet/in.h>
#include <string>
#include <vector>

class basic_flow
{
private:
	utils::summary_statistics fwd_pkt_stats;
	utils::summary_statistics bwd_pkt_stats;

	std::vector<basic_packet_info> forward;
	std::vector<basic_packet_info> backward;

	long forward_bytes;
	long backward_bytes;
	long fHeader_bytes;
	long bHeader_bytes;

	bool is_bidirectional;

	std::map<std::string, mutable_int> flag_counts;

	int fPSH_cnt;
	int bPSH_cnt;
	int fURG_cnt;
	int bURG_cnt;

	long act_data_pkt_forward;
	long min_seg_size_forward;
	int init_win_bytes_forward = 0;
	int init_win_bytes_backward = 0;

	in_addr src;
	in_addr dst;

	net::port_t src_port;
	net::port_t dst_port;
	int protocol;
	long flow_start_time;
	long start_active_time;
	long end_active_time;

	std::string flow_id = "";

	utils::summary_statistics flow_IAT;
	utils::summary_statistics forward_IAT;
	utils::summary_statistics backward_IAT;
	utils::summary_statistics flow_length_stats;
	utils::summary_statistics flow_active;
	utils::summary_statistics flow_idle;

	long flow_last_seen;
	long forward_last_seen;
	long backward_last_seen;
	long activity_timeout;

	/* flow bulks */
	long fbulk_duration = 0;
	long fbulk_packet_count = 0;
	long fbulk_size_total = 0;
	long fbulk_state_count = 0;
	long fbulk_packet_count_helper = 0;
	long fbulk_start_helper = 0;
	long fbulk_size_helper = 0;
	long flast_bulk_TS = 0;
	long bbulk_duration = 0;
	long bbulk_packet_count = 0;
	long bbulk_size_total = 0;
	long bbulk_state_count = 0;
	long bbulk_packet_count_helper = 0;
	long bbulk_start_helper = 0;
	long bbulk_size_helper = 0;
	long blast_bulk_TS = 0;

	/* subflows */
	long sf_last_packet_TS = -1;
	int sf_count = 0;
	long sf_ac_helper = -1;

public:
	void init_parameters();
	void init_flags();
	basic_flow(); // to do
	basic_flow(bool, basic_packet_info, in_addr, in_addr, net::port_t, net::port_t, long);
	basic_flow(bool, basic_packet_info, long);

	long const get_flow_start_time();
	int packet_count();
	void first_packet(basic_packet_info);
	void check_flags(basic_packet_info);
	void detect_update_subflows(basic_packet_info);
	void update_active_idle_time(long, long);
	void update_flow_bulk(basic_packet_info);
	void update_forward_bulk(basic_packet_info, long);
	void update_backward_bulk(basic_packet_info, long);
	void add_packet(basic_packet_info);
};

/* --------------------- */
class mutable_int
{
private:
	int val = 0;

public:
	mutable_int() { this->val = 0; }
	void increment() { this->val++; }
	int const get() { return this->val; }
};

#endif