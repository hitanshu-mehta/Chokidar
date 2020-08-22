#include "basic_packet_info.h"
#include "flow_generator_engine.h"
#include "packet_capture_engine.h"



#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <vector>

int main() {
	std::vector<basic_packet_info> buffer;
	char* filter = strdup("ip and not src host 127.0.0.1 ");
	packet_capture_engine(buffer, filter, 100, 1000, true);
	flow_generator_engine(120000000L, 5000000L, 0, "/home/hitanshu/output.txt", buffer);
}