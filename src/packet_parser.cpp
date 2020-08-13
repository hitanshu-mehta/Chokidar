#include "packet_parser.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

packet_parser::packet_parser(const struct pcap_pkthdr* headr, const uint8_t* pkt, uint8_t* args)
	: header(headr)
	, packet(pkt)
	, args(args){};

bool packet_parser::set_ether() {
	ether = (struct ether_header*)packet;
	return true;
}

bool packet_parser::set_ip() {
	ip = (struct ip_header*)(packet + net::size_ethernet);
	size_ip = IP_HL(ip) * 4;
	return (size_ip >= 20);
}

bool packet_parser::set_tcp() {
	if(ip->ip_p != IPPROTO_TCP) return false;
	tcp = (struct tcp_header*)(packet + net::size_ethernet + size_ip);
	size_tcp = TH_OFF(tcp) * 4;
	return (size_tcp >= 20);
}

bool packet_parser::set_udp() {
	if(ip->ip_p != IPPROTO_UDP) return false;
	udp = (struct udp_header*)(packet + net::size_ethernet + size_ip);
	return true;
}

void packet_parser::set_header(const struct pcap_pkthdr* header) { this->header = header; }

void packet_parser::set_args(uint8_t* args) { this->args = args; }

void packet_parser::set_packet(const uint8_t* packet) { this->packet = packet; }

const struct tcp_header* const packet_parser::get_tcp() { return tcp; }
const struct ip_header* const packet_parser::get_ip() { return ip; }
const struct ether_header* const packet_parser::get_ether() { return ether; }

bool packet_parser::handle_tcp(net::port_t* s_port, net::port_t* d_port) {
	if(!set_tcp()) return false;
	*s_port = (net::port_t)(tcp->th_sport);
	*d_port = (net::port_t)(tcp->th_dport);
	return true;
}

bool packet_parser::handle_udp(net::port_t* s_port, net::port_t* d_port) {
	if(!set_udp()) return false;
	*s_port = (net::port_t)(udp->srcport);
	*d_port = (net::port_t)(udp->destport);
	return true;
}
bool packet_parser::parse() {
	set_ether();

	if(!set_ip()) {
		fprintf(stderr, "invalid ip\n");
		return false;
	}
	in_addr s_ip = ip->ip_src, d_ip = ip->ip_dst;
	int protocol = ip->ip_p;
	long timestamp = header->ts.tv_sec;
	net::port_t s_port, d_port;

	bool captured = true;
	switch(ip->ip_p) {
	case IPPROTO_TCP:
		captured = handle_tcp(&s_port, &d_port);
		// printf("   Protocol: TCP\n");
		break;
	case IPPROTO_UDP:
		captured = handle_udp(&s_port, &d_port);
		// printf("   Protocol: UDP\n");
		break;
	// case IPPROTO_ICMP:
	// 	printf("   Protocol: ICMP\n");
	// 	return;
	// case IPPROTO_IP:
	// 	printf("   Protocol: IP\n");
	// 	return;
	default:
		captured = false;
		return;
	}
	if(captured) {
		basic_packet_info* basic_pkt =
			new basic_packet_info(s_ip, d_ip, s_port, d_port, protocol, timestamp);
	}

	return captured;
}
