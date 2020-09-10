#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <atomic>
#include <mutex>

// basic
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

#include <bsoncxx/document/value.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

/* 
	Singleton class.
	Because mongodb allows only one connection.
	Also adding thread safety. (In case i make this application multi-threaded.)
*/
class database
{

private:
	mongocxx::instance* inst;
	mongocxx::client* c;
	mongocxx::database db;
	static database* instance;
	bool is_capped = true;
	database();
	static std::atomic<database*> s_instance;
	static std::mutex s_mutex;

public:
	static database* get_instance();
	mongocxx::database* get_db() { return &this->db; }
};

#endif