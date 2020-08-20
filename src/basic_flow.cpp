#include "basic_flow.h"
#include <arpa/inet.h>

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

void basic_flow::first_packet(basic_packet_info packet) {
	update_flow_bulk(packet);
	detect_update_subflows(packet);
	check_flags(packet);
	this->flow_start_time = packet.get_timestamp();
	this->flow_last_seen = packet.get_timestamp();
	this->start_active_time = packet.get_timestamp();
	this->end_active_time = packet.get_timestamp();
	this->flow_length_stats.add_value((double)packet.get_payload_bytes());
	this->src = packet.get_ip_src();
	this->dst = packet.get_ip_dst();
	this->src_port = packet.get_src_port();
	this->dst_port = packet.get_dst_port();

	if(inet_ntoa(this->src) == inet_ntoa(packet.get_ip_src())) {
		this->min_seg_size_forward = packet.get_header_bytes();
		this->init_win_bytes_forward = packet.get_tcp_window();
		this->flow_length_stats.add_value((double)packet.get_payload_bytes());
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
		this->flow_length_stats.add_value((double)packet.get_payload_bytes());
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
		if(inet_ntoa(this->src) == inet_ntoa(packet.get_ip_src())) {
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
	if(inet_ntoa(this->src) == inet_ntoa(packet.get_ip_src())) {
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
