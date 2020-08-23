#include "packet_capture.hpp"

int packet_capture_engine(std::vector<basic_packet_info>& buffer,
						  char* filter_exp,
						  int num_pkts = 100,
						  int timeout = 1000,
						  bool is_promiscious = true) {

	packet_capture capture(filter_exp, num_pkts, timeout, is_promiscious);
	/* First find the device to capture */

	if(capture.find_dev() == NULL) {
		fprintf(stderr, "Couldn't find default device: %s\n", capture.get_errbuf());
		return 1;
	}
	/* get network number and mask associated with capture device */
	capture.get_net_num_and_mask();

	// /* print capture info */
	// printf("Device: %s\n", dev);
	// printf("Number of packets: %d\n", num_packets);
	// printf("Filter expression: %s\n", filter_exp);

	/* open capture device */
	capture.find_handle();
	if(!capture.valid_handle()) {
		fprintf(stderr, "Couldn't open device %s: %s\n", capture.get_dev(), capture.get_errbuf());
		return 2;
	}

	/* make sure we're capturing on an Ethernet device  */
	if(!capture.is_ethernet_handle()) {
		fprintf(stderr, "%s is not an Ethernet\n", capture.get_dev());
		return 3;
	}

	/* compile the filter expression */
	if(capture.compile_filter_expression() == -1) {
		fprintf(stderr,
				"Couldn't parse filter %s: %s\n",
				filter_exp,
				pcap_geterr(capture.get_handle()));
		return 4;
	}

	/* apply the compiled filter */
	if(capture.apply_filter() == -1) {
		fprintf(stderr,
				"Couldn't install filter %s: %s\n",
				filter_exp,
				pcap_geterr(capture.get_handle()));
		return 5;
	}

	/* now we can start sniffing */
	capture.sniff();

	/* get iterator of basic packet info vector of captured packets */
	buffer = capture.get_basic_pkts_from_parser();
	for(auto& ele : buffer) { ele.print_all_info(); }

	printf("\nCapture complete.\n");
	return 0;
}