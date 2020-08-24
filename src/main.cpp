#include "basic_packet_info.hpp"
#include "database.hpp"
#include "flow_generator_engine.hpp"
#include "packet_capture_engine.hpp"

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <vector>

int main() {
	std::vector<basic_packet_info> buffer;
	char* filter = strdup(
		"ip and not src host 127.0.0.1 and not port 27017"); // mongodb client runs on port 27017
	database* const db = database::get_instance();
	packet_capture_engine(buffer, filter, 100, 1000, true);
	flow_generator_engine(120000000L, 5000000L, "/home/hitanshu/output.txt", buffer);
	flow_generator_engine(120000000L, 5000000L, db, buffer);
}