#ifndef PACKET_PARSER_H
#define PACKET_PARSER_H

#include "ether_header.h"
#include "ip_header.h"
#include "tcp_header.h"
// #include "udp_header.h"
#include <cstdint>

class packet_parser
{
private:
	inline static long long count = 0;
	const struct ether_header* ether;
	const struct ip_header* ip;
	const struct tcp_header* tcp;
	const struct pcap_pkthdr* header;
	const uint8_t* packet;
	uint8_t* args;
	uint size_ip;
	uint size_tcp;

public:
	packet_parser(){};
	packet_parser(const struct pcap_pkthdr* header, const uint8_t* packet, uint8_t* args);
	bool set_ip();
	bool set_ether();
	bool set_tcp();
	bool set_udp();

	const struct tcp_header* const get_tcp();
	const struct ip_header* const get_ip();
	const struct ether_header* const get_ether();

	void set_header(const struct pcap_pkthdr*);
	void set_packet(const uint8_t*);
	void set_args(uint8_t*);

	bool parse();
};

#endif