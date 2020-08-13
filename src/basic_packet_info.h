#ifndef BASIC_PACKET_INFO
#define BASIC_PACKET_INFO

#include "constants.h"
#include <cstdint>
#include <netinet/in.h>
#include <string>

class basic_packet_info
{

private:
	long id;
	in_addr ip_src;
	in_addr ip_dst;
	net::port_t src_port;
	net::port_t dst_port;
	int protocol;
	long timestamp;
	long payloadbytes;
	std::string flowid = nullptr;
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
	bool has_FIN() { return flagFIN; }
	bool has_PSH() { return flagPSH; }
	bool has_URG() { return flagURG; }
	bool has_ECE() { return flagECE; }
	bool has_SYN() { return flagSYN; }
	bool has_ACK() { return flagACK; }
	bool has_CWR() { return flagCWR; }
	bool has_RST() { return flagRST; }

	/* --------------------  */

	basic_packet_info(in_addr, in_addr, net::port_t, net::port_t, int, long);
	void const print_all_info();
};

#endif