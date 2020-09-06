#ifndef FLOW_GENERATOR_HPP
#define FLOW_GENERATOR_HPP

#include "basic_flow.hpp"
#include "basic_packet_info.hpp"
#include "database.hpp"
#include "session_stats.hpp"

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
	flow_generator() { }
	flow_generator(bool, long, long);
	void init();

	void add_packet(basic_packet_info);
	int get_flow_count();
	int dump_labeled_current_flow_to_file(std::string);
	int dump_labeled_current_flow_to_db(database* const);
	void update_app_sesssion_statistics(session_stats*, int);
};

#endif