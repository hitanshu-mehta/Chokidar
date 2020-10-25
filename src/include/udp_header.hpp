#ifndef UDP_HEADER_HPP
#define UDP_HEADER_HPP

#include "src/include/constants.hpp"
#include <cstdint>

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
struct udp_header {

	uint16_t srcport;
	uint16_t destport;
	uint16_t len;
	uint16_t chksum;
};
#endif