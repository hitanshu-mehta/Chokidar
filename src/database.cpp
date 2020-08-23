#include "database.hpp"

// #include <bsoncxx/document/view.hpp>

database::database() {
	mongocxx::instance instance{};
	mongocxx::client client{mongocxx::uri{}}; // default is localhost, port 27017
	this->db = client["idsdb"];

	using bsoncxx::builder::basic::kvp;
	auto collection_options = bsoncxx::builder::basic::document{};
	// max size of capped collection is 10 MB or 5000 documents.
	collection_options.append(kvp("capped", true), kvp("size", 10000000), kvp("max", 5000));
	flows = db.create_collection("flows", collection_options.view());

	attacks = db.create_collection("attacks");
}

database* database::get_instance() {
	// double-checked locking
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

bool database::insert_doc(mongocxx::collection coll, bsoncxx::document::value doc) {
	try {
		coll.insert_one(doc.view());
		return true;
	}
	catch(...) {
		return false;
	}
}