#ifndef ETHER_HEADER_HPP
#define ETHER_HEADER_HPP

#include "constants.hpp"
#include <cstdint>

/* Ethernet header */
struct ether_header {
	uint8_t ether_dhost[net::ether_address_len]; /* Destination host address */
	uint8_t ether_shost[net::ether_address_len]; /* Source host address */
	uint16_t ether_type;						 /* IP? ARP? RARP? etc */
};

#endif