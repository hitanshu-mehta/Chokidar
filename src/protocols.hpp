#ifndef PROTOCOLS_HPP
#define PROTOCOLS_HPP

#include "ether_header.hpp"
#include "ip_header.hpp"
#include "tcp_header.hpp"
#include "udp_header.hpp"

class protocols
{
private:
	const struct ether_header* ether;
	const struct ip_header* ip;
	const struct tcp_header* tcp;
	const struct udp_header* udp;

public:
	protocols() {
		ip = new ip_header;
		udp = new udp_header;
		tcp = new tcp_header;
		ether = new ether_header;
	}

	const ip_header* get_ip() { return ip; }
	const ether_header* get_ether() { return ether; }
	const udp_header* get_udp() { return udp; }
	const tcp_header* get_tcp() { return tcp; }
	// void set_ether(struct ether_header* ether_h) { ether = ether_h; }
	// void set_ip(struct ip_header* ip_h) { ip = ip_h; }
	// void set_tcp(struct tcp_header* tcp_h) { tcp = tcp_h; }
	// void set_udp(struct udp_header* udp_h) { udp = udp_h; }
};

#endif