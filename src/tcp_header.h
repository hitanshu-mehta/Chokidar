#ifndef TCP_HEADER_H
#define TCP_HEADER_H

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
	using port_t = uint16_t;

private:
	port_t src_port; /* source port */
	port_t dst_port; /* destination port */
	uint32_t seq_no; /* sequence  number */
	uint32_t ack_no; /* acknowledgment number */
	u_char offx2;	 /* data offset and reserved (10 bits) */
	u_char flags;	 /* flags */

#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN | TH_SYN | TH_RST | TH_ACK | TH_URG | TH_ECE | TH_CWR)
	uint16_t win;	   /* window */
	uint16_t checksum; /* checksum */
	uint16_t urg_ptr;  /* urgent pointer */
	u_char off() const { return ((offx2 & 0xF0) >> 4); }
};

#endif