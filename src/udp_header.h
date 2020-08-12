#ifndef UDP_HEADER_H
#define UDP_HEADER_H

#include "constants.h"
#include <netinet/in.h>

/*
Taken from RFC 768

        0      7 8     15 16    23 24    31
        +--------+--------+--------+--------+
        |     Source      |   Destination   |
        |      Port       |      Port       |
        +--------+--------+--------+--------+
        |                 |                 |
        |     Length      |    Checksum     |
        +--------+--------+--------+--------+
        |
        |          data octets ...
        +---------------- ...

            User Datagram Header Format
*/

class udp_header
{

private:
	net::port_t src_port;
	net::port_t dst_port;
	uint16_t length;
	uint16_t checksum;
};

#endif