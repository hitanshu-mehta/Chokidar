#include "ether_header.h"
#include "packet_capture.h"
// #include "packet_parser.h"
#include "protocols.h"

#include <cstdint>
#include <cstdio>
#include <iostream>

int main() {
	packet_capture pkt;
	pkt.find_dev();
	pkt.find_handle();
	pkt.get_net_num_and_mask();
	pkt.set_filter("ip");
	pkt.compile_filter_expression();
	pkt.apply_filter();
	pkt.sniff();
}