#include "basic_packet_info.hpp"
#include <arpa/inet.h>
#include <sstream>

basic_packet_info::basic_packet_info(
	in_addr source, in_addr desti, net::port_t src_pt, net::port_t dst_pt, int proto, long ts)
	: src(source)
	, dst(desti)
	, src_port(src_pt)
	, dst_port(dst_pt)
	, protocol(proto)
	, timestamp(ts) {
	payload_bytes = 0;
	header_bytes = 0;
};

std::string basic_packet_info::get_flow_id() {
	bool forward = true;

	std::string src_s = inet_ntoa(this->src);
	std::string dst_s = inet_ntoa(this->src);
	for(int i = 0; i < src_s.length(); i++) {
		if(src_s[i] != dst_s[i]) {
			if(src_s[i] > dst_s[i]) { forward = false; }
			i = src_s.length();
		}
	}
	std::stringstream f_id;
	if(forward) {
		f_id << inet_ntoa(this->get_src()) << "-" << inet_ntoa(this->get_dst()) << "-"
			 << this->src_port << "-" << this->dst_port << "-" << this->protocol;
	}
	else {
		f_id << inet_ntoa(this->get_dst()) << "-" << inet_ntoa(this->get_src()) << "-"
			 << this->dst_port << "-" << this->src_port << "-" << this->protocol;
	}
	return this->flowid = f_id.str();
}

void const basic_packet_info::print_all_info() {
	fprintf(stderr, "|-----------------Packet Info----------------|\n");
	fprintf(stderr, "Source ip: %s\n", inet_ntoa(src));
	fprintf(stderr, "Destination ip: %s\n", inet_ntoa(dst));
	fprintf(stderr, "Source port: %d\n", ntohs(src_port));
	fprintf(stderr, "Destination port: %d\n", ntohs(dst_port));
	fprintf(stderr, "Protocol: %d\n", protocol);
	fprintf(stderr, "Timestamp: %ld\n", timestamp);
	fprintf(stderr, "TCP window: %d\n", tcp_window);
	fprintf(stderr, "header bytes: %ld\n", header_bytes);
	fprintf(stderr, "payload bytes bytes: %d\n", payload_bytes);
	// fprintf(stderr, "Flags %d\n", all_flags);
}

std::string basic_packet_info::get_fwd_flow_id() {

	std::stringstream f_id;

	f_id << inet_ntoa(this->get_src()) << "-" << inet_ntoa(this->get_dst()) << "-" << this->src_port
		 << "-" << this->dst_port << "-" << this->protocol;
	return this->flowid = f_id.str();
}
std::string basic_packet_info::get_bwd_flow_id() {
	std::stringstream f_id;

	f_id << inet_ntoa(this->get_dst()) << "-" << inet_ntoa(this->get_src()) << "-" << this->dst_port
		 << "-" << this->src_port << "-" << this->protocol;

	return this->flowid = f_id.str();
}