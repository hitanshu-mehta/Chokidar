#ifndef PACKET_PARSER_HPP
#define PACKET_PARSER_HPP

#include "basic_packet_info.hpp"
#include "ether_header.hpp"
#include "ip_header.hpp"
#include "tcp_header.hpp"
#include "udp_header.hpp"
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

	bool handle_tcp(basic_packet_info*);
	bool handle_udp(basic_packet_info*);

	bool parse();

	void update_stats(bool, basic_packet_info*);
};

#endif