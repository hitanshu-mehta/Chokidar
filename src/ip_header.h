#ifndef IP_HEADER_H
#define IP_HEADER_H

#include "constants.h"
#include <cstdint>

/*
 * structure of ip header.
 * Taken from RFC 791's protocol specification
 * Internet Header Format

       A summary of the contents of the internet header follows: 

           0                   1                   2                   3   
           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
          |Version|  IHL  |Type of Service|          Total Length         |
          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
          |         Identification        |Flags|      Fragment Offset    |
          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
          |  Time to Live |    Protocol   |         Header Checksum       |
          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
          |                       Source Address                          |
          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
          |                    Destination Address                        |
          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
          |                    Options                    |    Padding    |
          +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        
                           Example Internet Datagram Header
        
         Note that each tick mark represents one bit position.
 * 
 * 
 */

/* IP header */
struct ip_header {
	uint8_t ip_vhl;				   /* version << 4 | header length >> 2 */
	uint8_t ip_tos;				   /* type of service */
	uint16_t ip_len;			   /* total length */
	uint16_t ip_id;				   /* identification */
	uint16_t ip_off;			   /* fragment offset field */
#define IP_RF 0x8000			   /* reserved fragment flag */
#define IP_DF 0x4000			   /* dont fragment flag */
#define IP_MF 0x2000			   /* more fragments flag */
#define IP_OFFMASK 0x1fff		   /* mask for fragmenting bits */
	uint8_t ip_ttl;				   /* time to live */
	uint8_t ip_p;				   /* protocol */
	uint16_t ip_sum;			   /* checksum */
	struct in_addr ip_src, ip_dst; /* source and dest address */
};
#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip) (((ip)->ip_vhl) >> 4)

#endif