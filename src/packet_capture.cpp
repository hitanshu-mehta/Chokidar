#include "packet_capture.h"
#include "packet_parser.h"

char* packet_capture::find_dev() {
	this->dev = pcap_lookupdev(this->errbuf);
	return dev;
}

pcap_t* packet_capture::find_handle() {
	if(dev == NULL) return NULL;
	this->handle =
		pcap_open_live(this->dev, SNAP_LEN, this->ispromiscious, this->timeout, this->errbuf);
	return this->handle;
}

void set_filter() { }

void packet_capture::get_net_num_and_mask() {
	if(pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
		net = 0;
		mask = 0;
	}
}

bool packet_capture::is_ethernet_handle() { return (pcap_datalink(handle) == DLT_EN10MB); }

void packet_capture::compile_filter_expression() {
	if(pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
	}
}
void packet_capture::apply_filter() {
	if(pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
	}
}

/* TO CHANGE */
void packet_capture::got_packet(u_char* args,
								const struct pcap_pkthdr* header,
								const u_char* packet) {
	// packet_capture* sniffer = reinterpret_cast<packet_capture*>(args);
	parser.set_args(args);
	parser.set_header(header);
	parser.set_packet(packet);
	parser.parse();
}

int packet_capture::sniff() {
	fprintf(stderr, "Sniffing\n");
	return pcap_loop(handle, num_packets, got_packet, NULL);
}

packet_capture::~packet_capture() {
	pcap_freecode(&fp);
	pcap_close(handle);
}