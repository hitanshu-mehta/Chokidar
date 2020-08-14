#ifndef PACKET_PARSER_H
#define PACKET_PARSER_H

#include "basic_packet_info.h"
#include "ether_header.h"
#include "ip_header.h"
#include "tcp_header.h"
#include "udp_header.h"
#include <cstdint>
#include <pcap.h>
#include <vector>

class packet_parser
{
private:
	inline static long long count = 0;
	const struct ether_header* ether;
	const struct ip_header* ip;
	const struct tcp_header* tcp;
	const struct udp_header* udp;
	const struct pcap_pkthdr* header;
	const uint8_t* packet;
	uint8_t* args;
	uint size_ip;
	uint size_tcp;
	std::vector<basic_packet_info> basic_pkts;

public:
	packet_parser(){};
	packet_parser(const struct pcap_pkthdr*, const uint8_t*, uint8_t*);
	bool set_ip();
	bool set_ether();
	bool set_tcp();
	bool set_udp();

	std::vector<basic_packet_info> get_basic_pkts();
	const struct tcp_header* const get_tcp();
	const struct ip_header* const get_ip();
	const struct ether_header* const get_ether();

	void set_header(const struct pcap_pkthdr*);
	void set_packet(const uint8_t*);
	void set_args(uint8_t*);

	bool handle_tcp(net::port_t*, net::port_t*);
	bool handle_udp(net::port_t*, net::port_t*);

	bool parse();
};

#endif