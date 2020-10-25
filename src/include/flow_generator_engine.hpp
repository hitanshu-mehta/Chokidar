#ifndef FLOW_GENERATOR_ENGINE_HPP
#define FLOW_GENERATOR_ENGINE_HPP

#include "database.hpp"
#include "src/include/basic_packet_info.hpp"
#include "src/include/flow_generator.hpp"

void flow_generator_engine(long, long, std::string, std::vector<basic_packet_info>&);
void flow_generator_engine(long, long, database* const, std::vector<basic_packet_info>&);
void _flow_generator_engine(flow_generator&, std::vector<basic_packet_info>&);

#endif