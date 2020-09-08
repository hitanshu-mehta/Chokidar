#include "packet_parser.hpp"
#include "session_stats.hpp"

#include <arpa/inet.h>
#include <bitset>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

#define ETHERTYPE_IP 0x0800
#define ETHERTYPE_IPv6 0x8100
#define ETHERTYPE_ARP 0x0806

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

void packet_parser::update_stats(bool captured, basic_packet_info* pkt) {
	session_stats* stats = session_stats::get_instance();

	fprintf(stderr, "\t%d\n", ether->ether_type);
	switch(ether->ether_type) {
	case ETHERTYPE_ARP:
		stats->add_to_arp_count(1);
		break;
	case ETHERTYPE_IPv6:
		stats->add_to_ipv6_count(1);
		break;
	case ETHERTYPE_IP:
		// stats->add_to_ip_count(1);
		break;
	}

	if(captured) {
		std::string str(inet_ntoa(ip->ip_src));
		stats->add_uni_ip(str, 1);
		stats->add_to_ip_count(1);
		stats->add_no_pkts_captured(1);
		switch(ip->ip_p) {
		case IPPROTO_TCP:
			stats->add_to_tcp_count(1);
			break;
		case IPPROTO_UDP:
			stats->add_to_udp_count(1);
			break;
		case IPPROTO_ICMP:
			stats->add_to_icmp_count(1);
			break;
		case IPPROTO_DCCP:
			stats->add_to_dccp_count(1);
		default:
			break;
		}
	}
	else
		stats->add_no_pkts_discarded(1);
	stats->add_to_total_bytes(pkt->get_payload_bytes());
	int protocol = pkt->get_protocol();
}

bool packet_parser::parse() {
	set_ether();

	basic_packet_info pkt;
	bool captured = true;

	if(set_ip()) {

		pkt.set_src(ip->ip_src);
		pkt.set_dst(ip->ip_dst);
		pkt.set_protocol(ip->ip_p);
		pkt.set_timestamp(header->ts.tv_usec);
		switch(ip->ip_p) {
		case IPPROTO_TCP:
			// printf("   Protocol: TCP\n");
			captured = handle_tcp(&pkt);
			pkt.set_payload_bytes(ntohs(ip->ip_len) - (size_ip + size_tcp));
			pkt.set_header_bytes(size_tcp);
			break;
		case IPPROTO_UDP:
			// printf("   Protocol: UDP\n");
			captured = handle_udp(&pkt);
			pkt.set_header_bytes(8); // fix header of 8 bytes

			// len contains size (in bytes) of udp pkt (header + data)
			pkt.set_payload_bytes(udp->len - 8);
			break;
		// case IPPROTO_ICMP:
		// 	printf("   Protocol: ICMP\n");
		// 	return;
		// case IPPROTO_IP:
		// 	printf("   Protocol: IP\n");
		// 	return;
		default:
			captured = false;
		}

		if(captured) {
			pkt.set_id();
			basic_pkts.push_back(pkt);
		}
	}

	update_stats(captured, &pkt);

	return captured;
}
