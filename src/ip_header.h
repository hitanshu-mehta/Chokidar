#ifndef IP_HEADER_H
#define IP_HEADER_H

#include <netinet/in.h>
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

class ip_header
{
	using addr_t = uint32_t;
	using port_t = uint16_t;

private:
	uint8_t ver_ihl; // 4 bits version and 4 bits internet header length
	uint8_t tos;
	uint16_t total_length;
	uint16_t id;
	uint16_t flags_fo; // 3 bits flags and 13 bits fragment-offset
	uint8_t ttl;
	uint8_t protocol;
	uint16_t checksum;
	addr_t src_addr;
	addr_t dst_addr;

	/* ver_ihl is 8 bits long and ihl is 4 bit. so ANDing xxxxyyyy & 00001111 gives yyyy which is ihl */
	uint8_t ihl() const { return (ver_ihl & 0x0F); }

	size_t size() const { return ihl() * sizeof(uint32_t); }

public:
};

#endif