#include "src/include/basic_flow.hpp"
#include <arpa/inet.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <bsoncxx/json.hpp>
#include <cstdint>
#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <vector>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

basic_flow::basic_flow(bool is_bidirectional,
					   basic_packet_info packet,
					   in_addr src,
					   in_addr dst,
					   net::port_t src_post,
					   net::port_t dst_port,
					   long activity_timeout) {
	this->activity_timeout = activity_timeout;
	init_parameters();
	this->is_bidirectional = is_bidirectional;
	this->first_packet(packet);
	this->src = src;
	this->dst = dst;
	this->src_port = src_port;
	this->dst_port = dst_port;
}

basic_flow::basic_flow(bool is_bidirectional, basic_packet_info packet, long activity_timeout) {
	this->activity_timeout = activity_timeout;
	this->init_parameters();
	this->is_bidirectional = true;
	first_packet(packet);
}

void basic_flow::init_parameters() {
	this->forward = std::vector<basic_packet_info>();
	this->backward = std::vector<basic_packet_info>();
	this->flow_IAT = utils::summary_statistics();
	this->forward_IAT = utils::summary_statistics();
	this->backward_IAT = utils::summary_statistics();
	this->flow_active = utils::summary_statistics();
	this->flow_idle = utils::summary_statistics();
	this->flow_length_stats = utils::summary_statistics();
	this->fwd_pkt_stats = utils::summary_statistics();
	this->bwd_pkt_stats = utils::summary_statistics();
	this->flag_counts = std::map<std::string, mutable_int>();
	init_flags();
	this->forward_bytes = 0L;
	this->backward_bytes = 0L;
	this->start_active_time = 0L;
	this->end_active_time = 0L;
	this->src = in_addr();
	this->dst = in_addr();
	this->fPSH_cnt = 0;
	this->bPSH_cnt = 0;
	this->fURG_cnt = 0;
	this->bURG_cnt = 0;
	this->fHeader_bytes = 0L;
	this->bHeader_bytes = 0L;
}

void basic_flow::init_flags() {
	flag_counts["FIN"] = mutable_int();
	flag_counts["SYN"] = mutable_int();
	flag_counts["RST"] = mutable_int();
	flag_counts["PSH"] = mutable_int();
	flag_counts["ACK"] = mutable_int();
	flag_counts["URG"] = mutable_int();
	flag_counts["CWR"] = mutable_int();
	flag_counts["ECE"] = mutable_int();
}

in_addr basic_flow::get_src_ip() { return this->src; }
in_addr basic_flow::get_dst_ip() { return this->dst; }

net::port_t basic_flow::get_src_port() { return this->src_port; }
net::port_t basic_flow::get_dst_port() { return this->dst_port; }

double basic_flow::get_fpkts_per_second() {
	long duration = this->flow_last_seen - this->flow_start_time;
	if(duration > 0) { return (this->forward.size() / ((double)duration / 1000000L)); }
	else
		return 0;
}

double basic_flow::get_bpkts_per_second() {
	long duration = this->flow_last_seen - this->flow_start_time;
	if(duration > 0) { return (this->backward.size() / ((double)duration / 1000000L)); }
	else
		return 0;
}

long basic_flow::get_favg_bytes_per_bulk() {
	if(this->fbulk_state_count != 0)
		return (this->fbulk_size_total / (double)this->fbulk_state_count);
	return 0;
}

long basic_flow::get_favg_packets_per_bulk() {
	if(this->fbulk_state_count != 0)
		return (this->fbulk_packet_count / (double)this->fbulk_state_count);
	return 0;
}

long basic_flow::get_bavg_bytes_per_bulk() {
	if(this->bbulk_state_count != 0)
		return (this->bbulk_size_total / (double)this->bbulk_state_count);
	return 0;
}

long basic_flow::get_bavg_packets_per_bulk() {
	if(this->bbulk_state_count != 0)
		return (this->bbulk_packet_count / (double)this->bbulk_state_count);
	return 0;
}

long basic_flow::get_favg_bulk_rate() {
	if(this->fbulk_duration != 0)
		return (long)(this->fbulk_size_total / this->get_fbulk_duration_seconds());
	return 0;
}

long basic_flow::get_bavg_bulk_rate() {
	if(this->bbulk_duration != 0)
		return (long)(this->bbulk_size_total / this->get_bbulk_duration_seconds());
	return 0;
}

double basic_flow::get_fbulk_duration_seconds() { return this->fbulk_duration / 1000000.0; }

double basic_flow::get_bbulk_duration_seconds() { return this->bbulk_duration / 1000000.0; }

long basic_flow::get_sflow_fpackets() {
	if(this->sf_count <= 0) return 0;
	return this->forward.size() / this->sf_count;
}

long basic_flow::get_sflow_fbytes() {
	if(this->sf_count <= 0) return 0;
	return this->forward_bytes / this->sf_count;
}

long basic_flow::get_sflow_bpackets() {
	if(this->sf_count <= 0) return 0;
	return this->backward.size() / this->sf_count;
}

long basic_flow::get_sflow_bbytes() {
	if(this->sf_count <= 0) return 0;
	return this->backward_bytes / this->sf_count;
}

double basic_flow::get_flow_packets_s() {
	double flow_packets_s = 0;
	long flow_duration = flow_last_seen - flow_start_time;
	if(flow_duration != 0) flow_packets_s = packet_count() / (flow_duration / 1000000.0);
	return flow_packets_s;
}

double basic_flow::get_flow_bytes_s() {
	long flow_duration = flow_last_seen - flow_start_time;
	double flow_bytes_s = 0;
	if(flow_duration != 0)
		flow_bytes_s = (forward_bytes + backward_bytes) / (flow_duration / 1000000.0);
	return flow_bytes_s;
}

void basic_flow::first_packet(basic_packet_info packet) {
	update_flow_bulk(packet);
	detect_update_subflows(packet);
	check_flags(packet);
	this->flow_start_time = packet.get_timestamp();
	this->flow_last_seen = packet.get_timestamp();
	this->start_active_time = packet.get_timestamp();
	this->end_active_time = packet.get_timestamp();
	this->flow_length_stats.add_value((double)packet.get_payload_bytes());
	this->src = packet.get_src();
	this->dst = packet.get_dst();
	this->src_port = packet.get_src_port();
	this->dst_port = packet.get_dst_port();

	if(inet_ntoa(this->src) == inet_ntoa(packet.get_src())) {
		this->min_seg_size_forward = packet.get_header_bytes();
		this->init_win_bytes_forward = packet.get_tcp_window();
		// this->flow_length_stats.add_value((double)packet.get_payload_bytes());
		this->fwd_pkt_stats.add_value((double)packet.get_payload_bytes());
		this->fHeader_bytes = packet.get_header_bytes();
		this->forward_last_seen = packet.get_timestamp();
		this->forward_bytes += packet.get_payload_bytes();
		this->forward.push_back(packet);
		if(packet.has_PSH()) { this->fPSH_cnt++; }
		if(packet.has_URG()) { this->fURG_cnt++; }
	}
	else {
		this->init_win_bytes_backward = packet.get_tcp_window();
		// this->flow_length_stats.add_value((double)packet.get_payload_bytes());
		this->bwd_pkt_stats.add_value((double)packet.get_payload_bytes());
		this->bHeader_bytes = packet.get_header_bytes();
		this->backward_last_seen = packet.get_timestamp();
		this->backward_bytes += packet.get_payload_bytes();
		this->backward.push_back(packet);
		if(packet.has_PSH()) { this->bPSH_cnt++; }
		if(packet.has_URG()) { this->bURG_cnt++; }
	}
	this->protocol = packet.get_protocol();
	this->flow_id = packet.get_flow_id();
}

void basic_flow::add_packet(basic_packet_info packet) {
	update_flow_bulk(packet);
	detect_update_subflows(packet);
	check_flags(packet);
	long current_timestamp = packet.get_timestamp();
	if(is_bidirectional) {
		this->flow_length_stats.add_value((double)packet.get_payload_bytes());
		if(inet_ntoa(this->src) == inet_ntoa(packet.get_src())) {
			if(packet.get_payload_bytes() >= 1) { this->act_data_pkt_forward++; }
			this->fwd_pkt_stats.add_value((double)packet.get_payload_bytes());
			this->fHeader_bytes += packet.get_header_bytes();

			this->forward.push_back(packet);
			this->forward_bytes += packet.get_payload_bytes();

			if(this->forward.size() > 1)
				this->forward_IAT.add_value(current_timestamp - this->forward_last_seen);

			this->forward_last_seen = current_timestamp;
			this->min_seg_size_forward =
				std::min(packet.get_header_bytes(), this->min_seg_size_forward);
		}
		else {
			this->bwd_pkt_stats.add_value((double)packet.get_payload_bytes());
			init_win_bytes_backward = packet.get_tcp_window();
			this->bHeader_bytes += packet.get_header_bytes();

			this->backward.push_back(packet);
			this->backward_bytes += packet.get_header_bytes();

			this->backward.push_back(packet);
			this->backward_bytes += packet.get_payload_bytes();

			if(this->backward.size() > 1)
				this->backward_IAT.add_value(current_timestamp - this->backward_last_seen);

			this->backward_last_seen = current_timestamp;
		}
	}
	else {
		if(packet.get_payload_bytes() >= 1) { this->act_data_pkt_forward++; }

		this->fwd_pkt_stats.add_value((double)packet.get_payload_bytes());
		this->flow_length_stats.add_value((double)packet.get_payload_bytes());
		this->fHeader_bytes += packet.get_header_bytes();
		this->forward.push_back(packet);
		this->forward_bytes += packet.get_payload_bytes();
		this->forward_IAT.add_value(current_timestamp - this->forward_last_seen);
		this->forward_last_seen = current_timestamp;
		this->min_seg_size_forward =
			std::min(packet.get_header_bytes(), this->min_seg_size_forward);
	}
	// flow statistics
	this->flow_IAT.add_value(packet.get_timestamp() - this->flow_last_seen);
	this->flow_last_seen = packet.get_timestamp();
}

void basic_flow::update_flow_bulk(basic_packet_info packet) {
	if(inet_ntoa(this->src) == inet_ntoa(packet.get_src())) {
		update_forward_bulk(packet, blast_bulk_TS);
	}
	else {
		update_backward_bulk(packet, flast_bulk_TS);
	}
}

void basic_flow::update_forward_bulk(basic_packet_info packet,
									 long timestamp_of_last_bulk_in_other) {
	long size = packet.get_payload_bytes();
	if(timestamp_of_last_bulk_in_other > fbulk_start_helper) fbulk_start_helper = 0;
	if(size <= 0) return;

	packet.get_payload_packet();
	if(fbulk_start_helper == 0) {
		fbulk_start_helper = packet.get_timestamp();
		fbulk_packet_count_helper = 1;
		fbulk_size_helper = size;
		flast_bulk_TS = packet.get_timestamp();
	} //possible bulk
	else {
		/* to much idle time */
		if(((packet.get_timestamp() - flast_bulk_TS) / (double)1000000) > 1.0) {
			//  new bulk
			fbulk_start_helper = packet.get_timestamp();
			flast_bulk_TS = packet.get_timestamp();
			fbulk_packet_count_helper = 1;
			fbulk_size_helper = size;
		}
		else {
			/* Add to a bulk */
			fbulk_packet_count_helper += 1;
			fbulk_size_helper += size;
			//New bulk so bulk should have atleast 4 packets
			if(fbulk_packet_count_helper == 4) {
				fbulk_state_count += 1;
				fbulk_packet_count += fbulk_packet_count_helper;
				fbulk_size_total += fbulk_size_helper;
				fbulk_duration += packet.get_timestamp() - fbulk_start_helper;
			} //Continuation of existing bulk
			else if(fbulk_packet_count_helper > 4) {
				fbulk_packet_count += 1;
				fbulk_size_total += size;
				fbulk_duration += packet.get_timestamp() - flast_bulk_TS;
			}
			flast_bulk_TS = packet.get_timestamp();
		}
	}
}

void basic_flow::update_backward_bulk(basic_packet_info packet,
									  long timestamp_of_last_bulk_in_other) {
	long size = packet.get_payload_bytes();
	if(timestamp_of_last_bulk_in_other > bbulk_start_helper) bbulk_start_helper = 0;
	if(size <= 0) return;

	packet.get_payload_packet();
	if(bbulk_start_helper == 0) {
		bbulk_start_helper = packet.get_timestamp();
		bbulk_packet_count_helper = 1;
		bbulk_size_helper = size;
		blast_bulk_TS = packet.get_timestamp();
	} //possible bulk
	else {
		/* to much idle time */
		if(((packet.get_timestamp() - blast_bulk_TS) / (double)1000000) > 1.0) {
			//  new bulk
			bbulk_start_helper = packet.get_timestamp();
			blast_bulk_TS = packet.get_timestamp();
			bbulk_packet_count_helper = 1;
			bbulk_size_helper = size;
		}
		else {
			/* Add to a bulk */
			bbulk_packet_count_helper += 1;
			bbulk_size_helper += size;
			//New bulk so bulk should have atleast 4 packets
			if(bbulk_packet_count_helper == 4) {
				bbulk_state_count += 1;
				bbulk_packet_count += bbulk_packet_count_helper;
				bbulk_size_total += bbulk_size_helper;
				bbulk_duration += packet.get_timestamp() - bbulk_start_helper;
			} //Continuation of existing bulk
			else if(bbulk_packet_count_helper > 4) {
				bbulk_packet_count += 1;
				bbulk_size_total += size;
				bbulk_duration += packet.get_timestamp() - blast_bulk_TS;
			}
			blast_bulk_TS = packet.get_timestamp();
		}
	}
}

void basic_flow::detect_update_subflows(basic_packet_info packet) {
	/* new subflow */
	if(sf_last_packet_TS == -1) {
		sf_last_packet_TS = packet.get_timestamp();
		sf_ac_helper = packet.get_timestamp();
	}

	if((packet.get_timestamp() - (sf_last_packet_TS) / (double)1000000) > 1.0) {
		sf_count++;
		long lastSFduration = packet.get_timestamp() - sf_ac_helper;
		update_active_idle_time(packet.get_timestamp(), this->activity_timeout);
		sf_ac_helper = packet.get_timestamp();
	}

	sf_last_packet_TS = packet.get_timestamp();
}

void basic_flow::update_active_idle_time(long current_time, long threshold) {
	if((current_time - this->end_active_time) > threshold) {
		/* time exceeded than threshold */
		if((this->end_active_time - this->start_active_time) > 0) {
			this->flow_active.add_value((double)(this->end_active_time - this->start_active_time));
		}
		this->flow_idle.add_value((double)(current_time - this->end_active_time));
		this->start_active_time = current_time;
		this->end_active_time = current_time;
	}
	else {
		this->end_active_time = current_time;
	}
}

void basic_flow::check_flags(basic_packet_info packet) {
	if(packet.has_FIN()) { flag_counts["FIN"].increment(); }
	if(packet.has_SYN()) { flag_counts["SYN"].increment(); }
	if(packet.has_RST()) { flag_counts["RST"].increment(); }
	if(packet.has_PSH()) { flag_counts["PSH"].increment(); }
	if(packet.has_ACK()) { flag_counts["ACK"].increment(); }
	if(packet.has_URG()) { flag_counts["URG"].increment(); }
	if(packet.has_CWR()) { flag_counts["CWR"].increment(); }
	if(packet.has_ECE()) { flag_counts["ECE"].increment(); }
}

long const basic_flow::get_flow_start_time() { return this->flow_start_time; }

int basic_flow::packet_count() {
	if(is_bidirectional) { return this->forward.size() + this->backward.size(); }
	else
		return this->forward.size();
}

double basic_flow::get_down_up_ratio() {
	if(this->forward.size() > 0) { return (this->backward.size() / (double)this->forward.size()); }
	return 0.0;
}

double basic_flow::get_avg_pkt_size() {
	if(this->packet_count() > 0) {
		return (this->flow_length_stats.get_sum() / (double)this->packet_count());
	}
	return 0.0;
}

double basic_flow::get_favg_seg_size() {
	if(this->forward.size() != 0)
		return (this->fwd_pkt_stats.get_sum() / (double)this->forward.size());
	return 0.0;
}

double basic_flow::get_bavg_seg_size() {
	if(this->backward.size() != 0)
		return (this->bwd_pkt_stats.get_sum() / (double)this->backward.size());
	return 0.0;
}

std::string basic_flow::dump_flow_based_features_S() {
	std::string separator = ",";
	std::stringstream dump;
	dump << flow_id << separator;
	dump << inet_ntoa(this->src) << separator;
	dump << this->src_port << separator;
	dump << inet_ntoa(this->dst) << separator;
	dump << this->dst_port << separator;
	dump << this->protocol << separator;

	std::time_t t_c = std::chrono::system_clock::to_time_t(
		std::chrono::system_clock::time_point{std::chrono::seconds(this->flow_start_time)});

	dump << std::put_time(std::localtime(&t_c), "%d/%m/%Y %T") << separator;

	long flow_duration = flow_last_seen - flow_start_time;
	dump << flow_duration << separator;

	dump << (long)fwd_pkt_stats.get_count() << separator;
	dump << (long)bwd_pkt_stats.get_count() << separator;
	dump << (long)fwd_pkt_stats.get_sum() << separator;
	dump << (long)bwd_pkt_stats.get_sum() << separator;

	if(fwd_pkt_stats.get_count() > 0L) {
		dump << fwd_pkt_stats.get_max() << separator;
		dump << fwd_pkt_stats.get_min() << separator;
		dump << fwd_pkt_stats.get_avg() << separator;
		dump << fwd_pkt_stats.get_standard_deviation() << separator;
	}
	else {
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
	}

	if(bwd_pkt_stats.get_count() > 0L) {
		dump << bwd_pkt_stats.get_max() << separator;
		dump << bwd_pkt_stats.get_min() << separator;
		dump << bwd_pkt_stats.get_avg() << separator;
		dump << bwd_pkt_stats.get_standard_deviation() << separator;
	}
	else {
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
	}
	if(flow_duration != 0)
		dump << (forward_bytes + backward_bytes) / (flow_duration / 1000000.0) << separator;
	else
		dump << 0 << separator;
	if(flow_duration != 0) dump << packet_count() / (flow_duration / 1000000.0) << separator;
	else
		dump << 0 << separator;
	dump << flow_IAT.get_avg() << separator;
	dump << flow_IAT.get_standard_deviation() << separator;
	dump << flow_IAT.get_max() << separator;
	dump << flow_IAT.get_min() << separator;

	if(this->forward.size() > 1) {
		dump << forward_IAT.get_sum() << separator;
		dump << forward_IAT.get_avg() << separator;
		dump << forward_IAT.get_standard_deviation() << separator;
		dump << forward_IAT.get_max() << separator;
		dump << forward_IAT.get_min() << separator;
	}
	else {
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
	}

	if(this->backward.size() > 1) {
		dump << backward_IAT.get_sum() << separator;
		dump << backward_IAT.get_avg() << separator;
		dump << backward_IAT.get_standard_deviation() << separator;
		dump << backward_IAT.get_max() << separator;
		dump << backward_IAT.get_min() << separator;
	}
	else {
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
	}

	dump << fPSH_cnt << separator;
	dump << bPSH_cnt << separator;
	dump << fURG_cnt << separator;
	dump << bURG_cnt << separator;

	dump << fHeader_bytes << separator;
	dump << bHeader_bytes << separator;
	dump << get_fpkts_per_second() << separator;
	dump << get_bpkts_per_second() << separator;

	if(this->forward.size() > 0 || this->backward.size() > 0) {
		dump << flow_length_stats.get_min() << separator;
		dump << flow_length_stats.get_max() << separator;
		dump << flow_length_stats.get_avg() << separator;
		dump << flow_length_stats.get_standard_deviation() << separator;
		dump << flow_length_stats.get_variance() << separator;
	}
	else {
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
	}

	dump << flag_counts["FIN"].get() << separator;
	dump << flag_counts["SYN"].get() << separator;
	dump << flag_counts["RST"].get() << separator;
	dump << flag_counts["PSH"].get() << separator;
	dump << flag_counts["ACK"].get() << separator;
	dump << flag_counts["URG"].get() << separator;
	dump << flag_counts["CWR"].get() << separator;
	dump << flag_counts["ECE"].get() << separator;

	dump << get_down_up_ratio() << separator;
	dump << get_avg_pkt_size() << separator;
	dump << get_favg_seg_size() << separator;
	dump << get_bavg_seg_size() << separator;

	dump << get_favg_bytes_per_bulk() << separator;
	dump << get_favg_packets_per_bulk() << separator;
	dump << get_favg_bulk_rate() << separator;
	dump << get_bavg_bytes_per_bulk() << separator;
	dump << get_bavg_packets_per_bulk() << separator;
	dump << get_bavg_bulk_rate() << separator;

	dump << get_sflow_fpackets() << separator;
	dump << get_sflow_fbytes() << separator;
	dump << get_sflow_bpackets() << separator;
	dump << get_sflow_bbytes() << separator;

	dump << init_win_bytes_forward << separator;
	dump << init_win_bytes_backward << separator;
	dump << act_data_pkt_forward << separator;
	dump << min_seg_size_forward << separator;

	if(this->flow_active.get_count() > 0) {
		dump << flow_active.get_avg() << separator;
		dump << flow_active.get_standard_deviation() << separator;
		dump << flow_active.get_max() << separator;
		dump << flow_active.get_min() << separator;
	}
	else {
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
	}

	if(this->flow_idle.get_count() > 0) {
		dump << flow_idle.get_avg() << separator;
		dump << flow_idle.get_standard_deviation() << separator;
		dump << flow_idle.get_max() << separator;
		dump << flow_idle.get_min();
	}
	else {
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0 << separator;
		dump << 0;
	}

	return dump.str();
}

bsoncxx::document::value basic_flow::dump_flow_based_features_to_db() {

	long flow_duration = flow_last_seen - flow_start_time;
	if(flow_duration < 0) flow_duration = 0;
	std::time_t t_c = std::chrono::system_clock::to_time_t(
		std::chrono::system_clock::time_point{std::chrono::seconds(this->flow_start_time)});
	// dump << "Timestamp" << std::put_time(std::localtime(&t_c), "%d/%m/%Y %T");

	double fwd_pkt_stats_max = 0, fwd_pkt_stats_min = 0, fwd_pkt_stats_avg = 0,
		   fwd_pkt_stats_std = 0;
	if(fwd_pkt_stats.get_count() > 0L) {
		fwd_pkt_stats_max = fwd_pkt_stats.get_max();
		fwd_pkt_stats_min = fwd_pkt_stats.get_min();
		fwd_pkt_stats_avg = fwd_pkt_stats.get_avg();
		fwd_pkt_stats_std = fwd_pkt_stats.get_standard_deviation();
	}

	double bwd_pkt_stats_max = 0, bwd_pkt_stats_min = 0, bwd_pkt_stats_avg = 0,
		   bwd_pkt_stats_std = 0;
	if(bwd_pkt_stats.get_count() > 0L) {
		bwd_pkt_stats_max = bwd_pkt_stats.get_max();
		bwd_pkt_stats_min = bwd_pkt_stats.get_min();
		bwd_pkt_stats_avg = bwd_pkt_stats.get_avg();
		bwd_pkt_stats_std = bwd_pkt_stats.get_standard_deviation();
	}

	double fwd_iat_sum = 0, fwd_iat_avg = 0, fwd_iat_std = 0, fwd_iat_max = 0, fwd_iat_min = 0;
	if(this->forward.size() > 1) {
		fwd_iat_sum = forward_IAT.get_sum();
		fwd_iat_avg = forward_IAT.get_avg();
		fwd_iat_std = forward_IAT.get_standard_deviation();
		fwd_iat_max = forward_IAT.get_max();
		fwd_iat_min = forward_IAT.get_min();
	}

	double bwd_iat_sum = 0, bwd_iat_avg = 0, bwd_iat_std = 0, bwd_iat_max = 0, bwd_iat_min = 0;
	if(this->backward.size() > 1) {
		bwd_iat_sum = backward_IAT.get_sum();
		bwd_iat_avg = backward_IAT.get_avg();
		bwd_iat_std = backward_IAT.get_standard_deviation();
		bwd_iat_max = backward_IAT.get_max();
		bwd_iat_min = backward_IAT.get_min();
	}

	double flow_len_stats_min = 0, flow_len_stats_max = 0, flow_len_stats_avg = 0,
		   flow_len_stats_std = 0, flow_len_stats_var = 0;
	if(this->forward.size() > 0 || this->backward.size() > 0) {
		flow_len_stats_min = flow_length_stats.get_min();
		flow_len_stats_max = flow_length_stats.get_max();
		flow_len_stats_avg = flow_length_stats.get_avg();
		flow_len_stats_std = flow_length_stats.get_standard_deviation();
		flow_len_stats_var = flow_length_stats.get_variance();
	}

	double active_avg = 0, active_std = 0, active_max = 0, active_min = 0;
	if(this->flow_active.get_count() > 0) {
		active_avg = flow_active.get_avg();
		active_std = flow_active.get_standard_deviation();
		active_max = flow_active.get_max();
		active_min = flow_active.get_min();
	}

	double idle_avg = 0, idle_std = 0, idle_max = 0, idle_min = 0;
	if(this->flow_idle.get_count() > 0) {
		idle_avg = flow_idle.get_avg();
		idle_std = flow_idle.get_standard_deviation();
		idle_max = flow_idle.get_max();
		idle_min = flow_idle.get_min();
	}

	// clang-format off

	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder
		<< "Flow ID" << flow_id 
		<< "Source IP" << inet_ntoa(this->src) 
		<< "Source Port" << this->src_port 
		<< "Destination IP" << inet_ntoa(this->dst) 
		<< "Destination Port" << this->dst_port
		// << "Protocol" << this->protocol
		<< "Flow Duration" << flow_duration
	 	<< "Total Fwd Packets" << (long)fwd_pkt_stats.get_count()
	 	<< "Total Backward Packets" << (long)bwd_pkt_stats.get_count()
	 	<< "Total Length of Fwd Packets" << (long)fwd_pkt_stats.get_sum()
	 	<< "Total Length of Bwd Packets" << (long)bwd_pkt_stats.get_sum()
		<< "Fwd Packet Length Max" << fwd_pkt_stats_max
		<< "Fwd Packet Length Min" << fwd_pkt_stats_min
		<< "Fwd Packet Length Mean" << fwd_pkt_stats_avg
		<< "Fwd Packet Length Std" << fwd_pkt_stats_std
		<< "Bwd Packet Length Max" << bwd_pkt_stats_max
		<< "Bwd Packet Length Min" << bwd_pkt_stats_min
		<< "Bwd Packet Length Mean" << bwd_pkt_stats_avg
		<< "Bwd Packet Length Std" << bwd_pkt_stats_std
		<< "Flow Bytes/s" << get_flow_bytes_s()
		<< "Flow Packets/s"<< get_flow_packets_s()
		<<"Flow IAT Mean"<< flow_IAT.get_avg()
		<<"Flow IAT Std" << flow_IAT.get_standard_deviation()
		<<"Flow IAT Max"<< flow_IAT.get_max()
		<<"Flow IAT Min" << flow_IAT.get_min()
		<< "Fwd IAT Total" << fwd_iat_sum
		<< "Fwd IAT Mean" << fwd_iat_avg
		<< "Fwd IAT Std" << fwd_iat_std
		<< "Fwd IAT Max" << fwd_iat_max
		<< "Fwd IAT Min" << fwd_iat_min
		<< "Bwd IAT Total" << bwd_iat_sum
		<< "Bwd IAT Mean" <<bwd_iat_avg
		<< "Bwd IAT Std" << bwd_iat_std
		<< "Bwd IAT Max" << bwd_iat_max
		<< "Bwd IAT Min" << bwd_iat_min
		<< "Fwd PSH Flags" << fPSH_cnt
		<< "Bwd PSH Flags" << bPSH_cnt
		<< "Fwd URG Flags" << fURG_cnt
		<< "Bwd URG Flags" << bURG_cnt
		<< "Fwd Header Length" << fHeader_bytes
		<< "Bwd Header Length" << bHeader_bytes
		<< "Fwd Packets/s" << get_fpkts_per_second()
		<< "Bwd Packets/s" << get_bpkts_per_second()
		<< "Min Packet Length" << flow_len_stats_min
		<< "Max Packet Length" << flow_len_stats_max
		<< "Packet Length Mean" << flow_len_stats_avg
		<< "Packet Length Std" << flow_len_stats_std
		<< "Packet Length Variance" << flow_len_stats_var
		<< "FIN Flag Count" << flag_counts["FIN"].get()
		<< "SYN Flag Count" << flag_counts["SYN"].get()
		<< "RST Flag Count" << flag_counts["RST"].get()
		<< "PSH Flag Count" << flag_counts["PSH"].get()
		<< "ACK Flag Count" << flag_counts["ACK"].get()
		<< "URG Flag Count" << flag_counts["URG"].get()
		<< "CWR Flag Count" << flag_counts["CWR"].get()
		<< "ECE Flag Count" << flag_counts["ECE"].get()
		<< "Down/Up Ratio" << get_down_up_ratio()
		<< "Average Packet Size" << get_avg_pkt_size()
		<< "Avg Fwd Segment Size" << get_favg_seg_size()
		<< "Avg Bwd Segment Size" << get_bavg_seg_size()
		<< "Fwd Header Length_1" << fwd_pkt_stats.get_count()
		<< "Fwd Avg Bytes/Bulk" << get_favg_bytes_per_bulk()
		<< "Fwd Avg Packets/Bulk" << get_favg_packets_per_bulk()
		<< "Fwd Avg Bulk Rate" << get_favg_bulk_rate()
		<< "Bwd Avg Bytes/Bulk" << get_bavg_bytes_per_bulk()
		<< "Bwd Avg Packets/Bulk" << get_bavg_packets_per_bulk()
		<< "Bwd Avg Bulk Rate" << get_bavg_bulk_rate()
		<< "Subflow Fwd Packets" << get_sflow_fpackets()
		<< "Subflow Fwd Bytes" << get_sflow_fbytes()
		<< "Subflow Bwd Packets" << get_sflow_bpackets()
		<< "Subflow Bwd Bytes" << get_sflow_bbytes()
		<< "Init_Win_bytes_forward" << init_win_bytes_forward
		<< "Init_Win_bytes_backward" << init_win_bytes_backward
		<< "act_data_pkt_fwd" << act_data_pkt_forward
		<< "min_seg_size_forward" << min_seg_size_forward
		<< "Active Mean" << active_avg
		<< "Active Std" << active_std
		<< "Active Max" << active_max
		<< "Active Min" << active_min
		<< "Idle Mean" << idle_avg
		<< "Idle Std" << idle_std
		<< "Idle Max" << idle_max
		<< "Idle Min" << idle_min
  		<< bsoncxx::builder::stream::finalize;

	// clang-format on

	fprintf(stderr, "Made stream\n");
	return doc_value;
}
