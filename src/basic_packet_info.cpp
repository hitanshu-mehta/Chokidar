#include "basic_packet_info.h"
#include <arpa/inet.h>

basic_packet_info::basic_packet_info(
	in_addr src, in_addr dst, net::port_t src_pt, net::port_t dst_pt, int proto, long ts)
	: ip_src(src)
	, ip_dst(dst)
	, src_port(src_pt)
	, dst_port(dst_pt)
	, protocol(proto)
	, timestamp(ts){

	  };

void const basic_packet_info::print_all_info() {
	fprintf(stderr, "|-----------------Packet Info----------------|\n");
	fprintf(stderr, "Source ip: %s\n", inet_ntoa(ip_src));
	fprintf(stderr, "Destination ip: %s\n", inet_ntoa(ip_dst));
	fprintf(stderr, "Source port: %d\n", ntohs(src_port));
	fprintf(stderr, "Destination port: %d\n", ntohs(dst_port));
	fprintf(stderr, "Protocol: %d\n", protocol);
	fprintf(stderr, "Timestamp: %ld\n", timestamp);
	fprintf(stderr, "TCP window: %d\n", TCPwindow);
	// fprintf(stderr, "Flags %d\n", all_flags);
}