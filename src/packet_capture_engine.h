#ifndef BASIC_CAPTURE_ENGINE_H
#define BASIC_CAPTURE_ENGINE_H

#include "basic_packet_info.h"
#include <vector>

int packet_capture_engine(std::vector<basic_packet_info>&, char*, int, int, bool);

#endif