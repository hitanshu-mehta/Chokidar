#ifndef TCP_HEADER_H
#define TCP_HEADER_H

#include "constants.h"
#include <netinet/in.h>

/*
*        0                   1                   2                   3
*        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
*       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*       |          Source Port          |       Destination Port        |
*       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*       |                        Sequence Number                        |
*       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*       |                    Acknowledgment Number                      |
*       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*       |  Data | Re    |C|E|U|A|P|R|S|F|                               |
*       | Offset|-servd |W|C|R|C|S|S|Y|I|            Window             |
*       |       |       |R|E|G|K|H|T|N|N|                               |
*       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*       |           Checksum            |         Urgent Pointer        |
*       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*
*
*/

class tcp_header
{

private:
	net::port_t src_port; /* source port */
	net::port_t dst_port; /* destination port */
	uint32_t seq_no;	  /* sequence  number */
	uint32_t ack_no;	  /* acknowledgment number */
	u_char offx2;		  /* data offset and reserved (10 bits) */
	u_char flags;		  /* flags */

	static constexpr uint8_t TH_FIN{0x01};
	static constexpr uint8_t TH_SYN{0x02};
	static constexpr uint8_t TH_RST{0x04};
	static constexpr uint8_t TH_PUSH{0x08};
	static constexpr uint8_t TH_ACK{0x10};
	static constexpr uint8_t TH_URG{0x20};
	static constexpr uint8_t TH_ECE{0x40};
	static constexpr uint8_t TH_CWR{0x80};
	static constexpr uint8_t TH_FLAGS =
		(TH_FIN | TH_SYN | TH_RST | TH_ACK | TH_URG | TH_ECE | TH_CWR);

	uint16_t win;	   /* window */
	uint16_t checksum; /* checksum */
	uint16_t urg_ptr;  /* urgent pointer */
	u_char off() const { return ((offx2 & 0xF0) >> 4); }
};

#endif