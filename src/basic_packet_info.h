#ifndef BASIC_PACKET_INFO
#define BASIC_PACKET_INFO

#include "constants.h"
#include <cstdint>
#include <netinet/in.h>
#include <string>

class basic_packet_info
{

private:
	long id = 0;
	in_addr ip_src;
	in_addr ip_dst;
	net::port_t src_port;
	net::port_t dst_port;
	int protocol;
	long timestamp;
	long payloadbytes;
	std::string flowid = "";
	int all_flags = 0;
	/* --------------------  */
	bool flagFIN = false;
	bool flagPSH = false;
	bool flagURG = false;
	bool flagECE = false;
	bool flagSYN = false;
	bool flagACK = false;
	bool flagCWR = false;
	bool flagRST = false;
	int TCPwindow = 0;
	long headerBytes;
	int payloadPacket = 0;

public:
	basic_packet_info(){};
	basic_packet_info(in_addr, in_addr, net::port_t, net::port_t, int, long);
	// total 8
	bool has_FIN() { return flagFIN; }
	bool has_PSH() { return flagPSH; }
	bool has_URG() { return flagURG; }
	bool has_ECE() { return flagECE; }
	bool has_SYN() { return flagSYN; }
	bool has_ACK() { return flagACK; }
	bool has_CWR() { return flagCWR; }
	bool has_RST() { return flagRST; }

	in_addr const get_ip_src();
	in_addr const get_ip_dst();
	net::port_t const get_src_port();
	net::port_t const get_dst_port();

	void set_ip_src(in_addr ip_src) { this->ip_src = ip_src; }
	void set_ip_dst(in_addr ip_dst) { this->ip_dst = ip_dst; }
	void set_src_port(net::port_t src_port) { this->src_port = src_port; }
	void set_dst_port(net::port_t dst_port) { this->dst_port = dst_port; }
	void set_protocol(int protocol) { this->protocol = protocol; }
	void set_timestamp(long timestamp) { this->timestamp = timestamp; }

	void set_flagFIN(bool flagFIN) { this->flagFIN = flagFIN; }
	void set_flagPSH(bool flagPSH) { this->flagPSH = flagPSH; }
	void set_flagURG(bool flagURG) { this->flagURG = flagURG; }
	void set_flagECE(bool flagECE) { this->flagECE = flagECE; }
	void set_flagSYN(bool flagSYN) { this->flagSYN = flagSYN; }
	void set_flagACK(bool flagACK) { this->flagACK = flagACK; }
	void set_flagCWR(bool flagCWR) { this->flagCWR = flagCWR; }
	void set_flagRST(bool flagRST) { this->flagRST = flagRST; }

	void set_TCPwindow(int TCPwindow) { this->TCPwindow = TCPwindow; }
	/* --------------------  */
	void const print_all_info();
};

#endif