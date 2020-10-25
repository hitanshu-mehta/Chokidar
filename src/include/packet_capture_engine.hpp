#ifndef BASIC_CAPTURE_ENGINE_HPP
#define BASIC_CAPTURE_ENGINE_HPP

#include "basic_packet_info.hpp"
#include "src/include/packet_capture.hpp"
#include <vector>

class packet_capture_engine
{
private:
	std::vector<basic_packet_info>& buffer;
	char* filter_exp;
	int num_pkts;
	int timeout;
	bool is_promiscious;
	int rounds;
	packet_capture* capture;
	void init();

public:
	packet_capture_engine(std::vector<basic_packet_info>&);
	packet_capture_engine(std::vector<basic_packet_info>&, char*, int, int, bool);
	packet_capture* get_packet_capture();
	void sniff();
};

#endif