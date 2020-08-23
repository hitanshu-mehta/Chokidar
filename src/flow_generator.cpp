#include "flow_generator.hpp"
#include <fstream>

flow_generator::flow_generator(bool bidirectional, long flow_timeout, long activity_timeout) {
	this->bidirectional = bidirectional;
	this->flow_timeout = flow_timeout;
	this->flow_activity_timeout = activity_timeout;
	init();
}

void flow_generator::init() {
	this->current_flows.clear();
	this->finished_flows.clear();
	this->ip_addresses.clear();
	this->finished_flow_count = 0;
}

void flow_generator::add_packet(basic_packet_info packet) {
	if(packet.get_id() == -1) return;
	printf("adding packet\n");
	basic_flow flow;
	long current_timestamp = packet.get_timestamp();
	std::string id;

	if(this->current_flows.count(packet.get_fwd_flow_id()) > 0 ||
	   this->current_flows.count(packet.get_bwd_flow_id()) > 0) {
		/* add in one of the current flows */

		if(this->current_flows.count(packet.get_fwd_flow_id()) > 0) {
			id = packet.get_fwd_flow_id();
		}
		else
			id = packet.get_bwd_flow_id();
		flow = current_flows[id];
		// Flow finished due flowtimeout:
		// 1.- we move the flow to finished flow list
		// 2.- we eliminate the flow from the current flow list
		// 3.- we create a new flow with the packet-in-process

		if(current_timestamp - flow.get_flow_start_time() > flow_timeout) {
			if(flow.packet_count() > 1) { finished_flows[get_flow_count()] = flow; }

			current_flows.erase(id);
			current_flows[id] = basic_flow();
		}

		// Flow finished due FIN flag (tcp only):
		// 1.- we add the packet-in-process to the flow (it is the last packet)
		// 2.- we move the flow to finished flow list
		// 3.- we eliminate the flow from the current flow list
		else if(packet.has_FIN()) {
			flow.add_packet(packet);
			finished_flows[get_flow_count()] = flow;
			current_flows.erase(id);
		}
		else {
			flow.update_active_idle_time(current_timestamp, this->flow_activity_timeout);
			flow.add_packet(packet);
			current_flows[id] = flow;
		}
	}
	else {
		/* add new flow */
		current_flows[packet.get_fwd_flow_id()] =
			basic_flow(this->bidirectional, packet, this->flow_activity_timeout);
	}
}

int flow_generator::get_flow_count() {
	this->finished_flow_count++;
	return this->finished_flow_count;
}

int flow_generator::dump_labeled_current_flow(std::string output_path) {
	std::ofstream output_file(output_path, std::ios::app);
	int total = 0;
	for(auto flow : current_flows) {
		if(flow.second.packet_count() > 1) {
			std::string tmp = flow.second.dump_flow_based_features_S();
			std::cout << tmp << "\n";
			output_file << tmp + "\n";
			++total;
		}
	}
	return total;
}
