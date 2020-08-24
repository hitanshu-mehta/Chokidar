#ifndef BASIC_CAPTURE_ENGINE_HPP
#define BASIC_CAPTURE_ENGINE_HPP

#include "basic_packet_info.hpp"
#include <vector>

int packet_capture_engine(std::vector<basic_packet_info>&, char*, int, int, bool);

#endif