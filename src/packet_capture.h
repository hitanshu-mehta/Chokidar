#ifndef PACKET_CAPTURE_H
#define PACKET_CAPTURE_H

#include "basic_packet_info.h"
#include "packet_parser.h"
#include <pcap.h>
#include <vector>

class packet_capture
{
private:
#define SNAP_LEN 1518			   /* default snap length (maximum bytes per packet to capture) */
	char* dev = NULL;			   /* capture device name */
	char errbuf[PCAP_ERRBUF_SIZE]; /* error buffer */
	pcap_t* handle = NULL;		   /* packet capture handle */

	char* filter_exp;	   /* filter expression */
	struct bpf_program fp; /* compiled filter program (expression) */
	bpf_u_int32 mask;	   /* subnet mask */
	bpf_u_int32 net;	   /* ip */
	int num_packets = 30;  /* number of packets to capture */
	bool ispromiscious = 1;
	int timeout = 1000;

	std::vector<basic_packet_info*>* basic_pkts;
	packet_parser parser;

public:
	packet_capture(char*, int, int, bool);

	char* find_dev();
	pcap_t* find_handle();
	inline static packet_parser parser = packet_parser();

	void set_timeout(int time);
	void set_filter(char*);
	void set_num_packets(int);
	bool set_parser();

	void const get_net_num_and_mask();
	char* const get_errbuf();
	char* const get_dev();
	pcap_t* const get_handle();

	bool const is_ethernet_handle();
	bool valid_handle() { return handle != NULL; }

	int compile_filter_expression();
	int apply_filter();

	static void got_packet(u_char*, const struct pcap_pkthdr*, const u_char*);
	int sniff();
	~packet_capture();
};

#endif