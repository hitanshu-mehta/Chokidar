#ifndef TCP_HEADER_HPP
#define TCP_HEADER_HPP

#include "constants.hpp"
#include <cstdint>

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

/* TCP header */

struct tcp_header {
	u_short th_sport; /* source port */
	u_short th_dport; /* destination port */
	uint32_t th_seq;  /* sequence number */
	uint32_t th_ack;  /* acknowledgement number */
	uint8_t th_offx2; /* data offset, rsvd */
#define TH_OFF(th) (((th)->th_offx2 & 0xf0) >> 4)
	uint8_t th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN | TH_SYN | TH_RST | TH_PUSH | TH_ACK | TH_URG | TH_ECE | TH_CWR)
	uint16_t th_win; /* window */
	uint16_t th_sum; /* checksum */
	uint16_t th_urp; /* urgent pointer */
};

#endif