#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <netinet/in.h>

namespace net {
using port_t = uint16_t;
using addr_t = uint16_t;
inline constexpr uint8_t ether_address_len{6};
inline constexpr uint8_t size_ethernet{14};
} // namespace net
#endif