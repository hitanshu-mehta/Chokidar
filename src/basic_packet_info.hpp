#ifndef BASIC_PACKET_INFO_HPP
#define BASIC_PACKET_INFO_HPP

#include "constants.hpp"
#include "utils.hpp"

#include <cstdint>
#include <netinet/in.h>
#include <string>

class basic_packet_info
{

private:
	long id = -1;
	in_addr src;
	in_addr dst;
	net::port_t src_port;
	net::port_t dst_port;
	int protocol;
	long timestamp; // not set
	long payload_bytes = 0;
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
	int tcp_window = 0;
	long header_bytes = 0; // not set
	int payload_packet = 0;

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

	long const get_id() { return this->id; }
	in_addr const get_src() { return this->src; }
	in_addr const get_dst() { return this->dst; };
	net::port_t const get_src_port() { return this->src_port; }
	net::port_t const get_dst_port() { return this->dst_port; }
	std::string get_fwd_flow_id();
	std::string get_bwd_flow_id();
	std::string get_flow_id();
	int const get_tcp_window() { return this->tcp_window; }
	long const get_header_bytes() { return this->header_bytes; }
	long const get_timestamp() { return this->timestamp; }
	long const get_payload_bytes() { return this->payload_bytes; }
	int const get_protocol() { return this->protocol; }

	void set_id() { this->id = utils::id++; }
	void set_src(in_addr src) { this->src = src; }
	void set_dst(in_addr dst) { this->dst = dst; }
	void set_src_port(net::port_t src_port) { this->src_port = src_port; }
	void set_dst_port(net::port_t dst_port) { this->dst_port = dst_port; }
	void set_protocol(int protocol) { this->protocol = protocol; }
	void set_timestamp(long timestamp) { this->timestamp = timestamp; }
	void set_TCPwindow(int TCPwindow) { this->tcp_window = TCPwindow; }
	void set_payload_bytes(long bytes) { this->payload_bytes = bytes; }
	void set_header_bytes(long bytes) { this->header_bytes = bytes; }

	void set_flagFIN(bool flagFIN) { this->flagFIN = flagFIN; }
	void set_flagPSH(bool flagPSH) { this->flagPSH = flagPSH; }
	void set_flagURG(bool flagURG) { this->flagURG = flagURG; }
	void set_flagECE(bool flagECE) { this->flagECE = flagECE; }
	void set_flagSYN(bool flagSYN) { this->flagSYN = flagSYN; }
	void set_flagACK(bool flagACK) { this->flagACK = flagACK; }
	void set_flagCWR(bool flagCWR) { this->flagCWR = flagCWR; }
	void set_flagRST(bool flagRST) { this->flagRST = flagRST; }

	/* --------------------  */
	void const print_all_info();
	int get_payload_packet() { return this->payload_packet += 1; }
};

#endif