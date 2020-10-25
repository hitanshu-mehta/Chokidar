#include "src/include/flow_generator_engine.hpp"

#include <ctime>

void flow_generator_engine(long flow_timeout,
						   long activity_timeout,
						   std::string file_path,
						   std::vector<basic_packet_info>& buffer) {
	printf("generating flows\n");

	flow_generator flow_gen = flow_generator(true, flow_timeout, activity_timeout);
	_flow_generator_engine(flow_gen, buffer);

	printf("dumping the flows into file\n");
	flow_gen.dump_labeled_current_flow_to_file(file_path);
	printf("done\n");
}

void flow_generator_engine(long flow_timeout,
						   long activity_timeout,
						   database* const db,
						   std::vector<basic_packet_info>& buffer) {

	printf("generating flows\n");
	flow_generator flow_gen = flow_generator(true, flow_timeout, activity_timeout);
	_flow_generator_engine(flow_gen, buffer);

	printf("dumping the flows into database\n");
	flow_gen.dump_labeled_current_flow_to_db(db);
	printf("done\n");
}

void _flow_generator_engine(flow_generator& flow_gen, std::vector<basic_packet_info>& buffer) {
	int n_valid = 0;
	int n_total = 0;
	int n_discarded = 0;
	long start = std::time(0) * 1000; // time in miliseconds
	for(int i = 0; i < (int)buffer.size(); ++i) {
		try {
			++n_total;
			if(buffer[i].get_id() != -1) {
				flow_gen.add_packet(buffer[i]);
				++n_valid;
			}
			else {
				fprintf(stderr, "Packet discarded\n");
				++n_discarded;
			}
		}
		catch(...) {
			break;
		}
	}
}