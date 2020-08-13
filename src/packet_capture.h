#ifndef PACKET_CAPTURE_H
#define PACKET_CAPTURE_H

#include "packet_parser.h"
#include <pcap.h>

class packet_capture
{
private:
#define SNAP_LEN 1518			   /* default snap length (maximum bytes per packet to capture) */
	char* dev = NULL;			   /* capture device name */
	char errbuf[PCAP_ERRBUF_SIZE]; /* error buffer */
	pcap_t* handle;				   /* packet capture handle */

	char* filter_exp;	   /* filter expression */
	struct bpf_program fp; /* compiled filter program (expression) */
	bpf_u_int32 mask;	   /* subnet mask */
	bpf_u_int32 net;	   /* ip */
	int num_packets = 10;  /* number of packets to capture */
	bool ispromiscious = 1;
	int timeout = 1000;

public:
	~packet_capture();
	char* find_dev();
	pcap_t* find_handle();
	inline static packet_parser parser = packet_parser();
	void set_timeout(int time) { timeout = time; }
	void get_net_num_and_mask();
	bool is_ethernet_handle();
	void compile_filter_expression();
	void apply_filter();
	void set_filter(char* flt_exp) { filter_exp = flt_exp; }
	static void got_packet(u_char* args, const struct pcap_pkthdr* header, const u_char* packet);
	int sniff();
};

#endif