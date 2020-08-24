#include "packet_parser.hpp"

#include <arpa/inet.h>
#include <bitset>
#include <iostream>
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

std::vector<basic_packet_info> packet_parser::get_basic_pkts() { return basic_pkts; }

const struct tcp_header* const packet_parser::get_tcp() { return tcp; }
const struct ip_header* const packet_parser::get_ip() { return ip; }
const struct ether_header* const packet_parser::get_ether() { return ether; }

bool packet_parser::handle_tcp(basic_packet_info* pkt) {
	if(!set_tcp()) return false;
	pkt->set_src_port((net::port_t)(tcp->th_sport));
	pkt->set_dst_port((net::port_t)(tcp->th_dport));
	std::bitset<8> fs(tcp->th_flags);
	// std::cerr << "Flags :" << fs.to_string() << std::endl;

	pkt->set_flagFIN(tcp->th_flags & TH_FIN);
	pkt->set_flagPSH(tcp->th_flags & TH_PUSH);
	pkt->set_flagURG(tcp->th_flags & TH_URG);
	pkt->set_flagECE(tcp->th_flags & TH_ECE);
	pkt->set_flagSYN(tcp->th_flags & TH_SYN);
	pkt->set_flagACK(tcp->th_flags & TH_ACK);
	pkt->set_flagCWR(tcp->th_flags & TH_CWR);
	pkt->set_flagRST(tcp->th_flags & TH_RST);
	pkt->set_TCPwindow(tcp->th_win);

	return true;
}

bool packet_parser::handle_udp(basic_packet_info* pkt) {
	if(!set_udp()) return false;
	pkt->set_src_port(udp->srcport);
	pkt->set_dst_port(udp->destport);
	return true;
}

bool packet_parser::parse() {
	set_ether();

	if(!set_ip()) {
		fprintf(stderr, "invalid ip\n");
		return false;
	}
	basic_packet_info pkt;
	pkt.set_src(ip->ip_src);
	pkt.set_dst(ip->ip_dst);
	pkt.set_protocol(ip->ip_p);
	pkt.set_timestamp(header->ts.tv_sec);

	bool captured = true;
	switch(ip->ip_p) {
	case IPPROTO_TCP:
		// printf("   Protocol: TCP\n");
		captured = handle_tcp(&pkt);
		break;
	case IPPROTO_UDP:
		// printf("   Protocol: UDP\n");
		captured = handle_udp(&pkt);
		break;
	// case IPPROTO_ICMP:
	// 	printf("   Protocol: ICMP\n");
	// 	return;
	// case IPPROTO_IP:
	// 	printf("   Protocol: IP\n");
	// 	return;
	default:
		return captured = false;
	}
	if(captured) {
		pkt.set_id();
		basic_pkts.push_back(pkt);
	}

	return captured;
}
