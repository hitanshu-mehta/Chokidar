#include "database.hpp"

database* database::instance;
std::atomic<database*> database::s_instance;
std::mutex database::s_mutex;

database::database() {
	using bsoncxx::builder::basic::kvp;
	this->inst = new mongocxx::instance();
	this->c = new mongocxx::client(
		mongocxx::uri{"mongodb://127.0.0.1:27017"}); // default is localhost, port 27017
	this->db = c->database("idsdb");

	if(!this->db.has_collection("flows")) {
		auto collection_options = bsoncxx::builder::basic::document{};
		// max size of capped collection is 10 MB or 5000 documents.
		collection_options.append(kvp("capped", true), kvp("size", 100000000), kvp("max", 5000));
		fprintf(stderr, "Created flows");
		db.create_collection("flows", collection_options.view());
	}
	if(!this->db.has_collection("attacks")) { db.create_collection("attacks"); }
}

database* database::get_instance() {
	database* p = s_instance.load(std::memory_order_acquire);
	if(p == nullptr) {
		std::lock_guard<std::mutex> lock(s_mutex);
		p = s_instance.load(std::memory_order_relaxed);
		if(p == nullptr) {
			p = new database();
			s_instance.store(p, std::memory_order_release);
		}
	}
	return p;
}
