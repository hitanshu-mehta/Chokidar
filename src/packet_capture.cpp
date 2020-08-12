#include "packet_capture.h"

char* PacketCapture::find_dev() {
	this->dev = pcap_lookupdev(this->errbuf);
	return dev;
}

pcap_t* PacketCapture::find_handle() {
	if(dev == NULL) return NULL;
	this->handle =
		pcap_open_live(this->dev, SNAP_LEN, this->ispromiscious, this->timeout, this->errbuf);
	return this->handle;
}

void PacketCapture::get_net_num_and_mask() {
	if(pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
		net = 0;
		mask = 0;
	}
}

bool PacketCapture::is_ethernet_handle() { return (pcap_datalink(handle) == DLT_EN10MB); }

void PacketCapture::compile_filter_expression() {
	if(pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
	}
}
void PacketCapture::apply_filter() {
	if(pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
	}
}

/* TO CHANGE */
static void got_packet(u_char* args, const struct pcap_pkthdr* header, const u_char* packet) {
	PacketCapture* sniffer = reinterpret_cast<PacketCapture*>(args);
}

int PacketCapture::sniff() {
	return pcap_loop(handle, num_packets, got_packet, reinterpret_cast<u_char*>(this));
}

PacketCapture::~PacketCapture() {
	pcap_freecode(&fp);
	pcap_close(handle);
}