#ifndef FLOW_GENERATOR_H
#define FLOW_GENERATOR_H

#include "basic_flow.h"
#include "basic_packet_info.h"
#include <map>
#include <string>
#include <vector>

class flow_generator
{
private:
	// FlowGenListener mListener;
	std::map<std::string, basic_flow> current_flows;
	std::map<int, basic_flow> finished_flows;
	std::map<std::string, std::vector<int>> ip_addresses;

	bool bidirectional;
	long flow_timeout;
	long flow_activity_timeout;
	int finished_flow_count;

public:
	void add_packet(basic_packet_info);
	int get_flow_count();
};

#endif