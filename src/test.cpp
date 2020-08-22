// #include <cstdlib>
// #include <pcap.h>

// /* default snap length (maximum bytes per packet to capture) */
// #define SNAP_LEN 1518

// /* ethernet headers are always exactly 14 bytes */
// #define SIZE_ETHERNET 14

// /* Ethernet addresses are 6 bytes */
// #define ETHER_ADDR_LEN 6

// /* Ethernet header */
// struct sniff_ethernet {
// 	u_char ether_dhost[ETHER_ADDR_LEN]; /*destination host address*/
// 	u_char ether_shost[ETHER_ADDR_LEN]; /* source host address*/
// 	u_short ether_types;				/* IP ? ARP ? etc */
// };

// /* IP header */
// struct sniff_ip {
// 	u_char ip_vhl;				   /* version << 4 | header length >> 2 */
// 	u_char ip_tos;				   /* type of service */
// 	u_short ip_len;				   /* total length */
// 	u_short ip_id;				   /* identification */
// 	u_short ip_off;				   /* fragment offset field */
// #define IP_RF 0x8000			   /* reserved fragment flag */
// #define IP_DF 0x4000			   /* dont fragment flag */
// #define IP_MF 0x2000			   /* more fragments flag */
// #define IP_OFFMASK 0x1fff		   /* mask for fragmenting bits */
// 	u_char ip_ttl;				   /* time to live */
// 	u_char ip_p;				   /* protocol */
// 	u_short ip_sum;				   /* checksum */
// 	struct in_addr ip_src, ip_dst; /* source and dest address */
// };
// #define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
// #define IP_V(ip) (((ip)->ip_vhl) >> 4)

// /* TCP header */
// typedef u_int tcp_seq;

// struct sniff_tcp {
// 	u_short th_sport; /* source port */
// 	u_short th_dport; /* destination port */
// 	tcp_seq th_seq;	  /* sequence number */
// 	tcp_seq th_ack;	  /* acknowledgement number */
// 	u_char th_offx2;  /* data offset, rsvd */
// #define TH_OFF(th) (((th)->th_offx2 & 0xf0) >> 4)
// 	u_char th_flags;
// #define TH_FIN 0x01
// #define TH_SYN 0x02
// #define TH_RST 0x04
// #define TH_PUSH 0x08
// #define TH_ACK 0x10
// #define TH_URG 0x20
// #define TH_ECE 0x40
// #define TH_CWR 0x80
// #define TH_FLAGS (TH_FIN | TH_SYN | TH_RST | TH_ACK | TH_URG | TH_ECE | TH_CWR)
// 	u_short th_win; /* window */
// 	u_short th_sum; /* checksum */
// 	u_short th_urp; /* urgent pointer */
// };

// void got_packet(u_char* args, const struct pcap_pkthdr* header, const u_char* packet) { }

// int main() {
// 	char* dev = NULL;			   /* capture device name */
// 	char errbuf[PCAP_ERRBUF_SIZE]; /* error buffer */
// 	pcap_t* handle;				   /* packet capture handle */

// 	char filter_exp[] = "ip"; /* filter expression */
// 	struct bpf_program fp;	  /* compiled filter program (expression) */
// 	bpf_u_int32 mask;		  /* subnet mask */
// 	bpf_u_int32 net;		  /* ip */
// 	int num_packets = 1000;	  /* number of packets to capture */

// 	/* find capture device */
// 	dev = pcap_lookupdev(errbuf);
// 	if(dev == NULL) {
// 		fprintf(stderr, "Couldn't find the default device: %s\n", errbuf);
// 		exit(EXIT_FAILURE);
// 	}

// 	/* get network number and mask associated with capture device */
// 	if(pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
// 		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
// 		net = 0;
// 		mask = 0;
// 	}

// 	/* print the capture info */
// 	printf("Device %s\n", dev);
// 	printf("Number of packets: %d\n", num_packets);

// 	/* open capture device */
// 	handle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf);
// 	if(handle == NULL) {
// 		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
// 		exit(EXIT_FAILURE);
// 	}

// 	/* make sure we're capturing on an Ethernet device [2] */
// 	if(pcap_datalink(handle) != DLT_EN10MB) {
// 		fprintf(stderr, "%s is not an Ethernet\n", dev);
// 		exit(EXIT_FAILURE);
// 	}

// 	/* compile the filter expression */
// 	if(pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
// 		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
// 		exit(EXIT_FAILURE);
// 	}

// 	/* apply the compiled filter */
// 	if(pcap_setfilter(handle, &fp) == -1) {
// 		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
// 		exit(EXIT_FAILURE);
// 	}

// 	/* now we can set our callback function */
// 	pcap_loop(handle, num_packets, got_packet, NULL);

// 	/* cleanup */
// 	pcap_freecode(&fp);
// 	pcap_close(handle);

// 	printf("\nCapture complete.\n");

// 	return 0;
// }
