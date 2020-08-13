#include "packet_parser.h"
#include <arpa/inet.h>
#include <bitset>
#include <cstdio>
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

void packet_parser::set_header(const struct pcap_pkthdr* header) { this->header = header; }

void packet_parser::set_args(uint8_t* args) { this->args = args; }

void packet_parser::set_packet(const uint8_t* packet) { this->packet = packet; }

const struct tcp_header* const packet_parser::get_tcp() { return tcp; }
const struct ip_header* const packet_parser::get_ip() { return ip; }
const struct ether_header* const packet_parser::get_ether() { return ether; }

bool packet_parser::parse() {
	set_ether();
	fprintf(stderr, "Packet count: %lld\n", ++count);
	fprintf(stderr, "Parsing...\n");
	fprintf(stderr, "Ethernet\n");
	// std::bitset<16> ed(ether->ether_dhost);
	// std::bitset<16> es(ether->ether_shost);
	// fprintf(stderr, "Destination host: %s\n", );
	// fprintf(stderr, "Source host: %s\n", ed.to_string());
	if(!set_ip()) {
		fprintf(stderr, "invalid ip\n");
		return false;
	}
	fprintf(stderr, "\tIP\n");
	fprintf(stderr, "\tDestination ip: %s\n", inet_ntoa(ip->ip_dst));
	fprintf(stderr, "\tSource ip: %s\n", inet_ntoa(ip->ip_src));
	if(!set_tcp()) {
		fprintf(stderr, "invalid tcp\n");
		return false;
	}
	fprintf(stderr, "\t\tTCP\n");
	fprintf(stderr, "\t\tDestination port: %d\n", ntohs(tcp->th_dport));
	fprintf(stderr, "\t\tSource port: %d\n", ntohs(tcp->th_sport));
	return true;
}
