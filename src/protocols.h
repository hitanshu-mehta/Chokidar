#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include "ether_header.h"
#include "ip_header.h"
#include "tcp_header.h"
#include "udp_header.h"

class protocols
{
private:
	ip_header* ip;
	udp_header* udp;
	tcp_header* tcp;
	ether_header* ether;

public:
	protocols() {
		ip = new ip_header;
		udp = new udp_header;
		tcp = new tcp_header;
		ether = new ether_header;
	}

	ip_header* get_ip() { return ip; }
	ether_header* get_ether() { return ether; }
	udp_header* get_udp() { return udp; }
	tcp_header* get_tcp() { return tcp; }
};

#endif