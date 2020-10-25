#include "src/include/packet_capture_engine.hpp"
#include <iostream>
#include <string.h>

packet_capture_engine::packet_capture_engine(std::vector<basic_packet_info>& buff)
	: buffer(buff) {
	char* filter_exp = strdup(
		"ip and not src host 127.0.0.1 and not port 27017"); // mongodb client runs on port 27017
	this->num_pkts = 100;
	this->timeout = 1000;
	this->is_promiscious = true;
	this->rounds = 100;
	init();
};

packet_capture_engine::packet_capture_engine(
	std::vector<basic_packet_info>& buff, char* filter, int no_pkts, int tout, bool promiscious)
	: buffer(buff)
	, filter_exp(filter)
	, num_pkts(no_pkts)
	, timeout(tout)
	, is_promiscious(promiscious) {

	init();
}

void packet_capture_engine::sniff() {

	/* now we can start sniffing */
	this->capture->sniff();
	try {
		/* get iterator of basic packet info vector of captured packets */
		this->buffer = capture->get_basic_pkts_from_parser();
	}
	catch(const std::bad_alloc& e) {
		std::cout << "Allocation failed for buffer: " << e.what() << "\n";
	}

	for(auto& ele : buffer) { ele.print_all_info(); }

	printf("\nCapture complete.\n");
}

packet_capture* packet_capture_engine::get_packet_capture() { return capture; }

void packet_capture_engine::init() {

	capture = new packet_capture(filter_exp, num_pkts, timeout, is_promiscious);

	/* First find the device to capture */
	if(capture->find_dev() == NULL) {
		fprintf(stderr, "Couldn't find default device: %s\n", capture->get_errbuf());
	}
	/* get network number and mask associated with capture device */
	capture->get_net_num_and_mask();

	// /* print capture info */
	// printf("Device: %s\n", dev);
	// printf("Number of packets: %d\n", num_packets);
	// printf("Filter expression: %s\n", filter_exp);

	/* open capture device */
	capture->find_handle();
	if(!capture->valid_handle()) {
		fprintf(stderr, "Couldn't open device %s: %s\n", capture->get_dev(), capture->get_errbuf());
	}

	/* make sure we're capturing on an Ethernet device  */
	if(!capture->is_ethernet_handle()) {
		fprintf(stderr, "%s is not an Ethernet\n", capture->get_dev());
	}

	/* compile the filter expression */
	if(capture->compile_filter_expression() == -1) {
		fprintf(stderr,
				"Couldn't parse filter %s: %s\n",
				filter_exp,
				pcap_geterr(capture->get_handle()));
	}

	/* apply the compiled filter */
	if(capture->apply_filter() == -1) {
		fprintf(stderr,
				"Couldn't install filter %s: %s\n",
				filter_exp,
				pcap_geterr(capture->get_handle()));
	}
}