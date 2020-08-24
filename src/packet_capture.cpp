#include "packet_capture.hpp"

packet_capture::packet_capture() {
	int num_packets = 30;
	bool ispromiscious = 1;
	int timeout = 1000;
}

packet_capture::packet_capture(char* filter_exp, int num_pkts, int t_out, bool is_promiscious)
	: filter_exp(filter_exp)
	, num_packets(num_pkts)
	, timeout(t_out)
	, ispromiscious(is_promiscious){};

char* packet_capture::find_dev() {
	if(pcap_findalldevs(&alldevs, this->errbuf) == 0)
		if(alldevs == NULL) return dev = nullptr;
	dev = alldevs->name;
	return dev;
}

pcap_t* packet_capture::find_handle() {
	if(dev == NULL) return NULL;
	this->handle =
		pcap_open_live(this->dev, SNAP_LEN, this->ispromiscious, this->timeout, this->errbuf);
	return this->handle;
}

void packet_capture::set_filter(char* flt_exp) { filter_exp = flt_exp; }

void packet_capture::set_timeout(int time) { timeout = time; }

void packet_capture::set_num_packets(int num_pkts) { num_packets = num_pkts; }

char* const packet_capture::get_errbuf() { return errbuf; };

void const packet_capture::get_net_num_and_mask() {
	if(pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
		net = 0;
		mask = 0;
	}
}

char* const packet_capture::get_dev() { return dev; }

pcap_t* const packet_capture::get_handle() { return handle; }

std::vector<basic_packet_info> const packet_capture::get_basic_pkts_from_parser() {
	return parser.get_basic_pkts();
}

bool const packet_capture::is_ethernet_handle() { return (pcap_datalink(handle) == DLT_EN10MB); }

int packet_capture::compile_filter_expression() {
	return pcap_compile(handle, &fp, filter_exp, 0, net);
}

int packet_capture::apply_filter() { return pcap_setfilter(handle, &fp); }

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
	// return pcap_loop(handle, num_packets, got_packet, NULL);
	return pcap_dispatch(handle, num_packets, got_packet, NULL);
}

packet_capture::~packet_capture() {
	pcap_freecode(&fp);
	pcap_close(handle);
}