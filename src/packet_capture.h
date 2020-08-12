#ifndef PACKET_CAPTURE_H
#define PACKET_CAPTURE_H

#include <pcap.h>

class PacketCapture
{
private:
#define SNAP_LEN 1518			   /* default snap length (maximum bytes per packet to capture) */
	char* dev = NULL;			   /* capture device name */
	char errbuf[PCAP_ERRBUF_SIZE]; /* error buffer */
	pcap_t* handle;				   /* packet capture handle */

	char filter_exp[];		/* filter expression */
	struct bpf_program fp;	/* compiled filter program (expression) */
	bpf_u_int32 mask;		/* subnet mask */
	bpf_u_int32 net;		/* ip */
	int num_packets = 1000; /* number of packets to capture */
	bool ispromiscious;
	int timeout;

public:
	~PacketCapture();
	char* find_dev();
	pcap_t* find_handle();
	void get_net_num_and_mask();
	bool is_ethernet_handle();
	void compile_filter_expression();
	void apply_filter();
	int sniff();
};

#endif