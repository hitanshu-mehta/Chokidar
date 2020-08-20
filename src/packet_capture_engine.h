#ifndef BASIC_CAPTURE_ENGINE_H
#define BASIC_CAPTURE_ENGINE_H

int packet_capture_engine(char* filter_exp = "ip and not src host 127.0.0.1",
						  int num_pkts = -1,
						  int timeout = 10,
						  bool is_promiscious = true);

#endif